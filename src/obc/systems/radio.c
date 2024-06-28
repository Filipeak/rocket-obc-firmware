#include "radio.h"
#include "board_config.h"
#include "sensors.h"
#include "voltage.h"
#include "ign.h"
#include "ahrs.h"
#include "sm.h"
#include "../middleware/syslog.h"
#include "../middleware/events.h"
#include "lib/crypto/crc.h"
#include "hal/uart_driver.h"
#include "hal/time_tracker.h"
#include <math.h>

#define SYSTEM_NAME "radio"

static uint8_t s_CurSize = 0;
static uint8_t s_RecvBuffer[64];
static uint8_t s_BufLen = 0;
static msec_t s_TimeOffset;
static msec_t s_RecoveryTimeOffset;
static uint8_t s_Seq;
static radio_tlm_parsed_data_t s_CurrentParsedData;

static uint16_t _get_alt(void);
static uint8_t _get_control_flags(void);
static radio_obc_frame_t _create_packet(void);

void radio_init(void)
{
    hal_uart_init_all(OBC_UART, OBC_UART_RX, OBC_UART_TX, OBC_UART_FREQ);

    SYS_LOG("READY");
}

void radio_update(void)
{
    if ((s_TimeOffset != 0 && hal_time_get_ms_since_boot() - s_TimeOffset >= 250) || (hal_time_get_ms_since_boot() - s_RecoveryTimeOffset >= 2000))
    {
        radio_obc_frame_t frame = _create_packet();
        const size_t len = sizeof(frame);

        uint8_t lenBuff = (uint8_t)len;
        hal_uart_write(OBC_UART, &lenBuff, 1);

        uint8_t *buffer = (uint8_t *)&frame;
        hal_uart_write(OBC_UART, buffer, len);

        SYS_LOG("Sent %d bytes", len);

        s_TimeOffset = 0;
        s_RecoveryTimeOffset = hal_time_get_ms_since_boot();
    }

    if (hal_uart_is_readable(OBC_UART))
    {
        uint8_t byte;
        hal_uart_read(OBC_UART, &byte, 1);

        if (s_CurSize == 0)
        {
            if (byte <= sizeof(s_RecvBuffer))
            {
                s_CurSize = byte;

                SYS_LOG("Received request for %d bytes", s_CurSize);
            }
        }
        else
        {
            s_RecvBuffer[s_BufLen++] = byte;

            if (s_BufLen == s_CurSize)
            {
                SYS_LOG("All bytes received!");

                radio_tlm_frame_t *frame = (radio_tlm_frame_t *)s_RecvBuffer;

                if (frame->magic != RADIO_MAGIC || frame->crc != crc16_mcrf4xx_calculate((const uint8_t *)frame, sizeof(radio_tlm_frame_t) - 2))
                {
                    return;
                }

                SYS_LOG("Packet is valid!");

                s_CurrentParsedData = (radio_tlm_parsed_data_t){
                    .arm_enabled = (frame->flags & RADIO_TLM_FLAG_ARM_ENABLE) > 0,
                    .arm_disabled = (frame->flags & RADIO_TLM_FLAG_ARM_DISABLE) > 0,
                    .v3v3_enabled = (frame->flags & RADIO_TLM_FLAG_3V3_ENABLE) > 0,
                    .v3v3_disabled = (frame->flags & RADIO_TLM_FLAG_3V3_DISABLE) > 0,
                    .v5_enabled = (frame->flags & RADIO_TLM_FLAG_5V_ENABLE) > 0,
                    .v5_disabled = (frame->flags & RADIO_TLM_FLAG_5V_DISABLE) > 0,
                    .vbat_enabled = (frame->flags & RADIO_TLM_FLAG_VBAT_ENABLE) > 0,
                    .vbat_disabled = (frame->flags & RADIO_TLM_FLAG_VBAT_DISABLE) > 0,
                };

                events_publish(MSG_RADIO_PACKET_RECEIVED);

                s_TimeOffset = hal_time_get_ms_since_boot();

                s_CurSize = 0;
                s_BufLen = 0;
            }
        }
    }
}

const radio_tlm_parsed_data_t *radio_get_parsed_data(void)
{
    return &s_CurrentParsedData;
}

static uint16_t _get_alt(void)
{
    float tmp = sensors_get_frame()->baroHeight - (sm_get_state() == FLIGHT_STATE_STANDING ? (float)sensors_get_frame()->pos.alt : sm_get_base_alt());

    return tmp > 0 ? (uint16_t)tmp : 0;
}

static uint8_t _get_control_flags(void)
{
    uint8_t flags = 0;
    uint8_t voltageFlags = voltage_get_flags();

    if (ign_is_armed())
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_ARMED;
    }
    if (voltageFlags & VOLTAGE_PIN_STATE_3V3)
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_3V3;
    }
    if (voltageFlags & VOLTAGE_PIN_STATE_5V)
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_5V;
    }
    if (voltageFlags & VOLTAGE_PIN_STATE_VBAT)
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_VBAT;
    }
    if (true)
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_CALIBRATION;
    }
    if (sensors_get_frame()->gpsFix)
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_GPS;
    }
    if (true)
    {
        flags |= RADIO_OBC_FRAME_CONTROL_FLAGS_SELF_TEST;
    }

    return flags;
}

static radio_obc_frame_t _create_packet(void)
{
    radio_obc_frame_t frame = {
        .magic = RADIO_MAGIC,
        .roll = ahrs_get_data()->rotation.x,
        .pitch = ahrs_get_data()->rotation.y,
        .yaw = ahrs_get_data()->rotation.z,
        .velocity = sqrtf(ahrs_get_data()->velocity.x * ahrs_get_data()->velocity.x + ahrs_get_data()->velocity.y * ahrs_get_data()->velocity.y + ahrs_get_data()->velocity.z * ahrs_get_data()->velocity.z),
        .batteryVoltage10 = sensors_get_frame()->batVolts * 10,
        .batteryPercentage = sensors_get_frame()->batPercent,
        .lat = sensors_get_frame()->pos.lat,
        .lon = sensors_get_frame()->pos.lon,
        .alt = _get_alt(),
        .state = (uint8_t)sm_get_state(),
        .controlFlags = _get_control_flags(),
        .seq = s_Seq,
    };

    frame.crc = crc16_mcrf4xx_calculate((const uint8_t *)&frame, sizeof(frame) - 2);

    s_Seq = s_Seq == 255 ? 0 : s_Seq + 1;

    return frame;
}
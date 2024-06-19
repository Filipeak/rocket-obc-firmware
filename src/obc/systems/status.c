#include "status.h"
#include "board_config.h"
#include "sensors.h"
#include "dataman.h"
#include "ign.h"
#include "../middleware/events.h"
#include "lib/drivers/led/w2812_driver.h"

#define WS_BRIGHTNESS 0.05f
#define WS_COLOR(r, g, b) ws2812_get_color((uint8_t)((r) * WS_BRIGHTNESS), (uint8_t)((g) * WS_BRIGHTNESS), (uint8_t)((b) * WS_BRIGHTNESS))

#define IGN_FUSE_WORKING_IGN_PRESENT_FACTOR 0
#define IGN_FUSE_WORKING_IGN_NOT_PRESENT_FACTOR 0.0189607f
#define IGN_FUSE_NOT_WORKING_IGN_PRESENT_FACTOR 0.0297897f
#define IGN_FUSE_NOT_WORKING_IGN_NOT_PRESENT_FACTOR 0.0383104f
#define IGN_PIN_CHECK_EPS 0.005f

static ws2812_color_t s_Diodes[7];
static msec_t s_OtherDiodesTimer;

static void _update_diodes(void);
static ws2812_color_t _get_ign_diode_color(float v);
static ws2812_color_t _get_arm_diode_color(void);
static ws2812_color_t _get_bat_diode_color(void);
static ws2812_color_t _get_ready_diode_color(void);

void status_init(void)
{
    ws2812_init(PIN_LED, false);

    s_Diodes[0] = WS_COLOR(255, 0, 0);
    s_Diodes[1] = WS_COLOR(255, 0, 0);
    s_Diodes[2] = WS_COLOR(255, 0, 0);
    s_Diodes[3] = WS_COLOR(255, 0, 0);
    s_Diodes[4] = WS_COLOR(255, 0, 0);
    s_Diodes[5] = WS_COLOR(255, 0, 0);
    s_Diodes[6] = WS_COLOR(255, 0, 0);

    _update_diodes();
}

void status_update(void)
{
    if (events_poll(MSG_SENSORS_ADC_READ))
    {
        s_Diodes[0] = _get_ign_diode_color(sensors_get_frame()->ignDet1Volts);
        s_Diodes[1] = _get_ign_diode_color(sensors_get_frame()->ignDet2Volts);
        s_Diodes[2] = _get_ign_diode_color(sensors_get_frame()->ignDet3Volts);
        s_Diodes[3] = _get_ign_diode_color(sensors_get_frame()->ignDet4Volts);
        s_Diodes[5] = _get_bat_diode_color();

        _update_diodes();
    }

    if (hal_time_run_every_ms(500, &s_OtherDiodesTimer))
    {
        s_Diodes[4] = _get_ready_diode_color();
        s_Diodes[6] = _get_arm_diode_color();

        _update_diodes();
    }
}

static void _update_diodes(void)
{
    ws2812_set_colors(s_Diodes, sizeof(s_Diodes) / sizeof(ws2812_color_t));
}

static ws2812_color_t _get_ign_diode_color(float v)
{
    if (sensors_get_frame()->batPercent == 0)
    {
        return WS_COLOR(0, 0, 0);
    }

    float vref = sensors_get_frame()->batVolts;

    if (v < vref * (IGN_FUSE_WORKING_IGN_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        return WS_COLOR(0, 255, 0);
    }
    else if (v < vref * (IGN_FUSE_WORKING_IGN_NOT_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        return WS_COLOR(255, 0, 0);
    }
    else if (v < vref * (IGN_FUSE_NOT_WORKING_IGN_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        return WS_COLOR(255, 165, 0);
    }
    else if (v < (vref * IGN_FUSE_NOT_WORKING_IGN_NOT_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        return WS_COLOR(255, 165, 0);
    }
    else
    {
        return WS_COLOR(0, 0, 0);
    }
}

static ws2812_color_t _get_arm_diode_color(void)
{
    return ign_is_armed() ? WS_COLOR(0, 255, 0) : WS_COLOR(255, 0, 0);
}

static ws2812_color_t _get_bat_diode_color(void)
{
    uint8_t percent = sensors_get_frame()->batPercent;

    if (percent == 0)
    {
        return WS_COLOR(0, 0, 0);
    }

    if (percent < 50)
    {
        return WS_COLOR(255, 255 * percent / 50, 0);
    }
    else
    {
        return WS_COLOR(255 - 255 * (percent - 50) / 50, 255, 0);
    }
}

static ws2812_color_t _get_ready_diode_color(void)
{
    return dataman_is_ready() ? WS_COLOR(0, 255, 0) : WS_COLOR(255, 0, 0);
}
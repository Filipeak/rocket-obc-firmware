#pragma once

#include <LoRa.h>

#define RADIO_PACKET_SIGNATURE_LENGTH 16
#define RADIO_PACKET_PAYLOAD_SIZE 128
#define RADIO_COMMAND_MEASUREMENT 'M'

static const uint8_t RADIO_PACKET_SIGNATURE[RADIO_PACKET_SIGNATURE_LENGTH] = {
    0x3d,
    0x97,
    0x4d,
    0x0d,
    0x35,
    0x43,
    0x1e,
    0x2f,
    0x32,
    0x6b,
    0x56,
    0x2e,
    0x37,
    0x39,
    0x30,
    0x39,
};

typedef struct MeasurementData
{
    float pos_x;
    float pos_y;
    float pos_z;
    float roll;
    float pitch;
    float yaw;
    float latitude;
    float longitude;
    float altitude;
    float velocity;
} MeasurementData;

typedef struct RadioHeader
{
    uint8_t signature[RADIO_PACKET_SIGNATURE_LENGTH];
} RadioHeader;

typedef struct RadioBody
{
    uint8_t command;
    uint8_t _padding[3];
    uint8_t payload[RADIO_PACKET_PAYLOAD_SIZE];
    size_t payloadSize;
} RadioBody;

typedef struct RadioPacket
{
    RadioHeader header;
    RadioBody body;
} RadioPacket;

static const uint8_t RADIO_PACKET_KEY[] = {
    0x18,
    0x07,
    0x45,
    0x95,
    0xc0,
    0x24,
    0xb0,
    0x49,
    0x06,
    0xfe,
    0x95,
    0x4c,
    0xe7,
    0x10,
    0xfd,
    0x3c,
    0x5f,
    0x4e,
    0xc1,
    0x2f,
    0xed,
    0xdb,
    0xb3,
    0xd1,
    0x82,
    0x45,
    0x9d,
    0x74,
    0x31,
    0x0b,
    0xd6,
    0x82,
    0x3d,
    0xb1,
    0x97,
    0xdd,
    0x01,
    0x30,
    0x7e,
    0x87,
    0xee,
    0x9e,
    0x15,
    0xdf,
    0x1f,
    0x6d,
    0x8c,
    0x16,
    0x99,
    0x37,
    0xb0,
    0xc0,
    0x9f,
    0x56,
    0xf8,
    0x5d,
    0xbd,
    0x5e,
    0xd4,
    0xae,
    0x4d,
    0x23,
    0x6c,
    0xa0,
};

class MyLoRa
{
public:
    void Init();
    void Check();
    int GetRssi();

private:
    float m_Rssi;
    uint8_t m_Buffer[256];
    MeasurementData m_CurrentMeasurement;

    void HandlePacket();
    void BufferEncryptDecrypt(uint8_t *buffer, size_t bufferSize);
};
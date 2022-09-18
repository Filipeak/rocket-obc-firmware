#pragma once

#include "shared/measurements_utils.h"

typedef struct ReceiverInternalConditionData
{
    float measureRAMUsagePercent;
    float receiverRAMUsagePercent;
    float measureBatteryPercent;
    int radioSignalStrength;
} ReceiverInternalConditionData;

typedef struct ReceiverSendData
{
    MeasurementData measurement;
    ReceiverInternalConditionData condition;
} ReceiverSendData;

void initializeReceiverLogger();
void logReceiverData(ReceiverSendData *data);
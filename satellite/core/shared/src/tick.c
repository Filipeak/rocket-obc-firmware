#include "shared/tick.h"
#include "kernel/logging/logger.h"

VOID tick(TickData *data)
{
    if (runEvery(TICK_DELAY_MS, &data->_time))
    {
        MY_LOG_CORE_INFO("TICK");
    }
}
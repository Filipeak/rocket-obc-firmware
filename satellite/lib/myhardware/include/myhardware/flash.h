#pragma once

#include "utils/errors_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define FLASH_FILES_MAX_COUNT 4

/**
 * @brief Flash file structure. (INTERNAL USE)
 */
typedef struct _FlashFile
{
    /**
     * @brief File name.
     */
    const char *_name;

    /**
     * @brief Address offset in memory.
     */
    uint32_t _addressOffset;

    /**
     * @brief File size.
     */
    size_t _bytes;

    /**
     * @brief Buffer to write (Flash only allows to write 256 byte pages!).
     */
    uint8_t *_buffer;

    /**
     * @brief Buffer size.
     */
    size_t _bufferSize;
} _FlashFile;

/**
 * @brief Flash structure. All changes are done internally, so keep it just for reference!
 */
typedef struct FlashModule
{
    /**
     * @brief Is Flash module initialized.
     */
    bool _isInitialized;

    /**
     * @brief Flash files.
     */
    _FlashFile _files[FLASH_FILES_MAX_COUNT];

    /**
     * @brief Flash files count.
     */
    size_t _filesCount;
} FlashModule;

/**
 * @brief Initialize flash module.
 *
 * @param flashModule Flash module to setup
 * @return Result code.
 */
FUNCRESULT flashInit(FlashModule *flashModule);

/**
 * @brief Initialize file in flash module.
 *
 * @param module Flash module.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT flashInitFile(FlashModule *module, const char *fileName);

/**
 * @brief Get the Default Flash Module object
 *
 * @return Pointer to default Flash Module
 */
FlashModule *getDefaultFlashModule();

/**
 * @brief Write data to flash module. (Only 256 byte pages can be written, so data is buffered internally!)
 *
 * @param module Flash module.
 * @param fileName File name.
 * @param msg Text to write.
 * @return Result code.
 */
FUNCRESULT flashWriteFile(FlashModule *module, const char *fileName, const char *msg);

/**
 * @brief Clear file
 *
 * @param module Flash module.
 * @param fileName File name to clear.
 * @return Result code.
 */
FUNCRESULT flashFlushFile(FlashModule *module, const char *fileName);

/**
 * @brief Read file.
 *
 * @param module Flash module.
 * @param fileName File name to read from.
 * @param buffer_ptr Pointer to buffer to read to.
 * @param size Size of buffer.
 * @return FUNCRESULT
 */
FUNCRESULT flashGetFile(FlashModule *module, const char *fileName, uint8_t **buffer_ptr, size_t *size);

/**
 * @brief Terminate flash module. After this call, this file is read-only
 *
 * @param module Flash module.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT flashFileTerminate(FlashModule *module, const char *fileName);
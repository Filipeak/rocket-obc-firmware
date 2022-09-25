#pragma once

#include "tools/typedefs.h"

#define COMMANDS_MAX_COUNT 16
#define COMMAND_ARGUMENTS_COUNT_ERROR_MSG "Invalid arguments count!"

/**
 * @brief Command function structure.
 */
typedef void (*CommandFunction)(STRING *args, SIZE argc);

/**
 * @brief Command data structure.
 */
typedef struct CommandData
{
    /**
     * @brief Command name.
     */
    STRING name;

    /**
     * @brief Command function.
     */
    CommandFunction func;
} CommandData;

/**
 * @brief Command arguments data structure. All changes are done internally, so keep it just for reference!
 */
typedef struct CommandArgs
{
    /**
     * @brief Command arguments.
     */
    STRING *args;

    /**
     * @brief Command arguments count.
     */
    SIZE size;
} CommandArgs;

/**
 * @brief Register command.
 *
 * @param command Command to register.
 *
 * @return True if command was registered successfully, false otherwise.
 */
BOOL registerCommand(CommandData *command);

/**
 * @brief Retrieve command and its args with tokens.
 *
 * @param tokens Tokens to parse.
 * @param tokensSize Size of tokens.
 * @param args_out_ptr Command args to setup.
 * @return Pointer to command data
 */
CommandData *parseCommand(STRING *tokens, SIZE tokensSize, CommandArgs *args_out_ptr);

/**
 * @brief Executes command.
 *
 * @param command Command to execute.
 * @param args Args for command.
 *
 * @return True if command was executed successfully, false otherwise.
 */
BOOL executeCommand(CommandData *command, CommandArgs *args);

/**
 * @brief Checks if command args size is valid.
 *
 * @param expectedCount Expected args count.
 * @param actualCount Actucal count of args
 * @param output_ptr Pointer to output string.
 * @return True if args size is valid, false otherwise.
 */
BOOL checkArgsCount(SIZE expectedCount, SIZE actualCount, STRING *output_ptr);

/**
 * @brief Clears command args.
 *
 * @param args Args to clear.
 */
BOOL commandClearArgs(CommandArgs *args);
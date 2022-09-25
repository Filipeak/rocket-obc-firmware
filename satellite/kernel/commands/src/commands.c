#include "kernel/commands/commands.h"
#include "kernel/logging/logger.h"
#include "kernel/logging/recorder.h"
#include <stdlib.h>
#include <string.h>

static CommandData s_Commands[COMMANDS_MAX_COUNT];
static SIZE s_CommandSize;

BOOL registerCommand(CommandData *command)
{
    FUNCTION_PROFILE_BEGIN();

    if (!command)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return FALSE;
    }

    if (s_CommandSize >= COMMANDS_MAX_COUNT)
    {
        MY_LOG_CORE_ERROR("Commands are full!");
        FUNCTION_PROFILE_END();

        return FALSE;
    }

    s_Commands[s_CommandSize] = *command;
    s_CommandSize++;

    FUNCTION_PROFILE_END();

    return TRUE;
}

CommandData *parseCommand(STRING *tokens, SIZE tokensSize, CommandArgs *args_out_ptr)
{
    FUNCTION_PROFILE_BEGIN();

    if (!tokens || tokensSize == 0 || !args_out_ptr)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return NULL;
    }

    MY_LOG_CORE_INFO("Parsing command...");

    memset(args_out_ptr, 0, sizeof(CommandArgs));

    CommandData *command = NULL;

    for (SIZE i = 0; i < s_CommandSize; i++)
    {
        if (strcmp(s_Commands[i].name, tokens[0]) == 0)
        {
            command = &s_Commands[i];
            break;
        }
    }

    if (command)
    {
        MY_LOG_CORE_INFO("Command found: %s", command->name);

        args_out_ptr->size = tokensSize - 1;

        if (args_out_ptr->size > 0)
        {
            args_out_ptr->args = (STRING *)malloc(args_out_ptr->size * sizeof(STRING));

            for (SIZE i = 1; i < tokensSize; i++)
            {
                args_out_ptr->args[i - 1] = (STRING)malloc((strlen(tokens[i]) + 1) * sizeof(CHAR));

                strcpy(args_out_ptr->args[i - 1], tokens[i]);
            }
        }

        MY_LOG_CORE_INFO("Command arguments size: %d", args_out_ptr->size);

        FUNCTION_PROFILE_END();

        return command;
    }
    else
    {
        MY_LOG_CORE_ERROR("Command not found!");

        args_out_ptr->size = 0;

        FUNCTION_PROFILE_END();

        return NULL;
    }
}

BOOL executeCommand(CommandData *command, CommandArgs *args)
{
    FUNCTION_PROFILE_BEGIN();

    if (!command || !args)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return FALSE;
    }

    MY_LOG_CORE_INFO("Executing command: %s", command->name);

    command->func(args->args, args->size);

    MY_LOG_CORE_INFO("Command executed: %s", command->name);

    FUNCTION_PROFILE_END();

    return TRUE;
}

BOOL checkArgsCount(SIZE expectedCount, SIZE actualCount, STRING *output_ptr)
{
    if (actualCount != expectedCount)
    {
        *output_ptr = COMMAND_ARGUMENTS_COUNT_ERROR_MSG;

        return FALSE;
    }

    return TRUE;
}

BOOL commandClearArgs(CommandArgs *args)
{
    FUNCTION_PROFILE_BEGIN();

    if (!args)
    {
        MY_LOG_CORE_ERROR("Invalid input");
        FUNCTION_PROFILE_END();

        return FALSE;
    }

    if (args->size > 0)
    {
        for (SIZE i = 0; i < args->size; i++)
        {
            free(args->args[i]);
        }

        free(args->args);

        args->size = 0;
    }

    FUNCTION_PROFILE_END();

    return TRUE;
}
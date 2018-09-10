#include "log.h"

char Log::s_Buffer[PRINT_BUFFER_SIZE];


void Log::Info(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf(s_Buffer, PRINT_BUFFER_SIZE, format, args);
        
    va_end(args);

    printf("[Info]: %s\n", s_Buffer);
}

void Log::Warning(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf(s_Buffer, PRINT_BUFFER_SIZE, format, args);
        
    va_end(args);

    printf("[Warning]: %s\n", s_Buffer);
}

void Log::Error(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf(s_Buffer, PRINT_BUFFER_SIZE, format, args);
        
    va_end(args);

    printf("[Error]: %s\n", s_Buffer);
}

void Log::Fatal(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf(s_Buffer, PRINT_BUFFER_SIZE, format, args);
        
    va_end(args);

    printf("[Fatal]: %s\n", s_Buffer);
}

//TODO(patrik): Need a better check for system calls
bool Log::SystemCheck(int result, const char* cause)
{
    if(result == -1)
    {
        //NOTE(patrik): Error
        char* strError = strerror(errno);
        Error("System Call Error: %s - %s", cause, strError);
        return false;
    }

    return true;
}

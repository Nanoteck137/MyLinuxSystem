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

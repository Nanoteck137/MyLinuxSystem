#include "log.h"

char Log::s_Buffer[PRINT_BUFFER_SIZE];

void Log::InternalPrint(const char* file, int line, LogType type, const char* format, va_list args)
{
    vsnprintf(s_Buffer, PRINT_BUFFER_SIZE, format, args);

    switch(type)
    {
        case LogType::INFO:    printf("[Info] "); break;
        case LogType::WARNING: printf("[Warning] "); break;
        case LogType::ERROR:   printf("[Error] "); break;
        case LogType::FATAL:   printf("[Fatal] "); break;
    }

    if(file)
    {
        printf("(%s:%d) ", file, line);
    }
    
    printf(": %s\n", s_Buffer);
}

void Log::Print(const char* file, int line, LogType type, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    InternalPrint(file, line, type, format, args);
    
    va_end(args);
}

void Log::Print(LogType type, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    InternalPrint(nullptr, 0, type, format, args);
    
    va_end(args);
}

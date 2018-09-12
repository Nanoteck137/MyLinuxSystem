#pragma once

/* TODO(patrik):
     - Configuration from lua?????
     - Loggin to file also
     - Add some colors
 */

#define PRINT_BUFFER_SIZE 1024 * 10

#define LogInfo(x...)    Log::Print(__FILE__, __LINE__, LogType::INFO, x)
#define LogWarning(x...) Log::Print(__FILE__, __LINE__, LogType::WARNING, x)
#define LogError(x...)   Log::Print(__FILE__, __LINE__, LogType::ERROR, x)
#define LogFatal(x...)   Log::Print(__FILE__, __LINE__, LogType::FATAL, x)

enum class LogType {
        INFO,
        WARNING,
        ERROR,
        FATAL
};

class Log
{  
private:
    static char s_Buffer[];
private:
    Log() { }
public:
    static void Print(const char* file, int line, LogType type, const char* format, ...);
    static void Print(LogType type, const char* format, ...);
private:
    static void InternalPrint(const char* file, int line, LogType type, const char* format, va_list args);
};

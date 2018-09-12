#pragma once

/* TODO(patrik):
     - Configuration from lua?????
     - Loggin to file also
     - Add some colors
 */

#define PRINT_BUFFER_SIZE 1024 * 10

class Log
{
private:
    static char s_Buffer[];
private:
    Log() { }
public:
    static void Info(const char* format, ...);
    static void Warning(const char* format, ...);
    static void Error(const char* format, ...);
    static void Fatal(const char* format, ...);
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <memory>
#include <functional>

#include <lua.hpp>

#include "lua_manager.cpp"
#include "log.cpp"

/* TODO(patrik):
     - Handle all power/reboot/halt signals
     - Singal handler
     - Util classes for Write/Read files
 */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BBLK "\x1B[40m"
#define BRED "\x1B[41m"

/*
40 Black
41 Red
42 Green
43 Yellow
44 Blue
45 Magenta
46 Cyan
47 White
*/

void Test()
{
    printf("\033[37;41mHello World\033[0m\n");
    Log::Info("Hello World");
    Log::Info("Hello World22");
    Log::Info("Hello");

    Log::Warning("Hello World");
    Log::Warning("Hello World22");
    Log::Warning("Hello");

    Log::Error("Hello World");
    Log::Error("Hello World22");
    Log::Error("Hello");

    Log::Fatal("Hello World");
    Log::Fatal("Hello World22");
    Log::Fatal("Hello");
}

bool WriteFile(const char* filename, const char* str)
{
	int fd = open(filename, O_WRONLY | O_CREAT);
    if(!Log::SystemCheck(fd, "open"))
    {
        return false;
    }
            
    write(fd, str, strlen(str));
    
    close(fd);

    return true;
}

typedef struct stat Stat;

std::string ReadFile(const char* filename)
{
    int fd = open(filename, O_RDONLY);
   
    if(!Log::SystemCheck(fd, "open"))
    {        
        return std::string();
    }

    Stat st;
    fstat(fd, &st);
    int length = st.st_size;
    
    std::string result(length, 0);

    read(fd, &result[0], length);
    close(fd);
    
    return std::move(result);
}

bool ExecuteCommand(const char* command, char* args[], pid_t* childID)
{   
    pid_t processID = fork();
    if(!Log::SystemCheck(processID, "fork"))
    {
        return false;
    }
    
    if(processID == 0)
    {
        setsid();

        close(0);
        close(1);
        close(2);

        open("/dev/ttyS0", O_RDONLY, 0);
        open("/dev/ttyS0", O_WRONLY, 0);
        open("/dev/ttyS0", O_WRONLY, 0);

        //NOTE: Change the process code
        int result = execv(command, args);
        if(result == -1)
        {
            char* errorStr = strerror(errno);
            printf("Execv failed: %s\nError: %s\n", command, errorStr);
            
            return false;
        }
    }

    if(childID)
	    *childID = processID;
    
    return true;
}

static void catch_function(int signo)
{
    if(signo == SIGUSR1)
    {
        //NOTE(patrik): Halt
        Log::Info("Got Signal SIGUSR1");
    }
    else if(signo == SIGUSR2)
    {
        //NOTE(patrik): Poweroff
        Log::Info("Got Signal SIGUSR2");
    }
    else if(signo == SIGTERM)
    {
        //NOTE(patrik): Reboot
        Log::Info("Got Signal SIGTERM");
    }

    // reboot(RB_POWER_OFF);
}

int main(int argc, char** argv)
{
    const std::function<void(int)> test = &catch_function;
    
    //TODO(patrik): Change these signal functions
    typedef void funcTest(int);
    funcTest* const* func = test.target<void(*)(int)>();
    if(func == nullptr)
    {
        printf("Function pointer is null\n");
    }
    
    if(signal(SIGUSR1, *func) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    
    if(signal(SIGUSR2, *func) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    if(signal(SIGTERM, *func) == SIG_ERR)
    {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    Test();
    
    mount("/proc", "/proc", "proc",     0, 0);
    mount("/sys",  "/sys",  "sysfs",    0, 0);
    mount("/dev",  "/dev",  "devtmpfs", 0, 0);

    mkdir("/dev/pts", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mount("/dev/pts", "/dev/pts", "devpts", 0, 0);

    setenv("PATH", "/usr/bin:/usr/sbin", true);

    system("ifconfig eth0 10.0.2.15");
    system("route add default gw 10.0.2.2");
    system("dmesg -n 1");
    
    printf("NOTE: Initalization done\n");

    std::string configFile = ReadFile("/etc/init.conf.lua");

    std::unique_ptr<LuaManager> luaManager = std::make_unique<LuaManager>();
    luaManager->SetGlobal<std::string>("test", "Hello World");
    luaManager->RunScript(configFile);
    
    pid_t id = 0;
    char* args[] = { "login", NULL };
    bool result = false;
    int status = 0;
start:
	result = ExecuteCommand("/bin/busybox", args, &id);
    if(!result)
        return -1;

    do
    {
        pid_t w = waitpid(id, &status, WUNTRACED | WCONTINUED);
        if (w == -1)
        {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status))
        {
            printf("exited, status=%d\n", WEXITSTATUS(status));
            goto start;
        }
        else if (WIFSIGNALED(status))
        {
            printf("killed by signal %d\n", WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status))
        {
            printf("continued\n");
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));


    printf("NOTE: Got to return statement\n");
    return 0;
}

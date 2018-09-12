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

#include "log.cpp"

#include "lua_value.cpp"
#include "lua_manager.cpp"

/* TODO(patrik):
     - Handle all power/reboot/halt signals
     - Singal handler
     - Util classes for Write/Read files
 */

char* GetSystemError()
{
    char* strError = strerror(errno);
    
    return strError;
}

bool WriteFile(const char* filename, const char* str)
{
	int fd = open(filename, O_WRONLY | O_CREAT);
    if(fd == -1)
    {
        LogError("Could not open '%s' for writing - Error: %s", filename, GetSystemError());
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

    if(fd == -1)
    {
        LogError("Could not open '%s' for reading - Error: %s", filename, GetSystemError());
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
    if(processID == -1)
    {
        LogError("Could not fork process - Command: %s'", command);
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

static void HandlePowerSignals(int signalID)
{
    if(signalID == SIGUSR1)
    {
        //NOTE(patrik): Halt
        LogInfo("Got Signal SIGUSR1");
    }
    else if(signalID == SIGUSR2)
    {
        //NOTE(patrik): Poweroff
        LogInfo("Got Signal SIGUSR2");
    }
    else if(signalID == SIGTERM)
    {
        //NOTE(patrik): Reboot
        LogInfo("Got Signal SIGTERM");
    }

    // reboot(RB_POWER_OFF);
}

typedef void SignalFunction(int);

bool RegisterSignal(int signalID, const std::function<void(int)>& function)
{
    SignalFunction* const* funcPtr = function.target<void(*)(int)>();
    if(funcPtr == nullptr)
    {
        LogError("Could not get function pointer from std::function from '%s'", strsignal(signalID));
        return false;
    }
    
    if(signal(signalID, *funcPtr) == SIG_ERR)
    {
        LogError("Could not register singal '%s'", strsignal(signalID));
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{   
    RegisterSignal(SIGUSR1, HandlePowerSignals);
    RegisterSignal(SIGUSR2, HandlePowerSignals);
    RegisterSignal(SIGTERM, HandlePowerSignals);

    //TODO(patrik): Move all these commands to a "System Manager"
    mount("/proc", "/proc", "proc",     0, 0);
    mount("/sys",  "/sys",  "sysfs",    0, 0);
    mount("/dev",  "/dev",  "devtmpfs", 0, 0);

    mkdir("/dev/pts", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mount("/dev/pts", "/dev/pts", "devpts", 0, 0);

    setenv("PATH", "/usr/bin:/usr/sbin", true);

    system("ifconfig eth0 10.0.2.15");
    system("route add default gw 10.0.2.2");
    system("dmesg -n 1");
    //END TODO
    
    LogInfo("NOTE: Initalization done\n");

    std::string configFile = ReadFile("/etc/init.conf.lua");

    std::unique_ptr<LuaManager> luaManager = std::make_unique<LuaManager>();
    LuaValue* value = LuaValue::From(321.32f);
    
    luaManager->SetGlobal("test", value);
    
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

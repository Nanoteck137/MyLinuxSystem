#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <lua.hpp>

#include "lua_manager.cpp"

bool WriteFile(const char* filename, const char* str)
{
	int fd = open(filename, O_WRONLY | O_CREAT);
    if(fd == -1)
    {
        return false;
    }
            
    write(fd, str, strlen(str));
    
    close(fd);

    return true;
}

typedef struct stat Stat;

char* ReadFile(const char* filename)
{
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        //TODO: Write some errors
        return 0;
    }

    Stat st;
    fstat(fd, &st);
    int length = st.st_size;

    char* result = (char*)malloc(length);

    read(fd, result, length);
    close(fd);
    
    return result;
}

#define WARN_ONLY (1<<31)

int xcreate_stdio(char *path, int flags, int mode)
{
    int fd = open(path, (flags^O_CLOEXEC)&~WARN_ONLY, mode);

    //if (fd == -1) ((mode&WARN_ONLY) ? perror_msg_raw : perror_exit_raw)(path);
    return fd;
}

// Die unless we can open a file, returning file descriptor.
int xopen_stdio(char *path, int flags)
{
    return xcreate_stdio(path, flags, 0);
}

bool ExecuteCommand(const char* command, char* args[], pid_t* childID)
{   
    pid_t processID = fork();
    if(processID == -1)
    {
        char* errorStr = strerror(errno);
        
        printf("Error with forking a process: %s\nError: %s\n", command, errorStr);
    	return false;
    }
    
    if(processID == 0)
    {
        setsid();
        for(int i = 0; i < 3; i++)
        {
            close(i);
            xopen_stdio("/dev/ttyS0", O_RDWR | O_CLOEXEC);        
        }
        
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

int main(int argc, char** argv)
{
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

    char* configFile = ReadFile("/etc/init.conf.lua");
    
    LuaManager* luaManager = new LuaManager();
    luaManager->SetGlobal<lua_Number>("test", 123);
    luaManager->RunScript(configFile);
    
    delete luaManager;

    free(configFile);
    
    pid_t id = 0;
    char* args[] = { "login", NULL };
    bool result = false;
    int status = 0;
start:
	result = ExecuteCommand("/bin/busybox", args, &id);
    if(result == false)
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

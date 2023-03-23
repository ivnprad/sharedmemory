#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#define BUFFER_SIZE 31
//#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



class SharedMemory{
public:

    SharedMemory(const char* _name, int _shm_size, const bool CREATE_SHM);
    virtual ~SharedMemory(void);
    void* getShmBaseAdress(void);

private:
    const int shm_size;
    bool shmIsNotCreatedButLinked;
    char name[BUFFER_SIZE];
    int shm_fd; 
    SharedMemory(void); // avoid creating default constructor
    void* shmBaseAdress;
};

/*
#ifdef __XENO__
    #ifdef __COBALT__
        #define BUFFER_SIZE RESOURCE_NAME_SIZE_MAX
    #else
        #define BUFFER_SIZE 31
    #define CORE_IPC_VERBOSITY_LEVEL__ XenomaiOxfLogLevel
#elif __QNX__
    #define BUFFER_SIZE 31
    #define __CORE_IPC_VERBOSITY_LEVEL__ 0
#else
    #define BUFFER_SIZE 31
    #define __CORE_IPC_VERBOSITY_LEVEL__ 0
#endif


*/

/*

RT_HEAP is a real-time heap memory management library in the Robot Operating System (ROS) framework. 
It is used to allocate and deallocate memory dynamically in real-time systems, where the timing constraints are very strict and predictable.

#ifdef __XENO__
    typedef RT_HEAP %s;
#else 
    typedef int %s;

*/


#endif
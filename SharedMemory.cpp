#include "SharedMemory.hpp"
#include <iostream>
#include <sys/stat.h>

SharedMemory::SharedMemory(const char* _name, int _shm_size, const bool CREATE_SHM):
    shmIsNotCreatedButLinked(false), shmBaseAdress(nullptr),shm_size(_shm_size){

    /*
    #ifdef __XENO__
    #else
    #endif
    */

     // ------------------------open share memory 
     name[0] ='/';
     name[1] ='\0'; // strncat needs null terminator to concatenate _name to name 
     (void)strncat(name,_name,BUFFER_SIZE-2);
     shm_fd=-1;
     if(CREATE_SHM){
          shm_fd = shm_open(name,O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
     }else{
          shm_fd = shm_open(name,O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
          shmIsNotCreatedButLinked = (shm_fd != -1);
     }

     bool faulty_shm_creation = shm_fd == -1;
     if (faulty_shm_creation){
          std::cout << " shared memory creation failed" << std::string(name)<< std::endl;
     }

     // ------------------------configure the size of the shared memory segment
     int error = ftruncate(shm_fd,shm_size);
     if(error!=0){
          std::cout << " shared memory ftrucante wrong "<< std::endl;
     }

     //-------------------------map the shared memory segment to the address space of the process
     shmBaseAdress = (char*)mmap(0,shm_size,PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd,0);
     if (shmBaseAdress ==MAP_FAILED){
          std::cout <<" share memory map failed "<<std::endl;
     }

     //------------------------check of the size of the shared memory region using stat
     struct stat sb;
     if(fstat(shm_fd,&sb)==-1){
          std::cout << " fstat return an error"<< std::endl;
     }

     if(sb.st_size<shm_size){ // minimum block size 16384 Bytes
          std::cout<< " shared memory region actual size[bytes]: " << sb.st_size <<" expected size: "<< shm_size<<std::endl;
     }

     
}



void* SharedMemory::getShmBaseAdress(void){
     return shmBaseAdress;
}

SharedMemory::~SharedMemory(void){
     // unmap the shared memory segment
     int error  = munmap(shmBaseAdress,shm_size);
     if (error==-1){
          std::cout <<" shared emory munmap wrong"<<std::endl;
     }
     //close the shared memory segment
     error = close(shm_fd);
     if (error==-1){
          std::cout <<" shared memory close wrong"<<std::endl;
     }
     if(!shmIsNotCreatedButLinked){
          error = shm_unlink(name);
          if(error==-1){
               std::cout <<" shared memory unlink failed "<<std::endl;
          }
     }
}





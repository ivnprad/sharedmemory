#include <iostream>
#include "SharedMemory.hpp"
#include "SharedMemoryPointerManager.hpp"
#include <mutex>
#include <thread>
#include <atomic>
#include <future>
/* increment decrement */


int shared_variable = 0;
std::mutex mutex;
std::atomic_bool data_ready=false;
std::condition_variable cv;


void producer(){

    for (size_t i =0; i<10;++i){
        std::unique_lock<std::mutex> lock(mutex);
        //mutex.lock();
        int new_data = i;
        //while(data_ready){
            //mutex.unlock();
            //std::this_thread::yield();
            //mutex.lock();
            //cv.wait(lock); this replaces mutex.unlock(), yield and mutex lock
            /*
            Unlock the mutex: The first step is to unlock the mutex that is associated with the condition variable cv. This releases the exclusive lock on the shared data and allows other threads to access it.
            Put the thread to sleep: The second step is to put the thread to sleep, waiting for the condition variable to be signaled by another thread. The thread is removed from the CPU scheduling queue, allowing other threads to run.
            Wait for the signal: The third step is to wait for the condition variable cv to be signaled by another thread. When the signal is received, the thread is awoken and moved to the back of the CPU scheduling queue.
            Re-acquire the mutex: The fourth step is to re-acquire the mutex that is associated with the condition variable cv. Once the mutex is re-acquired, the thread can safely access the shared data again. 
            If the condition that the thread is waiting for is true, the thread can proceed with its work. Otherwise, the thread must release the mutex again and go back to sleep, waiting for the next signal.
            */
        //}
        cv.wait(lock,[&]{return !(data_ready.load());}); // this replaces (while data_ready){cv.wait(lock)}
        shared_variable = new_data;
        data_ready.store(true);
        std::cout << " producer data" << new_data <<std::endl;
        //mutex.unlock();
        lock.unlock();
        cv.notify_one();
    }

}

void consumer(){

    for (size_t i =0; i<10;++i){
        //mutex.lock();
        std::unique_lock<std::mutex> lock(mutex);
        //while(!data_ready){
            // mutex.unlock();
            // std::this_thread::yield();
            // mutex.lock();
            //cv.wait(lock);
        //}
        cv.wait(lock,[&]{return data_ready.load();});
        data_ready.store(false);
        std::cout << " consumer data" << shared_variable <<std::endl;
        //mutex.unlock();
        lock.unlock();
        cv.notify_one();
    }

}

/*
 PROMISE - FUTURE
*/

int fibonacci(int n){
    if (n<=1){
        return n;
    }else{
        return fibonacci(n-1)+fibonacci(n-2);
    }
}

void computeFibonacci(int n, std::promise<int>&& result_promise){
    int result = fibonacci(n);
    result_promise.set_value(result);
}


int main()
{
    
    //const int SHM_SIZE = 1024;  // shared memory size in bytes
    const int SHM_SIZE = 16385;  // shared memory size in bytes
    const char* SHM_NAME = "my_shared_memory"; // name of shared memory
    SharedMemory shared_memory(SHM_NAME, SHM_SIZE, true); // create shared memory
    SharedMemoryPointerManager<int> int_guard(shared_memory.getShmBaseAdress());
    int_guard.set(int(10));

    std::cout << " int shared memory " <<  *(static_cast<int*>(shared_memory.getShmBaseAdress()))<<std::endl;

    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    if (producerThread.joinable()){
        producerThread.join();
    }

    if(consumerThread.joinable()){
        consumerThread.join();
    }

    /* FUTURE - PROMISE */
    std::promise<int> result_promise;
    std::future<int> result_future = result_promise.get_future();

    std::thread fibonacciThread(computeFibonacci,6,std::move(result_promise));

    std::cout<<"waiting..."<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    auto valueSetByPromise = result_future.get();
    std::cout << " valueSetByPromise "<< valueSetByPromise << std::endl;

    if(fibonacciThread.joinable()){
        fibonacciThread.join();
    }

    return 0;  

}
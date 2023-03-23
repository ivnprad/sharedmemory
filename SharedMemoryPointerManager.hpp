#ifndef SHAREDMEMORYPOINTERMANAGER_H
#define SHAREDMEMORYPOINTERMANAGER_H

#include <memory>


template<typename T>
class SharedMemoryPointerManager {
public:
    // Constructor to allocate memory for the guarded object
    SharedMemoryPointerManager(void* baseAddress) {
        assert(baseAddress!=nullptr);
        ptr_ = reinterpret_cast<T*>(baseAddress);
    }

    SharedMemoryPointerManager<T>& operator=(const SharedMemoryPointerManager<T>& other){
        if (this!=&other){
            assert(other.ptr_!=nullptr);
            this->ptr_=other.ptr_;
        }
        return *this;
    }

    // Function to set the value of the guarded object
    void set(const T& data) {
        assert(ptr_!=nullptr);
        *ptr_ = data;
    }

    // Function to get a shared_ptr to the guarded object
    T* get() const {
        assert(ptr_!=nullptr);
        return ptr_;
    }

private:
    // Shared pointer to the guarded object
    T* ptr_;
};



#endif 
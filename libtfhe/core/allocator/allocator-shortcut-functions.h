#ifndef TFHE_ALLOCATOR_H
#error "allocator.h must be included to use this file"

#include "allocator.h" //just so that the editor don't freak out

template<>
class AllocatorImpl<0> { //idem
    void *allocate(size_t alignment, size_t byte_size);

    void deallocate(void *ptr);

#endif

    AllocatorImpl(const AllocatorImpl &alloc) = delete;

    AllocatorImpl &operator=(const AllocatorImpl &alloc)= delete;

    AllocatorImpl &operator=(AllocatorImpl &&alloc)= delete;



// This header is meant to be included within the class definition of an allocator
// it relies on a proper definition of allocate and deallocate, namely:

/*
 * allocates at least byte_size bytes of contiguous memory,
 * whose start and end addresses are multiples of alignment
 * @param byte_size minimum size to allocate (must be a multiple of alignment, consider using ceilalign if not sure)
 * @param alignment alignment of the begin and end area.
 */
//void *allocate(size_t alignment, size_t byte_size)=0;

/*
 * de-allocates a ptr that was previously allocated by this allocator
 * @param ptr a previously allocated address
 */
//void deallocate(void *ptr)=0;

/*
 * returns a child allocator, following the stack model:
 *  - An allocator can have only one (direct) child at any given point in time
 *  - During the lifespan of the child, no memory can be allocated or de-allocated by the father.
 * @param father the father allocator.
 * @param expected_size an expectation on the total memory that will be used by the allocator.
 *        Depending on the implementation, this expectation can be loose or strict
 * @return the child allocator
 */
//Allocator createStackChildAllocator(const size_t expected_size = 0)=0;

/**
 * Allocates and constructs a new object of type T, aligned with the specified alignment,
 * and call the constructor using args
 * @param alignment the alignment (power of 2)
 * @param args arguments to pass to the constructor
 */
    template<typename T, typename... ARGS>
    inline T *newObjectAligned(const size_t alignment, ARGS... args) {
        T *ptr = (T *) allocate(alignment, ceilalign(sizeof(T), alignment));
        new(ptr) T(args...);
        return ptr;
    };

/**
 * Allocates and constructs a new object of type T, aligned with the default alignment for the type T,
 * and call the constructor using args
 * @param args arguments to pass to the constructor
 */
    template<typename T, typename... ARGS>
    inline T *newObject(ARGS... args) {
        return newObjectAligned<T, ARGS...>(alignof(T), args...);
    };

/**
 * Destroys and deallocates an object allocated with newObject
 * @param ptr the previously created object
 */
    template<typename T, typename ...ARGS>
    inline void deleteObject(T *ptr, ARGS... args) {
        //ptr->~T();
        ptr->destroy(args...);
        deallocate(ptr);
    };

/**
 * Destroys and deallocates arithmetic types allocated with newObject or newAlignedObject
 * @param ptr the previously created array
 */
    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    inline void deleteObject(T *ptr) {
        deallocate(ptr);
    };

/**
 * Allocates and constructs a contiguous array of elements of type T,
 * The array is aligned with the specified alignment,
 * each element is contructed using args
 * @param n the number of elements to create
 * @param alignment the alignment (power of 2)
 * @param args arguments to pass to the constructor
 */
    template<typename T, typename... ARGS>
    inline T *newArrayAligned(size_t n, size_t alignment, ARGS... args) {
        T *ptr = (T *) allocate(alignment, ceilalign(n * sizeof(T), alignment));
        for (size_t i = 0; i < n; i++)
            new(ptr + i) T(args...);
        return ptr;
    };

/**
 * Allocates and constructs a contiguous array of elements of type T,
 * The array is aligned with the default alignment for type T,
 * each element is contructed using args
 * @param n the number of elements to create
 * @param args arguments to pass to the constructor
 */
    template<typename T, typename... ARGS>
    inline T *newArray(size_t n, ARGS... args) {
        return newArrayAligned<T, ARGS...>(n, alignof(T), args...);
    };

/**
 * Destroys and deallocates an array allocated with newArray or newAlignedArray
 * @param ptr the previously created array
 */
    template<typename T, typename... ARGS>
    inline void deleteArray(size_t n, T *ptr, ARGS... args) {
        for (size_t i = 0; i < n; i++) {
            //(ptr + i)->~T();
            (ptr + i)->destroy(args...);
        }
        deallocate(ptr);
    };

/**
 * Destroys and deallocates an array of arithmetic types allocated with newArray or newAlignedArray
 * @param ptr the previously created array
 */
    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    inline void deleteArray(size_t n, T *ptr) {
        deallocate(ptr);
    };


#ifndef TFHE_ALLOCATOR_H
};
#endif


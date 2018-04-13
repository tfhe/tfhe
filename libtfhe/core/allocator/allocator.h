#ifndef TFHE_ALLOCATOR_H
#define TFHE_ALLOCATOR_H

/**
 * @file allocator interface description
 * @brief allocator interface description
 */

#include <cstddef>
#include <memory>

const size_t default_stack_size = 50000000;

class Allocator;

#ifdef USE_TFHE_ALLOCATOR

#include "TFHEAllocator.h"

typedef TFHEAllocator AllocatorImpl;

#else

#include "ValgrindAllocator.h"

typedef ValgrindAllocator AllocatorImpl;

#endif

/**
 * returns the smallest multiple of alignment larger or equal to size
 * @param size must be positive
 * @param alignment must be a power of 2
 * @return the smallest multiple of alignment larger or equal to size
 */
inline size_t ceilalign(const size_t size, const size_t alignment) {
    return (size + alignment - 1) & (-alignment);
}

class Allocator : public AllocatorImpl {
public:
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
     * default constructor
     */
    template<typename ...ARGS>
    Allocator(ARGS... args):AllocatorImpl(args...) {}

    /**
     * destructor
     */
    ~Allocator() {}

    // An allocator cannot be copied. Move is ok, however.
    Allocator(const Allocator &alloc) = delete;

    Allocator(Allocator &&alloc) = default;

    Allocator &operator=(const Allocator &alloc)= delete;

    Allocator &operator=(Allocator &&alloc)= default;

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
    T *newObject(ARGS... args) {
        return newObjectAligned<T, ARGS...>(alignof(T), args...);
    };

    /**
     * Destroys and deallocates an object allocated with newObject
     * @param ptr the previously created object
     */
    template<typename T, typename ...ARGS>
    void deleteObject(T *ptr, ARGS... args) {
        //ptr->~T();
        ptr->destroy(args...);
        deallocate(ptr);
    };

    /**
     * Destroys and deallocates arithmetic types allocated with newObject or newAlignedObject
     * @param ptr the previously created array
     */
    template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    void deleteObject(T *ptr) {
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
    T *newArrayAligned(size_t n, size_t alignment, ARGS... args) {
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
    void deleteArray(size_t n, T *ptr, ARGS... args) {
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
    void deleteArray(size_t n, T *ptr) {
        deallocate(ptr);
    };

};

//static_assert(sizeof(Allocator)==16);

#endif //TFHE_ALLOCATOR_H

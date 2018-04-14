#ifndef TFHE_CASTABLE_UNIQUE_PTR_H
#define TFHE_CASTABLE_UNIQUE_PTR_H

/**
 * @brief This file defines non-templated smart pointers.
 * @file This file defines non-templated smart pointers.
 */

#include <cassert>
#include <functional>

/**
 * Utility function that deletes type D
 * @tparam D a static type
 * @param p pointer to delete
 */
template<typename D>
void delete_deleter(void *p) {
    delete reinterpret_cast<D *>(p);
}

/**
 * Utility function that deletes an array of type D
 * @tparam D a static type
 * @param p pointer to delete
 */
template<typename D>
void array_deleter(void *p) {
    delete[] reinterpret_cast<D *>(p);
}

/**
 * Our smart pointers will use this signature: void function(void*)
 */
typedef void (*DELETER_FUN)(void *);

/**
 * A castable unique pointer is a pointer to an arbitrary data type, which is automatically destructed when
 * the unique pointer gets out of scope.
 *
 * The main difference between std::unique_ptr and this pointer is that the class is not templated at rest:
 * the accessors as<T>() and asPtr<T>() cast the pointer on usage.
 *
 * Since a unique pointer owns its object, it cannot be copied. It can however be moved: in this case, ownership is
 * transfered to the destination, and the original pointer is invalidated.
 */
class CastableUniquePointer {
private:
    void *data;
    DELETER_FUN deleter;

    /** Actual function that auto-destructs the data */
    void deleteSmartPointer();

    /** Actual function that constructs and takes ownership */
    void constructGrab(void *data, DELETER_FUN deleter);

    /** Actual function that moves and transfers ownership */
    void constructMove(CastableUniquePointer &&smartPointer);

public:
    /** initializes a null pointer */
    CastableUniquePointer() : data(nullptr), deleter(nullptr) {}

    /** Construct and takes ownership of the data */
    template<typename D>
    CastableUniquePointer(D *data) {
        constructGrab(data, &delete_deleter<D>);
    }

    /** Construct and takes ownership of the data, and store the specified deleter to delete the object */
    CastableUniquePointer(void *data, DELETER_FUN deleter) {
        constructGrab(data, deleter);
    }

    /** Upon destruction, the data is automatically released */
    ~CastableUniquePointer() {
        if (deleter) deleteSmartPointer();
    }

    /** Copy constructor: FORBIDDEN */
    CastableUniquePointer(const CastableUniquePointer &) = delete;

    /** Copy Assignment Operator: FORBIDDEN */
    void operator=(const CastableUniquePointer &smartPointer)= delete;

    /** Copy this temporary, and take ownership of the data */
    CastableUniquePointer(CastableUniquePointer &&smartPointer) {
        constructMove(std::move(smartPointer));
    }

    /** move operator */
    void operator=(CastableUniquePointer &&smartPointer);

    /** function to reassign a unique-pointer */
    template<typename D>
    void reset(D *data) {
        deleteSmartPointer();
        constructGrab(data, &delete_deleter<D>);
    }

    /** casts and access the data as a const reference
     * @tparam S Mandatory type of the data
     */
    template<typename S>
    const S &as() const { return *(reinterpret_cast<const S *>(data)); }

    /** casts and access the data as a reference
     * @tparam S Mandatory type of the data
     */
    template<typename S>
    S &as() { return *(reinterpret_cast<S *>(data)); }

    /** casts and access the data as a const pointer
     * @tparam S Mandatory type of the data
     */
    template<typename S>
    const S *asPtr() const { return reinterpret_cast<const S *>(data); }

    /** casts and access the data as a pointer
     * @tparam S Mandatory type of the data
     */
    template<typename S>
    S *asPtr() { return (reinterpret_cast<S *>(data)); }

    /**
     * @brief gets a weak reference to the same object
     */
    CastableUniquePointer weakRef() const { return CastableUniquePointer(data, nullptr); }

};

#endif //TFHE_CASTABLE_UNIQUE_PTR_H

#ifndef THREADCONTEXT_H
#define THREADCONTEXT_H

#include <vector>
#include "castable_unique_ptr.h"
#include "../arithmetic/tfhe_core.h"

class TfheThreadContext {
private:
    /** @brief The vector of context objects, in the same order as the factory vector in the TfheGlobalContext */
    std::vector<CastableUniquePointer> contextObjectsArray;
public:
    /** @brief Direct const access to the context objects by pointer */
    const CastableUniquePointer *contextObjects;

private:
    /**
     * @brief Set a context object at position index, resizing the vector if needed
     * This function can only be called by the tfhe global context (friend class)
     *
     * @note SYNC WARNING:
     * This setter can be called by the main thread:
     * Accessing an object in another thread while this function is running may lead to undefined behaviour,
     * especially if it involves resizing the context array vector
     * Proper synchronization must be handled by the caller.
     */
    void setContextObject(uint64_t index, CastableUniquePointer &&object);

public:
    // Non-copyable object
    PREVENT_COPY_ALLOW_STACK(TfheThreadContext);

    friend class TFheGlobalContext;
};

#endif

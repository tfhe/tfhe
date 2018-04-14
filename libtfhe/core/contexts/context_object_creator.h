#ifndef TFHE_CONTEXT_OBJECT_CREATOR_H
#define TFHE_CONTEXT_OBJECT_CREATOR_H


#include "castable_unique_ptr.h"

class ContextObjectCreator {
    /** @brief factory method that creates a context object of any type */
    virtual CastableUniquePointer createObject()=0;
};


#endif //TFHE_CONTEXT_OBJECT_CREATOR_H

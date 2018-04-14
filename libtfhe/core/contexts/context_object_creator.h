#ifndef TFHE_CONTEXT_OBJECT_CREATOR_H
#define TFHE_CONTEXT_OBJECT_CREATOR_H


#include "castable_unique_ptr.h"

class ContextObjectCreator {
    virtual CastableUniquePointer createObject()=0;
};


#endif //TFHE_CONTEXT_OBJECT_CREATOR_H

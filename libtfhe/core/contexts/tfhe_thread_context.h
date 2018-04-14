#ifndef THREADCONTEXT_H
#define THREADCONTEXT_H

#include <vector>
#include "castable_unique_ptr.h"

class TfheThreadContext {
    /** @brief The vector of context objects, in the same order as the factory vector in the TfheGlobalContext */
    std::vector<CastableUniquePointer> contextObjects;
};

#endif

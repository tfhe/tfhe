#ifndef TFHE_TFHEGLOBALCONTEXT_H
#define TFHE_TFHEGLOBALCONTEXT_H


#include <set>
#include <cstdint>
#include <map>
#include <memory>
#include "tfhe_thread_context.h"
#include "context_object_creator.h"

class TfheGlobalContext {
    /** @brief next available thread context id */
    static uint64_t nextThreadId;
    /** @brief map of all available thread contexts, indexed by thread context id */
    std::map<uint64_t, TfheThreadContext> threadContexts;
    /** @brief vector of thread context objects factories (same indexes as the actual objects in the thread contexts) */
    std::vector<std::unique_ptr<ContextObjectCreator>> contextObjectCreators;
    /** @brief map between a (human readable) string identifier for a thread object and its actual index */
    std::map<std::string, int64_t> contextObjectIndex;

    /**
     * @brief creates a new thread context, and populate each of its objects using the factories
     * @return the created thread context
     */
    TfheThreadContext *newThreadContext();

    /**
     * @brief destroys a threadContext and all its objects
     * @param threadContext the threadContext to destroy
     */
    void deleteThreadContext(TfheThreadContext *threadContext);

    /**
     * Sets or replace all context objects in all threadContexts with the one from this factory
     * @param objectCreator the factory to create all the thread objects
     * @return the index in the object vector
     */
    int64_t setContextObject(const std::string &objectId, std::unique_ptr<ContextObjectCreator> &&objectCreator);

    /**
     * finds the index of the context object identified by its title
     * @return the index in the object vector, or -1 if not found
     */
    int64_t findContextObjectIndex(const std::string &objectId);
};


#endif //TFHE_TFHEGLOBALCONTEXT_H

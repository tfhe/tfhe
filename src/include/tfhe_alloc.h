#ifndef TFHE_ALLOC
#define TFHE_ALLOC

#include <new>
#include <utility>
#include <stdlib.h>


/**
 * @brief Class for tagging data structures which should be created
 *  using initializer method instead of constructor.
 * @details Data structures should inherit from this class and implement 
 *  static methods \c init_obj and \c destroy_obj. See example below:
 *  \code{.cpp}
 *    struct Foo: InitializerTag {
 *      static void init_obj(TFHE_TYPE* obj, ...);
 *      static void destroy_obj(TFHE_TYPE* obj);
 *    };
 *  \endcode
 */
struct InitializerTag {};

/**
 * @brief Allocate memory for given object type
 */
template<typename TFHE_TYPE>
TFHE_TYPE* alloc_obj() {
  return (TFHE_TYPE*)malloc(sizeof(TFHE_TYPE));
}

/**
 * @brief Free object memory
 */
template<typename TFHE_TYPE>
void free_obj(TFHE_TYPE* obj) {
  free(obj);
}

/**
 * @brief Initializes an object using class \c TFHE_TYPE initializer method \c init_obj
 */
template<
  typename TFHE_TYPE,
  typename std::enable_if<std::is_base_of<InitializerTag, TFHE_TYPE>::value == true, InitializerTag>::type* = nullptr,
  typename... Args>
void init_obj(TFHE_TYPE* obj, Args&&... args) {
  TFHE_TYPE::init_obj(obj, std::forward<Args>(args)...); 
}

/**
 * @brief Initializes an object using constructor
 * @details Class constructor is called by default for object initialization.
 *  This function can be overloaded in order implement other construction functionalities.
 */
template<
  typename TFHE_TYPE,
  typename std::enable_if<std::is_base_of<InitializerTag, TFHE_TYPE>::value == false, InitializerTag>::type* = nullptr,
  typename... Args>
void init_obj(TFHE_TYPE* obj, Args&&... args) {
  new((void*)obj) TFHE_TYPE(std::forward<Args>(args)...);
}


/**
 * @brief Destroys an object using class \c TFHE_TYPE destroyer method \c destroy_obj
 */
template<
  typename TFHE_TYPE,
  typename std::enable_if<std::is_base_of<InitializerTag, TFHE_TYPE>::value == true, InitializerTag>::type* = nullptr>
void destroy_obj(TFHE_TYPE* obj) {
  TFHE_TYPE::destroy_obj(obj); 
}

/**
 * @brief Destroys an object using destructor
 * @details Class destructor is called by default.
 *  As \c init_obj function, this method can be overloaded to  implement specific destruction.
 */
template<
  typename TFHE_TYPE,
  typename std::enable_if<std::is_base_of<InitializerTag, TFHE_TYPE>::value == false, InitializerTag>::type* = nullptr>
void destroy_obj(TFHE_TYPE* obj) {
  (obj)->~TFHE_TYPE();
}


/**
 * @brief Creates an object
 * @details Memory for object is allocated and it is initialized.
 */
template<typename TFHE_TYPE, typename... Args>
TFHE_TYPE* new_obj(Args&&... args)
{
  TFHE_TYPE* obj = alloc_obj<TFHE_TYPE>();
  init_obj(obj, std::forward<Args>(args)...);
  return obj;
}

/**
 * @brief Deletes an object
 * @details Object is destroyed and object memory is freed.
 */
template<typename TFHE_TYPE>
void del_obj(TFHE_TYPE* obj)
{
  destroy_obj(obj);
  free_obj(obj);
}


/**
 * @brief Allocate memory for an array of objects of given type
 */
template<typename TFHE_TYPE>
TFHE_TYPE* alloc_obj_array(const int nbelts) {
  return (TFHE_TYPE*)malloc(nbelts*sizeof(TFHE_TYPE));
}

/**
 * @brief Free object array memory
 */
template<typename TFHE_TYPE>
void free_obj_array(const int nbelts, TFHE_TYPE* obj) {
  free(obj);
}

/**
 * @brief Initializes an array of objects
 */
template<typename TFHE_TYPE, typename... Args>
void init_obj_array(const int nbelts, TFHE_TYPE* obj, Args&&... args) {
  for (int ii=0; ii<nbelts; ++ii)
    init_obj(obj+ii, std::forward<Args>(args)...);
}

/**
 * @brief Destroys an array of objects
 */
template<typename TFHE_TYPE>
void destroy_obj_array(const int nbelts, TFHE_TYPE* obj) {
  for (int ii=0; ii<nbelts; ++ii)
    destroy_obj(obj+ii);
}

/**
 * @brief Creates an array of objects
 */
template<typename TFHE_TYPE, typename... Args>
TFHE_TYPE* new_obj_array(const int nbelts, Args&&... args)
{
  TFHE_TYPE* obj = alloc_obj_array<TFHE_TYPE>(nbelts);
  init_obj_array(nbelts, obj, std::forward<Args>(args)...);
  return obj;
}

/**
 * @brief Deletes an array of objects
 */
template<typename TFHE_TYPE>
void del_obj_array(const int nbelts, TFHE_TYPE* obj)
{
  destroy_obj_array(nbelts, obj);
  free_obj_array(nbelts, obj);
}

#endif
#ifndef ALGORITHMS_MACRO_HELPERS
#define ALGORITHMS_MACRO_HELPERS

/**
 * @brief Helper macro to get size of an array
 *
 * @param Arr target array
 * @param Type type of an array
 *
 */
#define ARRAY_SIZE(Arr, Type) sizeof(Arr) / sizeof(Type)

/**
 * @brief Helper macro to pass begin and end of the array to the algorithms
 *
 * @param Arr array to get iterators from
 * @param Type type of elements in array
 *
 */
#define BEGIN_END(Arr, Type) \
    Arr, Arr + ARRAY_SIZE(Arr, Type)

/**
 * @brief Helper macro to pass begin, end and size of the array to the algorithms
 *
 * @param Arr array to get iterators from
 * @param Type type of elements in array
 *
 */
#define BEGIN_END_SIZE(Arr, Type) \
    Arr, Arr + ARRAY_SIZE(Arr, Type), sizeof(Type)

/**
 * @brief Helper macro to pass reverse begin, reverse end and size of the
 * array to the algorithms
 *
 * @param Arr array to get iterators from
 * @param Type type of elements in array
 *
 */
#define RBEGIN_REND_SIZE(Arr, Type) \
    Arr + ARRAY_SIZE(Arr, Type) - 1, Arr, -sizeof(Type)

/**
 * @brief GNU Extention helper macro to define anonymous functions
 *
 * @param ReturnType return type of a function
 * @param Body Curly braced body of a function
 *
 * @example LAMBDA(int, () {return 10;});
 *
 */
#define LAMBDA(ReturnType, Body) ({ ReturnType _ Body _; })

#endif  // ALGORITHMS_MACRO_HELPERS

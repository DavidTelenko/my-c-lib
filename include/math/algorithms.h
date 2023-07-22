#ifndef MY_ALGORITHMS_LIB
#define MY_ALGORITHMS_LIB

#include <assert.h>
#include <inttypes.h>
#include <iso646.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <types.h>

/**
 * @brief Helper macro to advance void* iterator to the step of size
 *
 * @param ptr pointer to advance
 * @param size amount of bytes to step into
 *
 */
#define ADVANCE(ptr, size) (ptr = (void *)((char *)ptr + size))

/**
 * @brief Pointer difference in bytes
 *
 * @param lhs left hand side pointer preferably the end of the range
 * @param lhs right hand side pointer preferably the beginning of the range
 *
 */
#define PTR_DIFFERENCE_BYTES(lhs, rhs) ((char *)lhs - (char *)rhs)

/**
 * @brief Pointer difference
 *
 * @param lhs left hand side pointer preferably the end of the range
 * @param lhs right hand side pointer preferably the beginning of the range
 *
 */
#define PTR_DIFFERENCE(lhs, rhs, Type) (((char *)lhs - (char *)rhs) / sizeof(Type))

/**
 * @brief Structure to hold two values.
 * It used as a return type for several algorithms,
 * to make this structure as small as possible it was decided not to store type
 * sizes inside, and rather delegate this responsibility to user
 *
 */
typedef struct Pair {
    void *_1;
    void *_2;
} Pair;

/**
 * @brief Consumer for print functions
 *
 */
typedef void (*PrintFunction)(const void *);

/**
 * @brief Applicator for the left hand side value
 *
 */
typedef void (*BinaryLApplicator)(void *const, const void *const);

/**
 * @brief Operator that accepts value of the sized type and returns value of the same sized type
 *
 */
typedef void *(*UnaryOperator)(const void *const);

/**
 * @brief Predicate for value of certain size
 *
 */
typedef bool (*UnaryPredicate)(const void *const);

/**
 * @brief Binary predicate that accepts two values of the same sized type
 *
 */
typedef bool (*BinaryPredicate)(const void *const, const void *const);

/**
 * @brief Value generator
 *
 */
typedef void *(*Generator)();

/**
 * @brief Random NUmber Generator
 *
 */
typedef int64_t (*RandomGenerator)();

/**
 * @brief Intermediate variable to store predicate received by not_unary_predicate. Used in _unary_not_predicate_wrapper
 *
 */
extern UnaryPredicate _local_unary_predicate;

/**
 * @brief Intermediate variable to store predicate received by not_binary_predicate. Used in _binary_not_predicate_wrapper
 *
 */
extern BinaryPredicate _local_binary_predicate;

/**
 * @brief Internals of a library. Uses global variable _local_unary_predicate to call stored local predicate with not operator
 *
 * @param any parameter of a predicate
 */
bool _unary_not_predicate_wrapper(const void *const any);

/**
 * @brief Internals of a library. Uses global variable _local_binary_predicate to call stored local predicate with not operator
 *
 * @param lhs left hand side parameter of a predicate
 * @param rhs right hand side parameter of a predicate
 */
bool _binary_not_predicate_wrapper(const void *const lhs,
                                   const void *const rhs);

/**
 * @brief Negates given unary predicate. The negation is equivalent to calling the predicate with not operator
 *
 * @param p unary predicate
 * @return UnaryPredicate pointer to negated unary predicate
 */
UnaryPredicate not_unary_predicate(UnaryPredicate p);

/**
 * @brief Negates given binary predicate. The negation is equivalent to calling the predicate with not operator
 *
 * @param p binary predicate
 * @return UnaryPredicate pointer to negated unary predicate
 */
BinaryPredicate not_binary_predicate(BinaryPredicate p);

/**
 * @brief Swaps the contents of two memory blocks.
 *
 * This function swaps the contents of two memory blocks, byte by byte,
 * using a temporary variable. The size of each element in the blocks is
 * determined by `nbytes`.
 *
 * @param lhs A pointer to the first memory block.
 * @param rhs A pointer to the second memory block.
 * @param nbytes The number of bytes to be swapped.
 * @return void
 */
void memswap(void *lhs, void *rhs, size_t nbytes);

/**
 * @brief Applies a binary operator to accumulate values in a range.
 *
 * This function iterates over the elements in the range defined by the pointers `first` and `last`,
 * and applies a binary operator to accumulate the values. The accumulated value is stored in the variable
 * pointed to by `accum`. The accumulation is performed sequentially from `first` to `last`.
 *
 * @param first A pointer to the beginning of the range.
 * @param last A pointer to the end of the range.
 * @param dtype The size of the type stored by the pointers. If a negative value is used, the traversal
 *              will be done in reverse order.
 * @param accum A pointer to the variable where the accumulated value will be stored.
 *              The variable should have the same type as the elements in the range.
 * @param op The binary applicator function that combines the previous accumulated value with each element
 *           in the range. It should take two arguments of type `void*` - the accumulated value and the current
 *           element - and modify the accumulated value accordingly. The function is expected to have side effects,
 *           as it modifies the state of the `accum` variable.
 */
void reduce(const void *first,
            const void *const last,
            int64_t dtype,
            void *const accum,
            BinaryLApplicator op);

/**
 * @brief Finds an element in a range that satisfies a given predicate.
 *
 * This function searches for an element in the range defined by the pointers `first` and `last`,
 * and returns a pointer to the found element. The search is performed sequentially from `first`
 * to `last`. If no element is found, `last` is returned.
 *
 * @param first A pointer to the beginning of the range.
 * @param last A pointer to the end of the range.
 * @param dtype The size of the type stored by the pointers. If a negative value is used, the traversal
 *              will be done in reverse order.
 * @param p The unary predicate function that determines if an element satisfies the desired condition.
 *          It should take a single argument of type `const void*` and return a boolean value.
 *          The function should return `true` if the element satisfies the condition, and `false` otherwise.
 *
 * @return A pointer to the found element, or `last` if no element is found.
 */
const void *find(const void *first,
                 const void *const last,
                 int64_t dtype,
                 UnaryPredicate p);

/**
 * @brief Filters elements in a range based on a unary predicate.
 *
 * This function iterates over the elements in the range defined by the pointers `first` and `last`,
 * and filters the elements based on a unary predicate `p`. The filtered elements are moved to the beginning
 * of the range pointed to by `first`, and the pointer `first` is updated to point to the new end of the filtered range.
 *
 * @param first A pointer to the beginning of the range. After filtering, this pointer will be updated to point to the new end of the filtered range.
 * @param last A pointer to the end of the range.
 * @param dtype The size of the type stored by the pointers.
 * @param p The unary predicate function that determines whether an element should be included in the filtered range or not.
 *          It should take one argument of type `void*` - the current element - and return a `bool` value indicating whether the element passes the filter or not.
 *
 * @return A pointer to the new end of the filtered range, which is also the updated value of the `first` parameter.
 */
void *filter(void *first,
             const void *const last,
             size_t dtype,
             UnaryPredicate p);

/**
 * @brief Checks if all elements in a range satisfy a unary predicate.
 *
 * This function iterates over the elements in the range defined by the pointers `first` and `last`,
 * and checks if all elements satisfy a unary predicate `p`. The unary predicate determines whether an element
 * satisfies a condition or not. If all elements in the range pass the predicate, the function returns `true`.
 * Otherwise, it returns `false`.
 *
 * @param first A pointer to the beginning of the range.
 * @param last A pointer to the end of the range.
 * @param dtype The size of the type stored by the pointers.
 * @param p The unary predicate function that determines whether an element satisfies a condition or not.
 *          It should take one argument of type `void*` - the current element - and return a `bool` value indicating whether the element satisfies the condition or not.
 *
 * @return `true` if all elements in the range satisfy the predicate, `false` otherwise.
 */
bool all(const void *first,
         const void *const last,
         size_t dtype,
         UnaryPredicate p);

/**
 * @brief Checks if at least one element in a range satisfy a unary predicate.
 *
 * This function iterates over the elements in the range defined by the pointers `first` and `last`,
 * and checks if at least one element satisfy a unary predicate `p`. The unary predicate determines whether an element
 * satisfies a condition or not. If at least one element in the range pass the predicate, the function returns `true`.
 * Otherwise, it returns `false`.
 *
 * @param first A pointer to the beginning of the range.
 * @param last A pointer to the end of the range.
 * @param dtype The size of the type stored by the pointers.
 * @param p The unary predicate function that determines whether an element satisfies a condition or not.
 *          It should take one argument of type `void*` - the current element - and return a `bool` value indicating whether the element satisfies the condition or not.
 *
 * @return `true` if at least one element in the range satisfy the predicate, `false` otherwise.
 */
bool any(const void *first,
         const void *const last,
         size_t dtype,
         UnaryPredicate p);

/**
 * @brief Counts the number of elements in a range that satisfy a given predicate.
 *
 * This function iterates over the range defined by the `first` and `last` pointers, and applies the unary predicate `p` to each element. It counts the number of elements for which the predicate returns true, and returns this count.
 *
 * @param first A pointer to the beginning of the range.
 * @param last A pointer to the end of the range (one-past-the-end).
 * @param dtype The size of each element in the range, in bytes.
 * @param p The unary predicate to be applied to each element.
 * @return The number of elements in the range that satisfy the predicate.
 */
size_t count(const void *first,
             const void *const last,
             size_t dtype,
             UnaryPredicate p);

/**
 * @brief Finds the first mismatched pair of elements in two ranges.
 *
 * This function compares each element in the range [first1, last1) with the corresponding
 * element in the range [first2, last2) until a mismatch is found or either of the ranges
 * ends. The comparison is done using the provided binary predicate p.
 *
 * @param first1 A pointer to the beginning of the first range
 * @param last1 A pointer to the end of the first range (exclusive)
 * @param dtype1 The size (in bytes) of each element in the first range
 * @param first2 A pointer to the beginning of the second range
 * @param last2 A pointer to the end of the second range (exclusive)
 * @param dtype2 The size (in bytes) of each element in the second range
 * @param p The binary predicate used for comparison
 * @return A Pair struct containing the first mismatched elements as void pointers
 */
Pair mismatch(const void *first1,
              const void *const last1,
              int64_t dtype1,
              const void *first2,
              const void *const last2,
              int64_t dtype2,
              BinaryPredicate p);

/**
 * @brief Finds the first occurrence of a pair of adjacent elements in a range that satisfy a specified condition.
 *
 * @param first Pointer to the beginning of the range.
 * @param last Pointer to the end of the range (one past the last element).
 * @param dtype Size of each element in the range.
 * @param p Binary predicate function used to compare elements.
 * @return const void* Pointer to the first element of the pair satisfying the condition. Returns last if no such element is found.
 */
const void *adjacent_find(const void *first,
                          const void *last,
                          size_t dtype,
                          BinaryPredicate p);

/**
 * @brief Searches for a sequence within another sequence using a binary predicate.
 *
 * This function searches for a sequence defined by [first2, last2) within another sequence defined by [first1, last1)
 * using the provided binary predicate p. The function returns a pointer to the first element in [first1, last1) that
 * matches the sequence [first2, last2). If the sequence is not found, it returns the pointer to last1.
 *
 * @param first1 A pointer to the beginning of the first sequence.
 * @param last1 A pointer to the end of the first sequence.
 * @param dtype1 The size of the elements in the first sequence.
 * @param first2 A pointer to the beginning of the second sequence.
 * @param last2 A pointer to the end of the second sequence.
 * @param dtype2 The size of the elements in the second sequence.
 * @param p A binary predicate used to compare elements in the two sequences.
 * @return const void* A pointer to the first element in the first sequence that matches the second sequence,
 *         or a pointer to last1 if the second sequence is not found.
 */
const void *search(const void *first1,
                   const void *last1,
                   int64_t dtype1,
                   const void *first2,
                   const void *last2,
                   int64_t dtype2,
                   BinaryPredicate p);

/**
 * @brief Reverses the elements in a range.
 *
 * This function takes a range of elements specified by `first` and `last`, and reverses the order of the elements in that range. It is assumed that the range contains elements of the same data type, which is specified by the `dtype` parameter.
 *
 * @param first A pointer to the first element in the range to be reversed.
 * @param last A pointer to the last element (exclusive) in the range to be reversed.
 * @param dtype The size of each element in bytes.
 */
void reverse(void *first,
             void *last,
             int64_t dtype);

/**
 * @brief Generate values using a generator function and store them in a range.
 *
 * This function generates values using the provided generator function and stores them in the specified range. The generator function should return a value of the same data type as the elements in the range. The generated values are copied into the range using `memcpy`.
 *
 * @param first Pointer to the first element in the range.
 * @param last Pointer to one past the last element in the range.
 * @param dtype Size of each element in bytes.
 * @param gen Pointer to the generator function that generates values.
 */
void generate(void *first,
              const void *const last,
              int64_t dtype,
              Generator gen);

/**
 * @brief Fill a range of memory with a specified value.
 *
 * This function copies the `value` to each element in the range starting from `first` and ending at `last`. The size of each element is determined by the `dtype` parameter.
 *
 * @param first Pointer to the beginning of the range to be filled.
 * @param last Pointer one past the end of the range to be filled.
 * @param dtype Size of each element in the range, in bytes.
 * @param value Pointer to the value to be copied to each element.
 *
 * @note Both `first` and `last` should point to memory blocks large enough to accommodate elements of size `dtype`.
 * @note The `dtype` parameter should be greater than or equal to zero.
 */
void fill(void *first,
          const void *const last,
          int64_t dtype,
          const void *const value);

/**
 * @brief Transforms a range of values from a source container to a destination container using a unary operator.
 *
 * This function applies the provided unary operator to each element in the source range and stores the result in the destination range.
 * The source and destination containers can have different types, specified by the source_dtype and dest_dtype respectively.
 * Both containers must be able to hold the number of elements specified by the ranges [source_first, source_last) and [dest_first, dest_last).
 *
 * @param source_first Pointer to the beginning of the source range.
 * @param source_last Pointer to the end of the source range (exclusive).
 * @param source_dtype The size (in bytes) of each element in the source range.
 * @param dest_first Pointer to the beginning of the destination range.
 * @param dest_last Pointer to the end of the destination range (exclusive).
 * @param dest_dtype The size (in bytes) of each element in the destination range.
 * @param op UnaryOperator that takes an element from the source range as input and returns the transformed value.
 */
void transform(const void *source_first,
               const void *const source_last,
               int64_t source_dtype,
               void *dest_first,
               const void *const dest_last,
               int64_t dest_dtype,
               UnaryOperator op);

/**
 * @brief Rotates a range of elements in an array.
 *
 * This function rotates the elements in the range [first, last) in the array.
 * The element pointed to by 'around' becomes the first element of the new range.
 * The elements before 'around' are moved to the end of the range, and the
 * elements after 'around' are moved to the beginning of the range.
 *
 * @param first A pointer to the beginning of the range.
 * @param around A pointer to the element that will become the first element
 *               of the new range.
 * @param last A pointer to one past the last element of the range.
 * @param dtype The size of each element in the array in bytes.
 * @return void* A pointer to the first element of the rotated range.
 */
void *rotate(void *first,
             void *around,
             void *last,
             int64_t dtype);

/**
 * @brief Removes consecutive duplicate elements in a range.
 *
 * This function removes consecutive duplicate elements in the range defined by `first`
 * and `last`. The comparison between elements is performed using the binary predicate
 * `p`. The size of each element is determined by `dtype`.
 *
 * @param first A pointer to the first element in the range.
 * @param last A pointer to one past the last element in the range.
 * @param dtype The size of the type stored by the pointers. If a negative value is used, the traversal
 *              will be done in reverse order.
 * @param p The binary predicate used to compare elements.
 * @return A pointer to the new end of the range, after removing duplicates.
 *         If no duplicates are found, it returns `last`.
 */
void *unique(void *first,
             void *last,
             int64_t dtype,
             BinaryPredicate p);

/**
 * @brief Shuffles a range of elements.
 *
 * This function shuffles the elements in the range defined by `first` and `last`,
 * using a random number generator `rnd`. The size of each element is determined
 * by `dtype`.
 *
 * @param first A pointer to the first element in the range.
 * @param last A pointer to one past the last element in the range.
 * @param dtype The size of the type stored by the pointers. If a negative value is used, the traversal
 *              will be done in reverse order.
 * @param rnd The random number generator function.
 * @return void
 */
void shuffle(void *first,
             const void *const last,
             int64_t dtype,
             RandomGenerator rnd);

#endif  // MY_ALGORITMS_LIBRARY
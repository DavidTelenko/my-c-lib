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
 * @brief Not predicate routine
 *
 */
extern UnaryPredicate _local_unary_predicate;
extern BinaryPredicate _local_binary_predicate;

bool _unary_not_predicate_wrapper(const void *const any);
bool _binary_not_predicate_wrapper(const void *const lhs,
                                   const void *const rhs);

UnaryPredicate not_unary_predicate(UnaryPredicate p);
BinaryPredicate not_binary_predicate(BinaryPredicate p);

/**
 * @brief Extention to the standard library, gives opportunity to swap two same sized memory cells
 *
 * @param lhs left hand side value
 * @param rhs right hand side value
 * @param nbytes amount of bytes that stored in the cell
 */
void memswap(void *lhs, void *rhs, size_t nbytes);

/**
 * @brief Applies binary applicator for value stored in accum and each value
 * of range [first, last), stores result in accum.
 *
 * @param first begin of range
 * @param last end of range
 * @param typeSize size of type stored by pointers
 * @param accum accumulator value that will store the result value
 * @param op binary applicator that stores result in left hand side parameter
 */
void reduce(const void *first,
            const void *const last,
            int64_t typeSize,
            void *const accum,
            BinaryLApplicator op);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return const void*
 */
const void *find(const void *first,
                 const void *const last,
                 int64_t typeSize,
                 UnaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return void*
 */
void *filter(void *first,
             const void *const last,
             size_t typeSize,
             UnaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return true
 * @return false
 */
bool all(const void *first,
         const void *const last,
         size_t typeSize,
         UnaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return true
 * @return false
 */
bool any(const void *first,
         const void *const last,
         size_t typeSize,
         UnaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return size_t
 */
size_t count(const void *first,
             const void *const last,
             size_t typeSize,
             UnaryPredicate p);

/**
 * @brief
 *
 * @param first1
 * @param last1
 * @param typeSize1
 * @param first2
 * @param last2
 * @param typeSize2
 * @param p
 * @return Pair
 */
Pair mismatch(const void *first1,
              const void *const last1,
              int64_t typeSize1,
              const void *first2,
              const void *const last2,
              int64_t typeSize2,
              BinaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return const void*
 */
const void *adjacent_find(const void *first,
                          const void *last,
                          size_t typeSize,
                          BinaryPredicate p);

/**
 * @brief
 *
 * @param first1
 * @param last1
 * @param typeSize1
 * @param first2
 * @param last2
 * @param typeSize2
 * @param p
 * @return const void*
 */
const void *search(const void *first1,
                   const void *last1,
                   int64_t typeSize1,
                   const void *first2,
                   const void *last2,
                   int64_t typeSize2,
                   BinaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 */
void reverse(void *first,
             void *last,
             int64_t typeSize);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param gen
 */
void generate(void *first,
              const void *const last,
              int64_t typeSize,
              Generator gen);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param value
 */
void fill(void *first,
          const void *const last,
          int64_t typeSize,
          const void *const value);

/**
 * @brief
 *
 * @param sourceFirst
 * @param sourceLast
 * @param sourceTypeSize
 * @param destFirst
 * @param destLast
 * @param destTypeSize
 * @param op
 */
void transform(const void *sourceFirst,
               const void *const sourceLast,
               int64_t sourceTypeSize,
               void *destFirst,
               const void *const destLast,
               int64_t destTypeSize,
               UnaryOperator op);

/**
 * @brief
 *
 * @param first
 * @param around
 * @param last
 * @param typeSize
 * @return void*
 */
void *rotate(void *first,
             void *around,
             void *last,
             int64_t typeSize);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param p
 * @return void*
 */
void *unique(void *first,
             void *last,
             int64_t typeSize,
             BinaryPredicate p);

/**
 * @brief
 *
 * @param first
 * @param last
 * @param typeSize
 * @param rnd
 */
void shuffle(void *first,
             const void *const last,
             int64_t typeSize,
             RandomGenerator rnd);

#endif  // MY_ALGORITMS_LIBRARY
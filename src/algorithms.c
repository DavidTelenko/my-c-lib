#include <algorithms.h>

UnaryPredicate _local_unary_predicate = 0;
BinaryPredicate _local_binary_predicate = 0;

bool _unary_not_predicate_wrapper(const void *const any) {
    assert(_local_unary_predicate);
    return not _local_unary_predicate(any);
}

bool _binary_not_predicate_wrapper(const void *const lhs, const void *const rhs) {
    assert(_local_binary_predicate);
    return not _local_binary_predicate(lhs, rhs);
}

UnaryPredicate not_unary_predicate(UnaryPredicate p) {
    _local_unary_predicate = p;
    return &_unary_not_predicate_wrapper;
}

BinaryPredicate not_binary_predicate(BinaryPredicate p) {
    _local_binary_predicate = p;
    return &_binary_not_predicate_wrapper;
}

void memswap(void *lhs, void *rhs, size_t nbytes) {
    for (char *_lhs = (char *)lhs, *_rhs = (char *)rhs; nbytes--; _lhs++, _rhs++) {
        char tmp = *_lhs;
        *_lhs = *_rhs;
        *_rhs = tmp;
    }
}

void reduce(const void *first, const void *const last, int64_t typeSize,
            void *const accum, BinaryLApplicator op) {
    for (; first != last; ADVANCE(first, typeSize)) {
        op(accum, first);
    }
}

const void *find(const void *first, const void *const last, int64_t typeSize,
                 UnaryPredicate p) {
    for (; first != last; ADVANCE(first, typeSize)) {
        if (p(first)) {
            break;
        }
    }
    return first;
}

void *filter(void *first, const void *const last, size_t typeSize,
             UnaryPredicate p) {
    first = (void *)find(first, last, typeSize, p);
    if (first == last) {
        return first;
    }
    for (void *i = first; ADVANCE(i, typeSize) != last;) {
        if (not p(i)) {
            memmove(first, i, typeSize);
            ADVANCE(first, typeSize);
        }
    }
    return first;
}

bool all(const void *first, const void *const last, size_t typeSize,
         UnaryPredicate p) {
    return find(first, last, typeSize, not_unary_predicate(p)) == last;
}

bool any(const void *first, const void *const last, size_t typeSize,
         UnaryPredicate p) {
    return find(first, last, typeSize, p) != last;
}

size_t count(const void *first, const void *const last, size_t typeSize,
             UnaryPredicate p) {
    size_t accum = 0;
    for (; first != last; ADVANCE(first, typeSize)) {
        accum += p(first);
    }
    return accum;
}

Pair mismatch(const void *first1, const void *const last1, int64_t typeSize1,
              const void *first2, const void *const last2, int64_t typeSize2,
              BinaryPredicate p) {
    while (first1 != last1 and first2 != last2 and
           p(first1, first2)) {
        ADVANCE(first1, typeSize1);
        ADVANCE(first2, typeSize2);
    }
    return (Pair){(void *)first1, (void *)first2};
}

const void *adjacent_find(const void *first, const void *last, size_t typeSize,
                          BinaryPredicate p) {
    if (first == last) {
        return first;
    }

    const void *next = first;
    ADVANCE(next, typeSize);

    for (; next != last; ADVANCE(next, typeSize), ADVANCE(first, typeSize)) {
        if (p(first, next)) {
            return first;
        }
    }

    return last;
}

const void *search(const void *first1, const void *last1, int64_t typeSize1,
                   const void *first2, const void *last2, int64_t typeSize2,
                   BinaryPredicate p) {
    for (;;) {
        for (const void *it1 = first1, *it2 = first2;;) {
            if (it2 == last2) {
                return first1;
            }
            if (it1 == last1) {
                return last1;
            }
            if (not p(it1, it2)) {
                break;
            }
            ADVANCE(it1, typeSize1);
            ADVANCE(it2, typeSize2);
        }
        ADVANCE(first1, typeSize1);
    }
}

void reverse(void *first, void *last, int64_t typeSize) {
    if (first == last) {
        return;
    }

    for (ADVANCE(last, -typeSize); first < last;) {
        memswap(first, last, typeSize);
        ADVANCE(first, typeSize);
        ADVANCE(last, -typeSize);
    }
}

void generate(void *first, const void *const last, int64_t typeSize,
              Generator gen) {
    for (; first != last; ADVANCE(first, typeSize)) {
        memcpy(first, gen(), typeSize);
    }
}

void fill(void *first, const void *const last, int64_t typeSize,
          const void *const value) {
    for (; first != last; ADVANCE(first, typeSize)) {
        memcpy(first, value, typeSize);
    }
}

void transform(const void *sourceFirst, const void *const sourceLast, int64_t sourceTypeSize,
               void *destFirst, const void *const destLast, int64_t destTypeSize,
               UnaryOperator op) {
    while (sourceFirst != sourceLast and destFirst != destLast) {
        memcpy(destFirst, op(sourceFirst), destTypeSize);
        ADVANCE(sourceFirst, sourceTypeSize);
        ADVANCE(destFirst, destTypeSize);
    }
}

void *rotate(void *first, void *around, void *last, int64_t typeSize) {
    if (first == around) {
        return last;
    }

    if (around == last) {
        return first;
    }

    void *read = around;
    void *write = first;
    void *nextRead = first;

    while (read != last) {
        if (write == nextRead) {
            nextRead = read;
        }
        memswap(write, read, typeSize);
        ADVANCE(write, typeSize);
        ADVANCE(read, typeSize);
    }

    rotate(write, nextRead, last, typeSize);
    return write;
}

void *unique(void *first, void *last, int64_t typeSize, BinaryPredicate p) {
    if (first == last) {
        return last;
    }

    void *result = first;
    while (ADVANCE(first, typeSize) != last) {
        if (not p(result, first) and ADVANCE(result, typeSize) != first) {
            memmove(result, first, typeSize);
        }
    }
    return ADVANCE(result, typeSize);
}

void shuffle(void *first, const void *last, int64_t typeSize,
             RandomGenerator rnd) {
    const void *const initial = first;
    const size_t size = PTR_DIFFERENCE_BYTES(first, last) / typeSize;

    for (; first != last; ADVANCE(first, typeSize)) {
        memswap(first, (char *)initial + (rnd() % size) * typeSize, typeSize);
    }
}

// void shift(void *first, const void *last, int64_t typeSize, int64_t n) {
//     if (n < 0) {
//         void *mid = first;
//         ADVANCE(mid, typeSize * -n);
//         memmove(first, mid, PTR_DIFFERENCE(last, mid));
//         return;
//     }
// }

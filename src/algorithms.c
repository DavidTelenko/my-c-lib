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

void reduce(const void *first, const void *const last, int64_t dtype,
            void *const accum, BinaryLApplicator op) {
    for (; first != last; ADVANCE(first, dtype)) {
        op(accum, first);
    }
}

const void *find(const void *first, const void *const last, int64_t dtype,
                 UnaryPredicate p) {
    for (; first != last; ADVANCE(first, dtype)) {
        if (p(first)) {
            break;
        }
    }
    return first;
}

void *filter(void *first, const void *const last, size_t dtype,
             UnaryPredicate p) {
    first = (void *)find(first, last, dtype, p);
    if (first == last) {
        return first;
    }
    for (void *i = first; ADVANCE(i, dtype) != last;) {
        if (not p(i)) {
            memmove(first, i, dtype);
            ADVANCE(first, dtype);
        }
    }
    return first;
}

bool all(const void *first, const void *const last, size_t dtype,
         UnaryPredicate p) {
    return find(first, last, dtype, not_unary_predicate(p)) == last;
}

bool any(const void *first, const void *const last, size_t dtype,
         UnaryPredicate p) {
    return find(first, last, dtype, p) != last;
}

size_t count(const void *first, const void *const last, size_t dtype,
             UnaryPredicate p) {
    size_t accum = 0;
    for (; first != last; ADVANCE(first, dtype)) {
        accum += p(first);
    }
    return accum;
}

Pair mismatch(const void *first1, const void *const last1, int64_t dtype1,
              const void *first2, const void *const last2, int64_t dtype2,
              BinaryPredicate p) {
    while (first1 != last1 and first2 != last2 and
           p(first1, first2)) {
        ADVANCE(first1, dtype1);
        ADVANCE(first2, dtype2);
    }
    return (Pair){(void *)first1, (void *)first2};
}

const void *adjacent_find(const void *first, const void *last, size_t dtype,
                          BinaryPredicate p) {
    if (first == last) {
        return first;
    }

    const void *next = first;
    ADVANCE(next, dtype);

    for (; next != last; ADVANCE(next, dtype), ADVANCE(first, dtype)) {
        if (p(first, next)) {
            return first;
        }
    }

    return last;
}

const void *search(const void *first1, const void *last1, int64_t dtype1,
                   const void *first2, const void *last2, int64_t dtype2,
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
            ADVANCE(it1, dtype1);
            ADVANCE(it2, dtype2);
        }
        ADVANCE(first1, dtype1);
    }
}

void reverse(void *first, void *last, int64_t dtype) {
    if (first == last) {
        return;
    }

    for (ADVANCE(last, -dtype); first < last;) {
        memswap(first, last, dtype);
        ADVANCE(first, dtype);
        ADVANCE(last, -dtype);
    }
}

void generate(void *first, const void *const last, int64_t dtype,
              Generator gen) {
    for (; first != last; ADVANCE(first, dtype)) {
        memcpy(first, gen(), dtype);
    }
}

void fill(void *first, const void *const last, int64_t dtype,
          const void *const value) {
    for (; first != last; ADVANCE(first, dtype)) {
        memcpy(first, value, dtype);
    }
}

void transform(const void *source_first, const void *const source_last, int64_t source_dtype,
               void *dest_first, const void *const dest_last, int64_t dest_dtype,
               UnaryOperator op) {
    while (source_first != source_last and dest_first != dest_last) {
        memcpy(dest_first, op(source_first), dest_dtype);
        ADVANCE(source_first, source_dtype);
        ADVANCE(dest_first, dest_dtype);
    }
}

void *rotate(void *first, void *around, void *last, int64_t dtype) {
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
        memswap(write, read, dtype);
        ADVANCE(write, dtype);
        ADVANCE(read, dtype);
    }

    rotate(write, nextRead, last, dtype);
    return write;
}

void *unique(void *first, void *last, int64_t dtype, BinaryPredicate p) {
    if (first == last) {
        return last;
    }

    void *result = first;
    while (ADVANCE(first, dtype) != last) {
        if (not p(result, first) and ADVANCE(result, dtype) != first) {
            memmove(result, first, dtype);
        }
    }
    return ADVANCE(result, dtype);
}

void shuffle(void *first, const void *last, int64_t dtype,
             RandomGenerator rnd) {
    const void *const initial = first;
    const size_t size = PTR_DIFFERENCE_BYTES(first, last) / dtype;

    for (; first != last; ADVANCE(first, dtype)) {
        memswap(first, (char *)initial + (rnd() % size) * dtype, dtype);
    }
}

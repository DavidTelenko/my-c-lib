#ifndef MY_MATRIX2
#define MY_MATRIX2

#include <assert.h>
#include <iso646.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//
#include <matrix_segment_view.h>

typedef struct {
    size_t rows;
    size_t cols;
    size_t dtype;
    void* const data;
} matrix2;

typedef void (*Apply)(void* const, void const* const, void const* const);

matrix_segment_view m2_get_row(matrix2 const* const m, size_t const index);

matrix_segment_view m2_get_column(matrix2 const* const m, size_t const index);

void* const m2_get_from_row(matrix_segment_view const* const row,
                            size_t const index);

void* const m2_get_from_column(matrix_segment_view const* const column,
                               size_t const index);

void* const m2_get_from_matrix(matrix2 const* const m, size_t const column,
                               size_t const row);

void m2_set_row(matrix2* const m, size_t const index, void* const data);

void m2_set_column(matrix2* const m, size_t const index, void* const data);

void m2_set_at(matrix2* const m, size_t const i, size_t const j,
               void* const data);

void m2_set_all(matrix2* const m, void* const data);

void m2_mult(matrix2* const dest, matrix2 const* const lhs,
             matrix2 const* const rhs, Apply perf);

void m2_apply(matrix2* const dest, matrix2 const* const lhs,
              matrix2 const* const rhs, Apply perf);

int m2_compare(matrix2 const* const lhs, matrix2 const* const rhs);

#endif  // MY_MATRIX2

#ifndef MATRIX2
#define MATRIX2

#include <assert.h>
#include <iso646.h>
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

typedef void (*Apply)(void* const,
                      void const* const,
                      void const* const);

matrix_segment_view m2_get_row(matrix2 const* const m, size_t index);
matrix_segment_view m2_get_column(matrix2 const* const m, size_t index);
void* const m2_get_from_row(matrix_segment_view const* const row, size_t index);
void* const m2_get_from_column(matrix_segment_view const* const column, size_t index);
void* const m2_get_from_matrix(matrix2 const* const m, size_t column, size_t row);

void m2_set_row(matrix2* const m, size_t index, void* data);
void m2_set_column(matrix2* const m, size_t index, void* data);
void m2_set_at(matrix2* const m, size_t i, size_t j, void* data);

void m2_mult(matrix2* const dest,
             matrix2 const* const lhs,
             matrix2 const* const rhs,
             Apply perf);

void m2_apply(matrix2* const dest,
              matrix2 const* const lhs,
              matrix2 const* const rhs,
              Apply perf);

#endif // MATRIX2
#ifndef INCLUDED_ALPHABET_H
#define INCLUDED_ALPHABET_H

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

matrix_segment_view m2_get_row(matrix2* const m, size_t index);
matrix_segment_view m2_get_column(matrix2* const m, size_t index);

void m2_set_row(matrix2* m, size_t index, void* data);
void m2_set_column(matrix2* m, size_t index, void* data);

void* const m2_get_from_row(matrix_segment_view* row, size_t index);
void* const m2_get_from_column(matrix_segment_view* column, size_t index);

void* const m2_get_from_matrix(matrix2* m, size_t column, size_t row);

#endif
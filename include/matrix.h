#ifndef INCLUDED_ALPHABET_H
#define INCLUDED_ALPHABET_H

#include <assert.h>
#include <iso646.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t rows;
    size_t cols;
    size_t dtype;
    void* const data;
} matrix2;

typedef struct {
    matrix2 const* const ref;
    const size_t begin;
    const size_t end;
} row_view;

typedef struct {
    matrix2 const* const ref;
    const size_t begin;
    const size_t end;
} column_view;

#define CREATE_MATRIX(data_type, num_rows, num_cols, buffer)            \
    {                                                                   \
        .rows = num_rows, .cols = num_cols, .dtype = sizeof(data_type), \
        .data = (void* const)buffer,                                    \
    }

#define PRINT_MATRIX(m, format, dtype)             \
    {                                              \
        for (size_t i = 0; i < m.rows; ++i) {      \
            for (size_t j = 0; j < m.cols; ++j) {  \
                row_view row = get_row(&m, i);     \
                void* val = get_from_row(&row, j); \
                printf(format, *((dtype*)val));    \
            }                                      \
            printf("\n");                          \
        }                                          \
    }

row_view get_row(matrix2 const* const m, size_t index) {
    assert(m and m->rows and m->cols and index < m->rows);

    return (row_view){
        .ref = m,
        .begin = index * m->cols,
        .end = index * m->cols + m->rows,
    };
}

column_view get_column(matrix2 const* const m, size_t index) {
    assert(m and m->rows and m->cols and index < m->cols);

    return (column_view){
        .ref = m,
        .begin = index,
        .end = index + m->cols * (m->rows - 1),
    };
}

void set_row(matrix2* m, size_t index, void* data) {
    memcpy((void*)((char*)m->data + index * m->cols * m->dtype), data,
           m->cols * m->dtype);
}

void set_column(matrix2* m, size_t index, void* data) {
    for (size_t i = index, j = 0; i < m->rows * m->cols; i += m->cols, ++j) {
        memcpy((void*)((char*)m->data + i * m->dtype),
               (void*)((char*)data + j * m->dtype), m->dtype);
    }
}

void* const get_from_row(row_view* row, size_t index) {
    return (void*)((char*)row->ref->data +
                   (row->begin + index) * row->ref->dtype);
}

void* const get_from_column(column_view* column, size_t index) {
    return (void*)((char*)column->ref->data +
                   (column->begin + index * column->ref->cols) *
                       column->ref->dtype);
}

void* const get_from_matrix(matrix2* m, size_t column, size_t row) {
    return (void*)((char*)m->data + (column + row * m->cols) * m->dtype);
}

#endif
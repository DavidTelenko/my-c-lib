#include <matrix2.h>

matrix_segment_view m2_get_row(matrix2 const* const m, size_t index) {
    assert(m and m->rows and m->cols and index < m->rows);

    return (matrix_segment_view){
        .ref = (void* const)m,
        .begin = index * m->cols,
        .end = index * m->cols + m->rows,
    };
}

matrix_segment_view m2_get_column(matrix2 const* const m, size_t index) {
    assert(m and m->rows and m->cols and index < m->cols);

    return (matrix_segment_view){
        .ref = (void* const)m,
        .begin = index,
        .end = index + m->cols * (m->rows - 1),
    };
}

void* const m2_get_from_row(matrix_segment_view const* const row, size_t index) {
    matrix2* const m = (matrix2* const)row->ref;
    return (void*)((char*)(m->data + (row->begin + index) * m->dtype));
}

void* const m2_get_from_column(matrix_segment_view const* const column, size_t index) {
    matrix2* const m = (matrix2* const)column->ref;
    return (void*)((char*)m->data + (column->begin + index * m->cols) * m->dtype);
}

void* const m2_get_from_matrix(matrix2 const* const m, size_t column, size_t row) {
    return (void*)((char*)m->data + (column + row * m->cols) * m->dtype);
}

void m2_set_row(matrix2* const m, size_t index, void* data) {
    memcpy((void*)((char*)m->data + index * m->cols * m->dtype), data,
           m->cols * m->dtype);
}

void m2_set_column(matrix2* const m, size_t index, void* data) {
    for (size_t i = index, j = 0; i < m->rows * m->cols; i += m->cols, ++j) {
        memcpy((void*)((char*)m->data + i * m->dtype),
               (void*)((char*)data + j * m->dtype), m->dtype);
    }
}

void m2_set_at(matrix2* const m, size_t x, size_t y, void* data) {
    memcpy(m->data + (x + m->cols * y) * m->dtype, data, m->dtype);
}

void m2_mult(matrix2* const dest,
             matrix2 const* const lhs,
             matrix2 const* const rhs,
             Apply perf) {
    assert(dest->rows == lhs->rows and
           dest->cols == rhs->cols and
           dest->dtype == lhs->dtype and
           dest->dtype == rhs->dtype);

    memset(dest->data, 0, dest->rows * dest->cols * dest->dtype);

    for (size_t i = 0; i < dest->rows; ++i) {
        for (size_t j = 0; j < dest->cols; ++j) {
            void* const dest_val =
                (void* const)((char*)dest->data +
                              (i * dest->cols + j) * dest->dtype);

            for (size_t k = 0; k < lhs->cols; ++k) {
                perf(dest_val,
                     (void const* const)((char*)lhs->data +
                                         (i * lhs->cols + k) * dest->dtype),
                     (void const* const)((char*)rhs->data +
                                         (k * rhs->cols + j) * dest->dtype));
            }
        }
    }
}

void m2_apply(matrix2* const dest,
              matrix2 const* const lhs,
              matrix2 const* const rhs,
              Apply apply) {
    assert(dest->rows == lhs->rows and
           dest->cols == lhs->cols and
           dest->rows == rhs->rows and
           dest->cols == rhs->cols and
           dest->dtype == lhs->dtype and
           dest->dtype == rhs->dtype);

    for (size_t i = 0; i < dest->cols * dest->rows; ++i) {
        apply((void* const)((char*)dest->data + i * dest->dtype),
              (void const* const)((char*)lhs->data + i * dest->dtype),
              (void const* const)((char*)rhs->data + i * dest->dtype));
    }
}

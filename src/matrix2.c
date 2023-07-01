#include <matrix2.h>

matrix_segment_view m2_get_row(matrix2* const m, size_t index) {
    assert(m and m->rows and m->cols and index < m->rows);

    return (matrix_segment_view){
        .ref = (void* const)m,
        .begin = index * m->cols,
        .end = index * m->cols + m->rows,
    };
}

matrix_segment_view m2_get_column(matrix2* const m, size_t index) {
    assert(m and m->rows and m->cols and index < m->cols);

    return (matrix_segment_view){
        .ref = (void* const)m,
        .begin = index,
        .end = index + m->cols * (m->rows - 1),
    };
}

void m2_set_row(matrix2* m, size_t index, void* data) {
    memcpy((void*)((char*)m->data + index * m->cols * m->dtype), data,
           m->cols * m->dtype);
}

void m2_set_column(matrix2* m, size_t index, void* data) {
    for (size_t i = index, j = 0; i < m->rows * m->cols; i += m->cols, ++j) {
        memcpy((void*)((char*)m->data + i * m->dtype),
               (void*)((char*)data + j * m->dtype), m->dtype);
    }
}

void* const m2_get_from_row(matrix_segment_view* row, size_t index) {
    matrix2* const m = (matrix2* const)row->ref;
    return (void*)((char*)(m->data + (row->begin + index) * m->dtype));
}

void* const m2_get_from_column(matrix_segment_view* column, size_t index) {
    matrix2* const m = (matrix2* const)column->ref;
    return (void*)((char*)m->data + (column->begin + index * m->cols) * m->dtype);
}

void* const m2_get_from_matrix(matrix2* m, size_t column, size_t row) {
    return (void*)((char*)m->data + (column + row * m->cols) * m->dtype);
}
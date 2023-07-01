#ifndef MATRIX2_MACRO_HELPERS
#define MATRIX2_MACRO_HELPERS

#include <matrix2.h>

#define CREATE_MATRIX2(data_type, num_rows, num_cols, buffer)           \
    {                                                                   \
        .rows = num_rows, .cols = num_cols, .dtype = sizeof(data_type), \
        .data = (void* const)buffer,                                    \
    }

#define PRINT_MATRIX2(m, format, dtype)                      \
    {                                                        \
        for (size_t i = 0; i < m.rows; ++i) {                \
            for (size_t j = 0; j < m.cols; ++j) {            \
                matrix_segment_view row = m2_get_row(&m, i); \
                void* val = m2_get_from_row(&row, j);        \
                printf(format, *((dtype*)val));              \
            }                                                \
            printf("\n");                                    \
        }                                                    \
    }

#endif  // MATRIX2_MACRO_HELPERS
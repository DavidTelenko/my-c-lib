#ifndef MATRIX2_MACRO_HELPERS
#define MATRIX2_MACRO_HELPERS

#include <matrix2.h>
#include <types.h>

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

#define DEFINE_APPLY_LAMBDA(dtype, op, name)               \
    void dtype##_##apply##_##name(void* const dest,        \
                                  void const* const lhs,   \
                                  void const* const rhs) { \
        *(dtype*)dest op*(dtype*)lhs**(dtype*)rhs;         \
    }

#define DEFINE_APPLY_ADD(dtype) DEFINE_APPLY_LAMBDA(dtype, +=, add)
#define DEFINE_APPLY_MULT(dtype) DEFINE_APPLY_LAMBDA(dtype, *=, mult)
#define DEFINE_APPLY_DIV(dtype) DEFINE_APPLY_LAMBDA(dtype, /=, div)
#define DEFINE_APPLY_EQ(dtype) DEFINE_APPLY_LAMBDA(dtype, =, eq)

#define FOR_ALL_TYPES(MACRO) \
    MACRO(f32)               \
    MACRO(f64)               \
    MACRO(i8)                \
    MACRO(i16)               \
    MACRO(i32)               \
    MACRO(i64)               \
    MACRO(u8)                \
    MACRO(u16)               \
    MACRO(u32)               \
    MACRO(u64)

FOR_ALL_TYPES(DEFINE_APPLY_ADD)
FOR_ALL_TYPES(DEFINE_APPLY_MULT)
FOR_ALL_TYPES(DEFINE_APPLY_DIV)
FOR_ALL_TYPES(DEFINE_APPLY_EQ)

#endif  // MATRIX2_MACRO_HELPERS
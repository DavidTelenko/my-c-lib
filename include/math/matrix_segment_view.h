#ifndef MATRIX_SEGMENT_VIEW_H
#define MATRIX_SEGMENT_VIEW_H

#include <stdlib.h>

typedef struct {
    void* const ref;
    const size_t begin;
    const size_t end;
} matrix_segment_view;

#endif  // MATRIX_SEGMENT_VIEW_H

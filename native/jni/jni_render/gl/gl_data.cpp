//
// Created by 赖振奇 on 2017/4/18.
//

#include <assert.h>
#include <cstdlib>
#include "gl_data.h"

void update_gl_data(GLData *glData, const void *data, size_t size) {
    assert(glData != NULL);
    if (glData->size == size) {
        memcpy(glData->data, data, size);
    } else {
        if (glData->data != NULL) free(glData->data);
        glData->data = malloc(size);
        glData->size = size;
    }
}

void release_gl_data(GLData *glData) {
    assert(glData != NULL);
    if (glData->data != NULL) free(glData->data);
    glData->data = NULL;
    glData->size = 0;
}

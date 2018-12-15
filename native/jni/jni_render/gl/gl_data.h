//
// Created by 赖振奇 on 2017/4/18.
//

#ifndef VIRTUALBEAUTY_GL_DATA_H
#define VIRTUALBEAUTY_GL_DATA_H

#include <cstddef>

typedef struct {
    void *data;
    size_t size;
} GLData;

void update_gl_data(GLData *glData, const void *data, size_t size);

void release_gl_data(GLData *glData);

#endif //VIRTUALBEAUTY_GL_DATA_H

//
// Created by 赖振奇 on 2017/3/29.
//

#ifndef VIRTUALBEAUTY_GL_PROGRAM_LOADER_H
#define VIRTUALBEAUTY_GL_PROGRAM_LOADER_H

#include <GLES2/gl2.h>

#define TEXTURE2D_IMAGE 0
#define RAW_TEXTURE2D_IMAGE  1
#define POLYGON  2
#define TEXTURE_OVERLAY 3
#define TEXTURE_MULTIPLY_COLOR 4
#define TEXTURE_MULTIPLY_TEXTURE  5
#define GAUSSIAN_BLUR  6
#define PS_GRAY  7
#define PS_LEVELS  8
#define LIGHTEN  9
#define TRANSFORM4_3TO16_9 10
#define BILATERALFILTER 11
#define SKINCOLOR 12
#define ALPHA_TEXTURE_COLOR 13
#define YUV_2_RGB 14
#define YUVSP_2_RGB 15
#define POLYGON_COLOR 16
#define MESH 17

typedef unsigned int uint;

namespace glutil {

    GLuint get_program_by_type(uint type);

    void release_all_program();
}
#endif //VIRTUALBEAUTY_GL_PROGRAM_LOADER_H

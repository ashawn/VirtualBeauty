//
// Created by 赖振奇 on 2017/2/22.
//

#ifndef VIRTUALBEAUTY_BASETEXTURE_H
#define VIRTUALBEAUTY_BASETEXTURE_H

#include <GLES2/gl2.h>

namespace render {

    class BaseTexture {

    public:
        virtual void init() = 0;

        virtual void draw() = 0;

        virtual void loadShader(GLuint program) = 0;

        virtual void update(int width, int height) = 0;

        virtual void release() = 0;
    };

}


#endif //VIRTUALBEAUTY_BASETEXTURE_H

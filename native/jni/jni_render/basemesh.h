//
// Created by 赖振奇 on 2017/2/22.
//

#ifndef VIRTUALBEAUTY_BASEMESH_H
#define VIRTUALBEAUTY_BASEMESH_H

#include <GLES2/gl2.h>

namespace render {

    class BaseMesh {

    public:
        virtual void init() = 0;

        virtual void draw() = 0;

        virtual void loadShader(GLuint program) = 0;

        virtual void release() = 0;
    };

}


#endif //VIRTUALBEAUTY_BASEMESH_H

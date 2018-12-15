//
// Created by 张硕 on 2017/5/9.
//

#ifndef VIRTUALBEAUTY_POLYGONCOLOR_H
#define VIRTUALBEAUTY_POLYGONCOLOR_H

#include <jni_render/gl/gl_data.h>
#include "basetexture.h"

namespace render {
    class PolygonColor : public BaseTexture {

    public:
        virtual void loadShader(GLuint program) override;

        virtual void init() override;

        virtual void draw() override;

        virtual void update(int width, int height) override;

        virtual void release() override;

        void setVertexData(GLfloat *VertexData, int vertex_len, GLbyte *IndicesData,
                        int indice_len);

        void setColor(float f0,float f1,float f2,float f3);

        GLuint getFrameTextureId();

    private:
        const static int POSITION_DATA_SIZE = 3;
        GLuint m_Vertex_GL_Buffer;
        GLuint m_Index_GL_Buffer;
        GLuint m_Frame_GL_Buffer;
        GLuint mFrameTexture;
        GLuint m_Program;
        GLuint m_Position_Handle;
        GLint m_ColorHandle;

        GLfloat m_Color[4];

        GLData m_Vertex_Data = {NULL,0};
        GLData m_Indices_Data = {NULL,0};

        GLint mWidth;
        GLint mHeight;
    };
}


#endif //VIRTUALBEAUTY_POLYGONCOLOR_H

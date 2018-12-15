//
// Created by 张硕 on 2017/4/6.
//

#ifndef VIRTUALBEAUTY_TEXTURE2D_H
#define VIRTUALBEAUTY_TEXTURE2D_H

#include <jni_render/gl/gl_data.h>
#include "basetexture.h"

namespace render {

    class Texture2D : public BaseTexture {

    public:
        virtual void init() override;

        virtual void draw() override;

        virtual void loadShader(GLuint program) override;

        virtual void update(int width, int height) override;

        virtual void release() override;

        void setRectData(GLfloat *VertexData, int vertex_len, GLbyte *IndicesData,
                         int indice_len);

        void setTexturePath(const char *texturepath);

    private:
        const static int BYTES_PER_FLOAT = sizeof(float);
        const static GLint POSITION_DATA_SIZE = 3;
        const static GLint TEXCOORD_DATA_SIZE = 2;
        const static GLint POSITION_OFFSET = 0;
        const static GLint TEXCOORD_OFFSET = POSITION_DATA_SIZE * BYTES_PER_FLOAT;
        const static GLint VERTEXT_DATA_STRIDE =
                (POSITION_DATA_SIZE + TEXCOORD_DATA_SIZE) * BYTES_PER_FLOAT;

        GLData m_Vertex_Data = {NULL, 0};
        GLData m_Indice_Data = {NULL, 0};

        GLuint m_Vertex_GL_Buffer = 0;
        GLuint m_Indice_GL_Buffer = 0;
        GLuint m_Program = 0;
        GLuint mGLTexture;
        GLint m_Position_Handle = 0;
        GLint m_TexCoord_Handle = 0;
        GLint mTexture_Handle = 0;

        GLint mWidth;
        GLint mHeight;
    };
}


#endif //VIRTUALBEAUTY_TEXTURE2D_H

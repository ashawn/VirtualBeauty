//
// Created by 张硕 on 2017/3/16.
//

#ifndef VIRTUALBEAUTY_TEXTUREMULTIPLYTEXTURE_H
#define VIRTUALBEAUTY_TEXTUREMULTIPLYTEXTURE_H

#include "basetexture.h"

namespace render {

    class TextureMultiplyTexture : public BaseTexture {

    public:
        virtual void loadShader(GLuint program) override;

        virtual void init() override;

        virtual void draw() override;

        virtual void update(int width, int height) override;

        virtual void release() override;

        void setBaseTextureId(GLuint BaseTextureId);

        void setBlendTextureId(GLuint BlendTextureId);

        GLuint getFrameTextureId();

    private:
        const static int BYTES_PER_FLOAT = sizeof(float);
        const static GLint POSITION_DATA_SIZE = 3;
        const static GLint TEXCOORD_DATA_SIZE = 2;
        const static GLint POSITION_OFFSET = 0;
        const static GLint TEXCOORD_OFFSET = POSITION_DATA_SIZE * BYTES_PER_FLOAT;
        const static GLint VERTEXT_DATA_STRIDE =
                (POSITION_DATA_SIZE + TEXCOORD_DATA_SIZE) * BYTES_PER_FLOAT;
        float VERTEX_DATA[20] = {
                1.0f, 1.0f, 0.0f, // Position 0
                1.0f, 0.0f,       // TexCoord 0
                1.0f, -1.0f, 0.0f, // Position 1
                1.0f, 1.0f,       // TexCoord 1
                -1.0f, -1.0f, 0.0f, // Position 2
                0.0f, 1.0f,       // TexCoord 2
                -1.0f, 1.0f, 0.0f, // Position 3
                0.0f, 0.0f        // TexCoord 3
        };

        GLubyte INDICES_DATA[6] = {
                0, 1, 3,
                1, 2, 3
        };

        GLuint m_Vertex_GL_BUFFER;
        GLuint m_Index_GL_BUFF;
        GLuint m_Frame_GL_Buffer;
        GLuint mFrameTexture;
        GLuint mBaseTEXTURE;
        GLuint mBlendTEXTURE;

        GLuint m_Program;
        GLint m_Position_Handle;
        GLint m_TexCoord_Handle;
        GLint m_BaseTextureHandle;
        GLint m_BlendTextureHandle;

        GLint mWidth;
        GLint mHeight;
    };
}


#endif //VIRTUALBEAUTY_TEXTUREMULTIPLYTEXTURE_H

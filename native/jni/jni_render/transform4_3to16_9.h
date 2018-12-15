//
// Created by 张硕 on 2017/3/30.
//

#ifndef VIRTUALBEAUTY_TRANSFORM4_3TO16_9_H
#define VIRTUALBEAUTY_TRANSFORM4_3TO16_9_H

#include "basetexture.h"

namespace render {

    class Transform4_3To16_9 : public BaseTexture {

    public:
        virtual void loadShader(GLuint program) override;

        virtual void init() override;

        virtual void draw() override;

        virtual void update(int width, int height) override;

        virtual void release() override;

        void setTextureId(GLuint TextureId);

    private:
        const static int BYTES_PER_FLOAT = sizeof(float);
        const static GLint POSITION_DATA_SIZE = 3;
        const static GLint TEXCOORD_DATA_SIZE = 2;
        const static GLint POSITION_OFFSET = 0;
        const static GLint TEXCOORD_OFFSET = POSITION_DATA_SIZE * BYTES_PER_FLOAT;
        const static GLint VERTEXT_DATA_STRIDE =
                (POSITION_DATA_SIZE + TEXCOORD_DATA_SIZE) * BYTES_PER_FLOAT;
        float CAMERA_VERTEX_DATA[20] = {
                1.0f, 1.0f, 0.99999f, // Position 0
                1.0f, 0.125f,       // TexCoord 0
                1.0f, -1.0f, 0.999999f, // Position 1
                1.0f, 0.875f,       // TexCoord 1
                -1.0f, -1.0f, 0.99999f, // Position 2
                0.0f, 0.875f,       // TexCoord 2
                -1.0f, 1.0f, 0.99999f, // Position 3
                0.0f, 0.125f        // TexCoord 3
        };

        GLubyte CAMERA_INDICES_DATA[6] = {
                0, 1, 3,
                1, 2, 3
        };

        GLuint m_Vertex_GL_BUFFER;
        GLuint m_Index_GL_BUFF;
        GLuint m_GL_TEXTURE;

        GLuint m_Program;
        GLint m_Position_Handle;
        GLint m_TexCoord_Handle;

        GLint mWidth;
        GLint mHeight;

    };
}


#endif //VIRTUALBEAUTY_TRANSFORM4_3TO16_9_H

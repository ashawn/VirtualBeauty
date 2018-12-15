//
// Created by 张硕 on 2017/3/16.
//

#ifndef VIRTUALBEAUTY_LIPREGION_H
#define VIRTUALBEAUTY_LIPREGION_H

#include <jni_render/gl/gl_data.h>
#include "basetexture.h"

namespace render {
    class LipRegion : public BaseTexture {

    public:
        virtual void loadShader(GLuint program) override;

        virtual void init() override;

        virtual void draw() override;

        virtual void update(int width, int height) override;

        virtual void release() override;

        void setLipData(GLfloat *lipVertexData, int vertex_len, GLbyte *lipIndicesData,
                        int indice_len);

        GLuint getLipRegionTextureId();

    private:
        const static int POSITION_DATA_SIZE = 3;
        GLuint m_Vertex_GL_Buffer;
        GLuint m_Index_GL_Buffer;
        GLuint m_Frame_GL_Buffer;
        GLuint mFrameTexture;
        GLuint m_Program;
        GLuint m_Position_Handle;

        GLData m_Lip_Vertex_Data = {NULL,0};
        GLData m_Lip_Indices_Data = {NULL,0};

        GLint mWidth;
        GLint mHeight;
    };
}


#endif //VIRTUALBEAUTY_LIPREGION_H

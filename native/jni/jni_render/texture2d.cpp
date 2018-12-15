//
// Created by 张硕 on 2017/4/6.
//

#include <glog/logging.h>
#include <jni_render/gl/gl_program_loader.h>
#include <jni_render/gl/glutils.h>
#include <jni_render/image/image_data.h>
#include <jni_render/gl/gl_data.h>
#include "texture2d.h"

using namespace render;

void Texture2D::init() {
    LOG(INFO) << "Texture2D init";
    m_Program = glutil::get_program_by_type(TEXTURE2D_IMAGE);
    m_Position_Handle = glGetAttribLocation(m_Program, "position");
    m_TexCoord_Handle = glGetAttribLocation(m_Program, "texCoord");
    mTexture_Handle = glGetUniformLocation(m_Program,"mTexture");

    glGenTextures(1, &mGLTexture);

    glGenBuffers(1, &m_Vertex_GL_Buffer);
    glGenBuffers(1, &m_Indice_GL_Buffer);
}

void Texture2D::setTexturePath(const char *texturepath) {
    glBindTexture(GL_TEXTURE_2D,
                  mGLTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    RawImageData rawImageData = glutil::load_png_asset_into_texture(texturepath);
    glTexImage2D(GL_TEXTURE_2D, 0, rawImageData.gl_color_format, rawImageData.width,
                 rawImageData.height,
                 0, rawImageData.gl_color_format, GL_UNSIGNED_BYTE, rawImageData.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,
                  0); // Unbind texture when done, so we won't accidentily mess up our texture.
    release_raw_image_data(&rawImageData);
}

void Texture2D::draw() {
    LOG(INFO) << "Texture2D draw";

    if (m_Vertex_Data.data == NULL || m_Indice_Data.data == NULL)
        return;

    glUseProgram(m_Program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mGLTexture);
    glUniform1i(mTexture_Handle,0);

    glEnableVertexAttribArray(m_Position_Handle);
    glEnableVertexAttribArray(m_TexCoord_Handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Indice_GL_Buffer);

    glVertexAttribPointer(m_Position_Handle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)POSITION_OFFSET);
    glVertexAttribPointer(m_TexCoord_Handle, TEXCOORD_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEXT_DATA_STRIDE, (GLvoid*)TEXCOORD_OFFSET);

    glDrawElements(GL_TRIANGLES, m_Indice_Data.size/ sizeof(GLubyte), GL_UNSIGNED_BYTE, (GLvoid *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(m_Position_Handle);
    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);
}

void Texture2D::setRectData(GLfloat *VertexData, int vertex_len, GLbyte *IndicesData,
                                  int indice_len) {
    LOG(INFO) << "set rect data";
    update_gl_data(&m_Vertex_Data,VertexData,vertex_len * sizeof(GLfloat));
    update_gl_data(&m_Indice_Data,IndicesData,indice_len * sizeof(GLubyte));

    glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_GL_Buffer);
    glBufferData(GL_ARRAY_BUFFER, m_Vertex_Data.size, m_Vertex_Data.data, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Indice_GL_Buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indice_Data.size, m_Indice_Data.data, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Texture2D::loadShader(GLuint program) {

}

void Texture2D::update(int width, int height) {

}

void Texture2D::release() {
    LOG(INFO) << "Texture2D release";
    release_gl_data(&m_Vertex_Data);
    release_gl_data(&m_Indice_Data);
    glDeleteProgram(m_Program);
}


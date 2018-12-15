//
// Created by 张硕 on 2017/5/12.
//

#include <glog/logging.h>
#include <jni_render/gl/gl_program_loader.h>
#include <jni_render/gl/glutils.h>
#include <jni_render/gl/gl_data.h>
#include "objmesh.h"

using namespace render;

void OBJMesh::init() {
    LOG(INFO) << "OBJMesh init";
    m_Program = glutil::get_program_by_type(MESH);

    m_ProjectionMatrixLocation = glGetUniformLocation(m_Program, "ProjectionMatrix");
    m_ModelViewMatrixLocation  = glGetUniformLocation(m_Program, "ModelViewMatrix");
    m_NormalMatrixLocation     = glGetUniformLocation(m_Program, "NormalMatrix");
    m_RotateMatrixLocation     = glGetUniformLocation(m_Program, "RotateMatrix");
    m_LightPositionLocation    = glGetUniformLocation(m_Program, "LightPosition");
    m_AmbientLightLocation     = glGetUniformLocation(m_Program, "AmbientLight");

    cam_position = gl::vec3(0, 0, 5);
    cam_rotation = gl::vec2(0, 0);
    light_rotation = gl::vec2(-90, 0);
    mProjectionMat = gl::perspective(gl::to_radians(60.f), 16.0f / 9.0f, 0.1f, 100.f);
    mViewMat = gl::xrotation(gl::to_radians(cam_rotation[0]))
              * gl::yrotation(gl::to_radians(cam_rotation[1]))
              * translation(-cam_position);
    mNormalMat = gl::normal(mViewMat);
    mLightVec = gl::xrotation(gl::to_radians(light_rotation[0]))
                * gl::yrotation(gl::to_radians(light_rotation[1])) * gl::vec4(0, 0, 1, 0);

    glGenTextures(1, &mDiffuseTexture);
    glGenTextures(1, &mNormalTexture);
    glGenTextures(1, &mSpecularTexture);

    setTexturePath("test/RAYGLASS.png",mDiffuseTexture);
    setTexturePath("test/RAYGLASS.png",mNormalTexture);
    setTexturePath("test/RAYGLASS.png",mSpecularTexture);
}

void OBJMesh::setOBJPath(const char *texturepath) {
    if((m_Object = obj_create(texturepath))) {
        LOG(INFO) << "OBJMesh setOBJPath";
        obj_set_vert_loc(m_Object, glGetAttribLocation(m_Program, "vTangent"),
                         glGetAttribLocation(m_Program, "vNormal"),
                         glGetAttribLocation(m_Program, "vTexCoord"),
                         glGetAttribLocation(m_Program, "vPosition"));

        //obj_set_prop_loc(m_Object, OBJ_KN, -1, glGetUniformLocation(m_Program, "NormalTexture"), -1);
        //obj_set_prop_loc(m_Object, OBJ_KD, -1, glGetUniformLocation(m_Program, "DiffuseTexture"), -1);
        //obj_set_prop_loc(m_Object, OBJ_KS, -1, glGetUniformLocation(m_Program, "SpecularTexture"), -1);
    }

}

void OBJMesh::setTexturePath(const char *texturepath , GLuint TextureID){
    glBindTexture(GL_TEXTURE_2D,
                  TextureID); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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

void OBJMesh::setRotateMatrix(GLfloat *RotateMatrix,int length) {
    LOG(INFO) << "setRotateMatrix";
    int i;
    for(i = 0;i<9;i++){
        mRotateMatrix[i] = RotateMatrix[i];
    }
}

void OBJMesh::draw() {
    LOG(INFO) << "OBJMesh draw";

    if (m_Object == NULL)
        return;

    glUseProgram(m_Program);

    glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_TRUE, mProjectionMat);
    glUniformMatrix4fv(m_ModelViewMatrixLocation,  1, GL_TRUE, mViewMat);
    glUniformMatrix3fv(m_NormalMatrixLocation,     1, GL_TRUE, mNormalMat);
    glUniformMatrix3fv(m_RotateMatrixLocation,     1, GL_TRUE, mRotateMatrix);
    glUniform4fv      (m_LightPositionLocation,    1,          mLightVec);
    glUniform4f(m_AmbientLightLocation,  0.2, 0.2, 0.2, 1.0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mDiffuseTexture);
    glUniform1i(glGetUniformLocation(m_Program, "DiffuseTexture"),0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,mNormalTexture);
    glUniform1i(glGetUniformLocation(m_Program, "NormalTexture"),1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,mSpecularTexture);
    glUniform1i(glGetUniformLocation(m_Program, "SpecularTexture"),2);

    obj_render(m_Object);

    glUseProgram(0);
}

void OBJMesh::loadShader(GLuint program) {

}

void OBJMesh::release() {
    LOG(INFO) << "OBJMesh release";
    glDeleteProgram(m_Program);
}


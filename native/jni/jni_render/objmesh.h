//
// Created by 张硕 on 2017/5/12.
//

#ifndef VIRTUALBEAUTY_OBJMESH_H
#define VIRTUALBEAUTY_OBJMESH_H

#include <jni_render/gl/gl_data.h>
#include <jni_render/gl/gl_obj_loader.h>
//#include <glm.hpp>
#include <jni_render/gl/glfundamentals.h>
#include "basemesh.h"

namespace render {

    class OBJMesh : public BaseMesh {

    public:
        virtual void init() override;

        virtual void draw() override;

        virtual void loadShader(GLuint program) override;

        virtual void release() override;

        void setOBJPath(const char *texturepath);

        void setTexturePath(const char *texturepath , GLuint TextureID);

        void setRotateMatrix(GLfloat *RotateMatrix,int length);

    private:
        obj* m_Object;
        GLuint m_Program = 0;

        GLint m_ProjectionMatrixLocation;
        GLint m_ModelViewMatrixLocation;
        GLint m_NormalMatrixLocation;
        GLint m_RotateMatrixLocation;
        GLint m_LightPositionLocation;
        GLint m_AmbientLightLocation;

        GLuint mDiffuseTexture;
        GLuint mNormalTexture;
        GLuint mSpecularTexture;

        gl::vec3 cam_position;
        gl::vec2 cam_rotation;
        gl::vec2 light_rotation;
        gl::mat4 mProjectionMat;
        gl::mat4 mViewMat;
        gl::mat3 mNormalMat;
        gl::vec4 mLightVec;
        GLfloat  mRotateMatrix[9];
    };
}


#endif //VIRTUALBEAUTY_OBJMESH_H

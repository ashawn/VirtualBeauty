uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 RotateMatrix;
uniform mat3 NormalMatrix;
uniform vec4 LightPosition;

attribute vec4 vPosition;
attribute vec3 vNormal;
attribute vec3 vTangent;
attribute vec2 vTexCoord;

varying vec3 fView;
varying vec3 fLight;
varying vec2 fTexCoord;

mat3 transpose(mat3 inMatrix) {
    vec3 i0 = inMatrix[0];
    vec3 i1 = inMatrix[1];
    vec3 i2 = inMatrix[2];

    highp mat3 outMatrix = mat3(
                 vec3(i0.x, i1.x, i2.x),
                 vec3(i0.y, i1.y, i2.y),
                 vec3(i0.z, i1.z, i2.z)
                 );

    return outMatrix;
}

void main()
{
    // Tangent space vectors give the columns of the eye-to-tangent transform.

    vec3 N = NormalMatrix * vNormal;
    vec3 T = NormalMatrix * vTangent;
    mat3 M = transpose(mat3(T, cross(N, T), N));

    // Compute the per-fragment attributes.

    fView     =  M * vec3(ModelViewMatrix * vPosition);
    fLight    =  M * vec3(ModelViewMatrix * LightPosition);
    fTexCoord =  vTexCoord;

    vec3 test = RotateMatrix * vec3(vPosition);

    gl_Position = ProjectionMatrix  * vec4(test,1);
}
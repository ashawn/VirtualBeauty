#extension GL_OES_EGL_image_external : require
precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform samplerExternalOES cameraTex;

void main()
{
	gl_FragColor = texture2D(cameraTex, TexCoord);
}
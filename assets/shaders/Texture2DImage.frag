precision mediump float;
varying vec2 TexCoord;

uniform sampler2D mTexture;


void main()
{
    vec4 ourTex = texture2D(mTexture, TexCoord);
	gl_FragColor = ourTex;

}
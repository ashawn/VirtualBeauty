precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D mTexture;
uniform vec4 mColor;

void main()
{
    vec4 mTex = texture2D(mTexture, vec2(TexCoord.x,1.0-TexCoord.y));

	gl_FragColor = mTex * mColor;

}
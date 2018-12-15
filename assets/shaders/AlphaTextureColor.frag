precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D mTexture;
uniform vec4 mColor;
uniform float mAlphaFactor;

void main()
{
    vec4 mTex = texture2D(mTexture, vec2(TexCoord.x,TexCoord.y));

    vec4 color = mColor;
    color.a = mTex.a / mAlphaFactor;

	gl_FragColor = color;

}
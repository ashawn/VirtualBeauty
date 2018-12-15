precision mediump float;

uniform sampler2D mTexture;

varying vec2 TexCoord;

void main()
{
    vec4 mTex = texture2D(mTexture, vec2(TexCoord.x,1.0-TexCoord.y));
    gl_FragColor = mTex;
}

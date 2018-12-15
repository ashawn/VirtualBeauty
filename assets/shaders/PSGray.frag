precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D mTexture;

void main()
{
    vec4 mTex = texture2D(mTexture, TexCoord);

    vec3 mColor = mTex.rgb;

    vec3 W = vec3(0.3,0.59,0.11);
    float luminace = dot(mColor,W);

	gl_FragColor = vec4(luminace,luminace,luminace,1);

}
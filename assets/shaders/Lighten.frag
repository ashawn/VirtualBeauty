precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D cameraTexture;
uniform sampler2D mTexture;

void main()
{
    vec4 cameraTex = texture2D(cameraTexture, vec2(TexCoord.x,1.0-TexCoord.y));
    vec4 mTex = texture2D(mTexture, vec2(TexCoord.x,1.0-TexCoord.y));

    gl_FragColor = max(cameraTex,mTex);
}
precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D baseTexture;
uniform sampler2D blendTexture;

void main()
{
    vec4 baseTex = texture2D(baseTexture, vec2(TexCoord.x,1.0-TexCoord.y));
    vec4 blendTex = texture2D(blendTexture, vec2(TexCoord.x,1.0-TexCoord.y));

	gl_FragColor = baseTex * blendTex;

}
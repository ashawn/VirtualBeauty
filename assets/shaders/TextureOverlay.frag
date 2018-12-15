precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D baseTexture;
uniform sampler2D blendTexture;

float OverlayBlendMode(float basePixel,float blendPixel)
{
    if(blendPixel < 0.5)
     {
         return basePixel;
     }
    else
    {
        return (1.0 - 2.0 * (1.0 - basePixel) * (1.0 - blendPixel));
    }
}

void main()
{
    vec4 baseTex = texture2D(baseTexture, vec2(TexCoord.x,1.0-TexCoord.y));
    vec4 blendTex = texture2D(blendTexture, vec2(TexCoord.x,1.0-TexCoord.y));

    vec4 blendedImage = baseTex;
    blendedImage.x = OverlayBlendMode(baseTex.x, blendTex.x);
    blendedImage.y = OverlayBlendMode(baseTex.y, blendTex.y);
    blendedImage.z = OverlayBlendMode(baseTex.z, blendTex.z);

    gl_FragColor = baseTex * 0.7 + blendedImage * 0.3;
}
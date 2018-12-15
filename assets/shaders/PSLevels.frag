precision mediump float;

uniform sampler2D mTexture;

varying vec2 TexCoord;

float saturate( float v)
{
    if( v < 0.0 )
        return 0.0;
    if( v > 1.0 )
        return 1.0;

    return v;
}

float levelChannel(float inPixel, float inBlack, float inGamma, float inWhite, float outBlack, float outWhite) {
            float pixelResult;
            pixelResult = (inPixel * 255.0);
            pixelResult = max(0.0, pixelResult - inBlack);
            pixelResult = saturate(pow(pixelResult / (inWhite - inBlack), inGamma));
            pixelResult = (pixelResult * (outWhite - outBlack) + outBlack)/255.0;
            return pixelResult;
}

vec3 levels(vec3 inPixel, float inBlack, float inGamma, float inWhite, float outBlack, float outWhite) {
    vec3 o = vec3(1.0);
    o.r = levelChannel(inPixel.r, inBlack, inGamma, inWhite, outBlack, outWhite);
    o.g = levelChannel(inPixel.g, inBlack, inGamma, inWhite, outBlack, outWhite);
    o.b = levelChannel(inPixel.b, inBlack, inGamma, inWhite, outBlack, outWhite);
    return o;
}

void main()
{
    vec4 mTex = texture2D(mTexture, TexCoord);
    gl_FragColor = vec4(levels(mTex.rgb,50.0,1.61,200.0,0.0,255.0),mTex.a);
}

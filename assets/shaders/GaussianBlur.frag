precision mediump float;

uniform sampler2D mTexture;
uniform bool  horizontal;
uniform float width;
uniform float height;

varying vec2 TexCoord;

void main()
{
     float weight[5];
     weight[0] = 0.3;
     weight[1] = 0.2;
     weight[2] = 0.15;
     weight[3] = 0.05;
     weight[4] = 0.02;
     vec2 tex_offset = vec2(1.0/width,1.0/height); // gets size of single texel

     vec4 texturecolor = texture2D(mTexture, TexCoord);

     vec3 result = texturecolor.rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture2D(mTexture, TexCoord + vec2(tex_offset.x * float(i*2), 0.0)).rgb * weight[i];
            result += texture2D(mTexture, TexCoord - vec2(tex_offset.x * float(i*2), 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture2D(mTexture, TexCoord + vec2(0.0, tex_offset.y * float(i*2))).rgb * weight[i];
             result += texture2D(mTexture, TexCoord - vec2(0.0, tex_offset.y * float(i*2))).rgb * weight[i];
         }
     }
    gl_FragColor = vec4(result,texturecolor.a);
}

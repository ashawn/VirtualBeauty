precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D cameraTexture;
uniform sampler2D mTexture;

uniform float Intensity;

void main()
{
    highp vec4 bilateral = texture2D(mTexture, TexCoord);
    highp vec4 origin = texture2D(cameraTexture,vec2(TexCoord.x,1.0-TexCoord.y));
    highp vec4 color;
    lowp float r = origin.r;
    lowp float g = origin.g;
    lowp float b = origin.b;
    lowp float smoothDegree = 0.5;
    if (r > 0.3725 && g > 0.1568 && b > 0.0784 && r > b && (max(max(r, g), b) - min(min(r, g), b)) > 0.0588 && abs(r-g) > 0.0588) {
     color = (1.0 - smoothDegree) * (origin - bilateral) + bilateral;
    }
    else {
     color = origin;
    }
    color.r = log(1.0 + (Intensity - 1.0) * color.r)/log(Intensity);
    color.g = log(1.0 + (Intensity - 1.0) * color.g)/log(Intensity);
    color.b = log(1.0 + (Intensity - 1.0) * color.b)/log(Intensity);
    gl_FragColor = color;
}
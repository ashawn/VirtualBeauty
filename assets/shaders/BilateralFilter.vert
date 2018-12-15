attribute vec3 position;
attribute vec2 texCoord;

const int GAUSSIAN_SAMPLES = 9;

uniform float width;
uniform float height;

varying vec2 TexCoord;
varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];

void main()
{
    gl_Position = vec4(position, 1.0);
    TexCoord = texCoord;

    // Calculate the positions for the blur
    int multiplier = 0;
    vec2 blurStep;
    vec2 singleStepOffset = vec2(1.0/width,1.0/height); // gets size of single texel

    for (int i = 0; i < GAUSSIAN_SAMPLES; i++)
    {
        multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));
        // Blur in x (horizontal)
        blurStep = float(multiplier) * singleStepOffset;
        blurCoordinates[i] = texCoord + blurStep;
    }
}

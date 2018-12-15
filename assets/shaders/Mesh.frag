precision mediump float;
uniform vec4 AmbientLight;

uniform sampler2D NormalTexture;
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;

varying vec3 fView;
varying vec3 fLight;
varying vec2 fTexCoord;

void main()
{
    // Sample the textures.

    vec4 tN = texture2D(NormalTexture,   fTexCoord);
    vec4 tD = texture2D(DiffuseTexture,  fTexCoord);
    vec4 tS = texture2D(SpecularTexture, fTexCoord);

    // Determine the per-fragment lighting vectors.

    vec3 N = normalize(2.0 * tN.xyz - 1.0);
    vec3 L = normalize(fLight);
    vec3 V = normalize(fView);
    vec3 R = reflect(L, N);

    // Compute the diffuse shading.

    float kd =     max(dot(L, N), 0.0);
    float ks = pow(max(dot(V, R), 0.0), 8.0);

    // Calculate the fragment color.

    gl_FragColor.rgb = vec3(AmbientLight * tD + kd * tD + tS * ks);
    gl_FragColor.a   = tD.a;

    gl_FragColor = tD;
    //gl_FragColor = vec4(0.5,0.5,0.5,0.5);
}
precision mediump float;
varying vec2 TexCoord;

// Texture samplers
uniform sampler2D texY; // Y
uniform sampler2D texUV; // U

vec3 yuv2rgb(in vec3 yuv)
{
	// YUV offset
	const vec3 offset = vec3(-0.0625, -0.5, -0.5);
	// RGB coefficients
	const vec3 Rcoeff = vec3( 1.164, 0.000,  1.596);
	const vec3 Gcoeff = vec3( 1.164, -0.391, -0.813);
	const vec3 Bcoeff = vec3( 1.164, 2.018,  0.000);

	vec3 rgb;

	yuv = clamp(yuv, 0.0, 1.0);

	yuv += offset;

	rgb.r = dot(yuv, Rcoeff);
	rgb.g = dot(yuv, Gcoeff);
	rgb.b = dot(yuv, Bcoeff);
	return rgb;
}

vec3 get_yuv_from_texture(in vec2 tcoord)
{
	vec3 yuv;
	yuv.x = texture2D(texY, tcoord).r;
	// Get the U and V values
	yuv.y = texture2D(texUV, tcoord).a;
	yuv.z = texture2D(texUV, tcoord).r;
	return yuv;
}

vec4 mytexture2D(in vec2 tcoord)
{
	vec3 rgb, yuv;
	yuv = get_yuv_from_texture(tcoord);
	// Do the color transform
	rgb = yuv2rgb(yuv);
	return vec4(rgb, 1.0);
}

void main()
{
	gl_FragColor = mytexture2D(TexCoord);
}
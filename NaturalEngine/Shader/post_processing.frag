#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D cloudTEX;
uniform sampler2D depthTex;
uniform bool wireframe;

uniform vec2 resolution;

#define HDR(col, exps) 1.0 - exp(-col * exps)

vec3 TonemapACES(vec3 x)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;
	return(x * (A * x + B)) /(x * (C * x +D) + E);
}

void main()
{
	vec4 cloud = texture(cloudTEX, TexCoords);
	vec4 bg = texture(screenTexture, TexCoords);
	float mixVal = (texture(depthTex, TexCoords).r < 1.0 ? 0.0 : 1.0);
	vec4 col = mix(bg, cloud, (!wireframe ? mixVal : 0.0));

	const float gamma = 2.2;
	const float exposure = 3.0;

	// vignette
	vec2 uv = gl_FragCoord.xy / resolution;
	col.rgb *= pow(16.0 * uv.x * uv.y * (1.0- uv.x) * (1.0 - uv.y), 0.11);
	FragColor = col;
}
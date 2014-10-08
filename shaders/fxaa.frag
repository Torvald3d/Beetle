#version 110

// from https://github.com/mitsuhiko/webgl-meincraft/blob/master/assets/shaders/fxaa.glsl

const float FXAA_REDUCE_MIN = 1.0/128.0;
const float FXAA_REDUCE_MUL = 1.0/8.0;
const float FXAA_SPAN_MAX = 8.0;

uniform sampler2D texture;
uniform vec2 screenSize;

varying vec2 vTexCoord;

void main() {
	vec4 color;
	vec2 inverseVP = vec2(1.0/screenSize.x, 1.0/screenSize.y);
	vec3 rgbNW = texture2D(texture, vTexCoord + vec2(-1.0, -1.0) * inverseVP).xyz;
	vec3 rgbNE = texture2D(texture, vTexCoord + vec2(1.0, -1.0) * inverseVP).xyz;
	vec3 rgbSW = texture2D(texture, vTexCoord + vec2(-1.0, 1.0) * inverseVP).xyz;
	vec3 rgbSE = texture2D(texture, vTexCoord + vec2(1.0, 1.0) * inverseVP).xyz;
	vec4 rgbM  = texture2D(texture, vTexCoord);
	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM  = dot(rgbM.rgb,  luma);
	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
						  (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
			  max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
			  dir * rcpDirMin)) * inverseVP;

	vec3 rgbA = 0.5 * (
		texture2D(texture, vTexCoord + dir * (1.0 / 3.0 - 0.5)).xyz +
		texture2D(texture, vTexCoord + dir * (2.0 / 3.0 - 0.5)).xyz);
	vec3 rgbB = rgbA * 0.5 + 0.25 * (
		texture2D(texture, vTexCoord + dir * -0.5).xyz +
		texture2D(texture, vTexCoord + dir * 0.5).xyz);

	float lumaB = dot(rgbB, luma);
	if ((lumaB < lumaMin) || (lumaB > lumaMax))
		color = vec4(rgbA, rgbM.a);
	else
		color = vec4(rgbB, rgbM.a);

	gl_FragColor = color;
}

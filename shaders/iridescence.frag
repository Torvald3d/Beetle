#version 110

uniform sampler2D texture;
uniform samplerCube envMap;
uniform vec3 light;
uniform float bright;
uniform float focalDistance;
uniform float focalRange;
uniform vec3 eyeDir;
uniform float hueAverage;
uniform float hueRange;

varying vec2 vTexCoord;
varying mat3 vTbn;
varying float zPos;

const float shadowOpacity = 0.7;
const float envBright = 1.0;
const float envLighting = -0.6;
const float iridescenceOpacity = 0.6;
const float hsvSaturation = 0.7;
const float hsvValue = 0.8;
const float addIridescenceValue = 0.2;

vec3 hsv2rgb(float h, float s, float v) {
	vec4 k = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
	vec3 p = abs(fract(vec3(h)+k.xyz)*6.0-k.www);
	return v*mix(k.xxx, clamp(p-k.xxx, 0.0, 1.0), s);
}

void main() {
	vec4 dataTex = texture2D(texture, vTexCoord);
	vec3 normal = normalize(vTbn*(vec3(dataTex.rg, 1.0)*2.0-1.0));
	float lighting = clamp(dot(normal, light), 0.0, 1.0);

	float hue = (1.0-pow(dot(normal, eyeDir), 2.0))*hueRange+hueAverage;
	hue -= (1.0-dataTex.a)*addIridescenceValue;
	vec3 diffuse = hsv2rgb(hue, hsvSaturation, hsvValue)*iridescenceOpacity;

	vec3 env = textureCube(envMap, reflect(-eyeDir, normal)).rgb;
	diffuse += env*bright*envBright+envLighting;

	float shadow = lighting*dataTex.b*shadowOpacity+1.0-shadowOpacity;

	gl_FragColor.rgb = shadow*diffuse;
	gl_FragColor.a = clamp((focalDistance+zPos)/focalRange, -1.0, 1.0);
}

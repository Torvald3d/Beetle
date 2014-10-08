#version 110

uniform sampler2D texture;
uniform samplerCube envMap;
uniform vec3 light;
uniform float bright;
uniform float focalDistance;
uniform float focalRange;
uniform vec3 eyeDir;

varying vec2 vTexCoord;
varying mat3 vTbn;
varying float zPos;

const vec3 ambientColor = vec3(0.025, 0.05, 0.025);
const float envBright = 0.5;
const float envLighting = -0.05;

void main() {
	vec4 dataTex = texture2D(texture, vTexCoord);
	vec3 normal = normalize(vTbn*(vec3(dataTex.rg, 1.0)*2.0-1.0));
	float lighting = clamp(dot(normal, light), 0.0, 1.0);

	vec3 env = textureCube(envMap, reflect(-eyeDir, normal)).rgb;
	float diffuse = dot(env, vec3(0.299, 0.587, 0.114));
	diffuse = diffuse*bright*envBright+envLighting;

	gl_FragColor.rgb = lighting*dataTex.b*vec3(diffuse)+ambientColor;
	gl_FragColor.a = clamp((focalDistance+zPos)/focalRange, -1.0, 1.0);
}

#version 110

uniform sampler2D texture;
uniform vec4 color;
uniform vec3 light;
uniform float bright;
uniform float focalDistance;
uniform float focalRange;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying float zPos;

const vec3 ambientColor = vec3(0.025, 0.05, 0.025);

void main() {
	float ao = texture2D(texture, vTexCoord).b;
	float lighting = clamp(dot(vNormal, light), 0.0, 1.0);

	float blur = clamp((focalDistance+zPos)/focalRange, -1.0, 1.0);
	gl_FragColor = vec4(lighting*ao*bright*color.rgb + ambientColor, blur);
}

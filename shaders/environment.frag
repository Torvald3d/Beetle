#version 110

uniform samplerCube envMap;
uniform float bright;

varying vec3 viewRay;

void main() {
	gl_FragColor = vec4(textureCube(envMap, viewRay).rgb * bright, 1.0);
}

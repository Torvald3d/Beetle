#version 110

uniform float fov;
uniform float aspect;
uniform mat4 invViewMat;

attribute vec2 vertex;

varying vec3 viewRay;

void main() {
	gl_Position = vec4(vertex, 0.0, 1.0);

	viewRay = vec3(vertex*fov, -1.0);
	viewRay.x *= aspect;
	viewRay = vec3(invViewMat*vec4(viewRay, 0.0));
}

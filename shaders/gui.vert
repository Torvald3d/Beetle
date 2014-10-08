#version 110

uniform mat4 projMat;

attribute vec2 vertex;
attribute vec2 texCoord;

varying vec2 vTexCoord;

void main() {
	gl_Position = projMat*vec4(vertex, 0.0, 1.0);
	vTexCoord = texCoord;
}

#version 110

attribute vec2 vertex;
attribute vec2 texCoord;

varying vec2 vTexCoord;

void main() {
	gl_Position = vec4(vertex, 0.0, 1.0);
	vTexCoord = texCoord;
}

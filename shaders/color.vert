#version 110

uniform mat4 matrixProj;
uniform mat4 matrixView;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying float zPos;

void main() {
	vec4 pos = matrixProj*matrixView*vec4(vertex, 1.0);
	gl_Position = pos;
	vTexCoord = texCoord;
	vNormal = normal;
	zPos = pos.z;
}

#version 110

uniform sampler2D guiMap;
uniform sampler2D themeMap;
uniform float themeNum;

varying vec2 vTexCoord;

void main() {
	float a = texture2D(guiMap, vTexCoord).r;
	gl_FragColor = texture2D(themeMap, vec2(a, themeNum));
}

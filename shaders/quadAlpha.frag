#version 110

uniform sampler2D texture;
uniform float alpha;

varying vec2 vTexCoord;

void main() {
	gl_FragColor = texture2D(texture, vTexCoord)*alpha;
}

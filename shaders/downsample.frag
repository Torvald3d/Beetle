#version 110

uniform sampler2D texture;
uniform vec2 screen;
uniform float aspect;

varying vec2 vTexCoord;

void main() {
	vec2 as = vec2(1.0, aspect);

	vec2 poissonDisk[8];
	poissonDisk[0] = vec2( 0.0,       0.0     );
	poissonDisk[1] = vec2( 0.527837, -0.85868 );
	poissonDisk[2] = vec2(-0.040088,  0.536087);
	poissonDisk[3] = vec2(-0.670445, -0.179949);
	poissonDisk[4] = vec2(-0.419418, -0.616039);
	poissonDisk[5] = vec2( 0.440453, -0.639399);
	poissonDisk[6] = vec2(-0.757088,  0.349334);
	poissonDisk[7] = vec2( 0.574619,  0.685879);

	vec4 c = vec4(0.0);

	for (int i=0; i<8; i++) {
		c += texture2D(texture, vTexCoord+poissonDisk[i]*0.003*as);
	}
	gl_FragColor = c/8.0;
}

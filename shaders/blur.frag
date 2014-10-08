#version 110

const int MAX_OFFSET_SIZE = 128;

uniform sampler2D texture;
uniform sampler2D lowTexture;
uniform int offsetSize;
uniform float seed;
uniform vec2 offsets[MAX_OFFSET_SIZE];

varying vec2 vTexCoord;

float rand(){
	return fract(sin(dot(vTexCoord*seed, vec2(12.9898,78.233)))*43758.5453);
}

void main() {
	vec4 resulColor = texture2D(texture, vTexCoord);
	float currentSize = resulColor.a;
	resulColor = vec4 (0.0);

	for (int i=0; i<offsetSize; i++) {
		vec4 highSample = texture2D(texture, vTexCoord+offsets[i]*currentSize);
		vec4 lowSample = texture2D(lowTexture, vTexCoord+offsets[i]*currentSize);
		float sampleSize = abs(highSample.a);

		highSample.rgb = mix(highSample.rgb, lowSample.rgb, sampleSize);
		highSample.a = highSample.a >= currentSize ? 1.0 : highSample.a;

		sampleSize = abs(highSample.a);
		resulColor.rgb += highSample.rgb * sampleSize;
		resulColor.a += sampleSize;
	}

	gl_FragColor = resulColor/resulColor.a;
	gl_FragColor.rgb += 0.03*rand();
	//gl_FragColor = vec4(cd);
	//gl_FragColor = texture2D(lowTexture, vTexCoord);
}

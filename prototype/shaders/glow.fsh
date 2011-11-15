varying vec2 texture_varying;

void main() {
	vec2 n = normalize(vec2(-1, 1));
	float d = abs(dot(texture_varying, n));

	gl_FragColor = max(
		mix(
			vec4(0, 1, 0, 1),
			vec4(0),
			smoothstep(.00, .02, d)
		),
		 mix(
			vec4(0, 1, 0, .45),
			vec4(0),
			smoothstep(.00, .2, d)
		)
	)
	;
}

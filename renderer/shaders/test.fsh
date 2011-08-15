varying lowp vec2 texVarying;

void main() {
	if (length(texVarying) > 1.0) { discard; }

	gl_FragColor = (1.0 - smoothstep(.05, .06, abs(texVarying.y))) * vec4(1, 0, 0, 1)
		+ smoothstep(.05, .06, abs(texVarying.y)) * vec4(1, 1, 1, 1);
}

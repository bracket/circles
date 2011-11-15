uniform float phase, phase_derivative;

varying vec2 texture_varying;

float wrap(float x) {
	return x - floor(x);
}

vec2 dir(float phase) {
	return vec2(cos(phase), sin(phase));
}

float linstep(float edge0, float edge1, float x) {
	float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return mix(0., 1., t);
}

float pi = 4.0 * atan(1.0),
	max_lag = 2.0 * pi * (5.0 / 6.0),
	slice_length = 2.0 * pi / 40.
	;

vec4 circle() {
	float r = length(texture_varying);
	if (r > 1.0) { discard; }

	float clamped_phase = mod(floor(phase / slice_length) * slice_length + .5 * slice_length, 2.0 * pi),
		lag = max_lag * max(log2(phase_derivative), 0.0) / 12.0,
		slice_count = max(floor(lag / slice_length), 1.0),
		lag_phase = clamped_phase - slice_count * slice_length;
	

	float theta = atan(texture_varying.y, texture_varying.x);
	if (theta < 0. && theta + 2.0 * pi < clamped_phase) { theta += 2.0 * pi; }
	else if (theta > 0. && theta > clamped_phase) { theta -= 2.0 * pi; }
	
	float alpha = smoothstep(clamped_phase, clamped_phase - .01, theta)
		- floor(slice_count * linstep(clamped_phase, lag_phase, theta)) / slice_count;

	vec3 lightest = mix(vec3(1), vec3(1,0,0), .15),
		block_color = mix(lightest, vec3(1,0,0), alpha);

	float on_line = 1.0;
	for (int i = 0; i < 20; ++i) {
		vec2 n = dir(float(i) * slice_length);
		on_line = min(on_line,
			max(
				smoothstep(.01, .03, abs(dot(n, texture_varying))),
				max(smoothstep(.90, .91, r), smoothstep(.75, .74, r))
			)
		);
	}

	return mix(vec4(block_color, 1), vec4(0, 0, 0, 0), on_line);
}

float max_sine_length = .65, max_sine_height = .68 * .65;

vec4 sine_wave() {
	if (texture_varying.x < -max_sine_length) { return vec4(0); }
	if (texture_varying.x > max_sine_length) { return vec4(0); } 

	float x = 2.0 * pi * linstep(-max_sine_length, max_sine_length, texture_varying.x),
		y = max_sine_height * sin(x);

	return mix(
		vec4(1, 0, 0, 1),
		vec4(0),
		smoothstep(.01, .02, abs(texture_varying.y - y))
	);
}

void main() {
	gl_FragColor = circle() + sine_wave();
}

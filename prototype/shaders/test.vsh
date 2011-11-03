uniform float time;

attribute vec3 position;
attribute vec2 texture;

varying vec2 texture_varying;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0);
	texture_varying =  texture;
}

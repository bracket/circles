uniform mat4 projection;
uniform mat4 model_view;

attribute vec4 position;
attribute vec2 tex;

varying vec2 texture_varying;

void main() {
	gl_Position = projection * model_view * position;
	texture_varying =  tex;
}

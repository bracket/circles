attribute vec4 position;
attribute vec2 tex;

uniform mat4 projection;
uniform mat4 model_view;

varying vec2 texVarying;

void main() {
	gl_Position = projection * model_view * position;
	texVarying = tex;
}

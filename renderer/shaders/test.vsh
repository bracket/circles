attribute vec4 position;
attribute vec4 color;

uniform mat4 projection;
uniform mat4 model_view;

varying vec4 colorVarying;

void main() {
	gl_Position = projection * model_view * position;

	colorVarying = color;
}

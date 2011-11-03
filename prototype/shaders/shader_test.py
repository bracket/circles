import sys, time, math;
from itertools import chain;

import numpy;
from numpy import array;

from OpenGL.GL import *;
from OpenGL.GLU import *;
from OpenGL.GLUT import *;

from OpenGL.GL import shaders;
import time;

class TestSquare(object):
	def __init__(self, vsh_path, fsh_path):
		self.program = shaders.compileProgram(
			self.make_vertex_shader(vsh_path),
			self.make_fragment_shader(fsh_path)
		);

		width = 640.0 / 480.0;

		self.vertices = numpy.array(
			list(chain(
				[ -.5, -.5, 0 ],
				[ -.5,  .5, 0 ],
				[  .5, -.5, 0 ],
				[  .5,  .5, 0 ],
			)),
			dtype = numpy.float32
		);

		self.textures = numpy.array(
			list(chain(
				[ -1, -1 ],
				[ -1, 1 ],
				[ 1, -1 ],
				[ 1, 1 ],
			)),
			dtype = numpy.float32
		);

		self.pos_idx = glGetAttribLocation(self.program, "position");
		self.tex_idx = glGetAttribLocation(self.program, "texture");

		self.start = time.time();
		self.last_time = 0.0;
		self.phase = 0.0;
		self.phase_derivative = .25;

		self.phase_idx = glGetUniformLocation(self.program, "phase");
		self.phase_derivative_idx = glGetUniformLocation(self.program, "phase_derivative");

	def make_vertex_shader(self, vsh_path):
		return shaders.compileShader(open(vsh_path, 'r').read(), GL_VERTEX_SHADER);

	def make_fragment_shader(self, fsh_path):
		return shaders.compileShader(open(fsh_path, 'r').read(), GL_FRAGMENT_SHADER);

	def draw(self):
		glUseProgram(self.program);

		glVertexAttribPointer(
			self.pos_idx,
			3, 
			GL_FLOAT,
			GL_FALSE,
			0,
			self.vertices
		);

		glVertexAttribPointer(
			self.tex_idx,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			self.textures
		);

		glEnableVertexAttribArray(self.pos_idx);
		glEnableVertexAttribArray(self.tex_idx);

		t = (time.time() - self.start)
		t_delta = t - self.last_time;
		self.last_time = t;

		# note phase is in radians, but phase_derivative is in hz
		self.phase_derivative = .1 * 2**(t / 4.0);

		f = math.log(self.phase_derivative) / math.log(2);

		if f <= math.log(.5) / math.log(2):
			self.phase += t_delta * 2.0 * math.pi * self.phase_derivative;
		else:
			self.phase += t_delta * 2.0 * math.pi * .5 * 2.**(f % 1.)

		glUniform1f(self.phase_idx, self.phase);
		glUniform1f(self.phase_derivative_idx, self.phase_derivative);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)
	
square = None;

def main():
	width = 640; height = 480;
	glutInit(sys.argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("sexy skexy");

	global square;
	square = TestSquare(sys.argv[1], sys.argv[2]);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
	gluPerspective(90, width / float(height), 1, 1e3);

	glutDisplayFunc(display);
	glutKeyboardFunc(key_event);
	glutMainLoop();

def key_event(key, x, y):
	global square;
	try:
		square = TestSquare(sys.argv[1], sys.argv[2]);
	except RuntimeError as e:
		print "\n".join(map(str, e.args));

def display():
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslate(0, 0, -1);
	square.draw();
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();

if __name__ == "__main__": main();

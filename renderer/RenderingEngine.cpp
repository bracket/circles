#include <cmath>
#include <OpenGLES/ES2/gl.h>
#include <math/MatrixOps.hpp>
#include <renderer/RenderingEngine.hpp>

bool RenderingEngine::init() {

	GLuint frame_buffer, render_buffer;
	glGenFramebuffers(1, &frame_buffer);
	glGenRenderbuffers(1, &render_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);


	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_RENDERBUFFER, render_buffer
	);

	glViewport(0, 0, view_width_, view_height_);

	set_projection_matrix(perspective(
		1.0f, 10.0f,
		view_width_ / view_height_,
		static_cast<float>(2 * std::atan(1))
	));

	set_camera_matrix(identity_matrix<float>());

	return true;
}

void RenderingEngine::render() {
	typedef RenderMap::iterator program_iterator;
	typedef RenderList::iterator iterator;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (program_iterator pit = render_map_.begin(); pit != render_map_.end(); ++pit) {
		pit->first->bind();
		pit->first->bind_uniform("projection", projection_matrix_);

		iterator it = pit->second.begin(), end = pit->second.end();
		for (; it != end; ++it) { (*it)->render(this); }
		pit->second.clear();
	}
}

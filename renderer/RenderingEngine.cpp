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

	camera_.set_projection_matrix(
		1.0f, 10.0f,
		view_width_ / view_height_,
		static_cast<float>(2 * std::atan(1))
	);

	float C[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 5, 1,
	};

	camera_.set_camera_matrix(Matrix<4, 4, float>(C));

	return true;
}

void RenderingEngine::render() {
	typedef RenderMap::iterator program_iterator;
	typedef RenderList::iterator iterator;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (program_iterator pit = render_map_.begin(); pit != render_map_.end(); ++pit) {
		pit->first->bind();
		pit->first->bind_uniform("projection", camera_.get_projection_matrix());

		iterator it = pit->second.begin(), end = pit->second.end();
		for (; it != end; ++it) { (*it)->render(this); }
		pit->second.clear();
	}
}

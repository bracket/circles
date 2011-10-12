#pragma once

#include <math/MatrixOps.hpp>
#include <math/Rectangle.hpp>
#include <math/Vec.hpp>
#include <renderer/Vertex.hpp>
#include <vector>

class Program;
class RenderingEngine;

struct Square {
	typedef std::vector<Vertex> VertexContainer;

	Square(Program * program);

	void render(RenderingEngine const * engine, Renderable::FrameType const & f);

	VertexContainer const & get_vertices() const { return vertices_; }

	private:
		Program * program_;
		std::vector<Vertex> vertices_;
		GLuint pos_idx_, tex_idx_;
};

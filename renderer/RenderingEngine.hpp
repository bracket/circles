#pragma once

#include <boost/unordered_map.hpp>
#include <memory>
#include <renderer/Program.hpp>
#include <renderer/Renderable.hpp>
#include <vector>

class RenderingEngine {
	public:
		typedef std::vector<Renderable*> RenderList;
		typedef boost::unordered_map<Program*, RenderList> RenderMap;

		static RenderingEngine * construct() {
			std::auto_ptr<RenderingEngine> ptr(new RenderingEngine());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		void submit(Renderable * renderable) {
			Program * program = renderable->get_program();
			render_map_[program].push_back(renderable);
		}

		void render();
	
	private:
		RenderingEngine() { }
		
		bool init();

		RenderMap render_map_;
};

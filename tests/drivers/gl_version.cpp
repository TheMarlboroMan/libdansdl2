#include "../../include/ldv/screen.h"
#include "../../include/ldv/sdl_video_tools.h"
#include "../../include/ldi/sdl_input.h"
#include "../../include/ldt/sdl_tools.h"
#include "../class/gl_version.h"
#include <iostream>

int main(int /*argc*/, char ** /*argv*/) {

	ldt::sdl_init();

	//Basic skeleton: video and input...
	ldv::screen screen(64, 64);
	ldi::sdl_input input;

	ldtest::gl_version test;

	std::cout<<"opengl version: "<<ldv::get_opengl_version()<<std::endl;
	test.run(screen, input);

	ldt::sdl_shutdown();

	return 0;
}

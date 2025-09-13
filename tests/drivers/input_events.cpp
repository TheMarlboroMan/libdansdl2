#include "../../include/ldv/screen.h"
#include "../../include/ldi/sdl_input.h"
#include "../../include/ldt/sdl_tools.h"
#include "../../include/ldt/log.h"
#include "../class/input_events.h"

int main(int /*argc*/, char ** /*argv*/) {

	ldt::log_lsdl::set_type(ldt::log_lsdl::types::out);
	ldt::sdl_init();

	//Basic skeleton: video and input...
	ldv::screen screen(64, 64);
	ldi::sdl_input input;

	ldtest::input_events test;
	test.run(screen, input);

	ldt::sdl_shutdown();

	return 0;
}

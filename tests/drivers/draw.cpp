#include "../../include/ldv/screen.h"
#include "../../include/ldi/sdl_input.h"
#include "../../include/ldt/sdl_tools.h"

#include "../class/draw.h"

int main(
	int /*argc*/, 
	char ** /*argv*/
) {

	ldt::sdl_init();

	//Basic skeleton: video and input...
	ldv::screen screen(500, 500);
	ldi::sdl_input input;

	ldtest::draw test;
	test.run(screen, input);

	ldt::sdl_shutdown();

	return 0;
}

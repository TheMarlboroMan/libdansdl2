#include "../../include/ldv/screen.h"
#include "../../include/ldi/sdl_input.h"
#include "../../include/ldt/sdl_tools.h"
#include "../class/input_events.h"

/*
g++ tests/drivers/input_events.cpp -o input_events.out -ansi -pedantic -ldansdl2 -llm  -lSDL2 -lSDL2_mixer -lGL -Wall -Wextra -fno-rtti --std=c++11 
*/

int main(int /*argc*/, char ** /*argv*/) {

	ldt::sdl_init();

	//Basic skeleton: video and input...
	ldv::screen screen(64, 64);
	ldi::sdl_input input;

	ldtest::input_events test;
	test.run(screen, input);

	ldt::sdl_shutdown();

	return 0;
}

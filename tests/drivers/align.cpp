/*
This program will test the text representation aligment capabilities.
*/

//TODO: These are (and always will be) horrible. Add EVERYTHING to a single namespace thingy.
#include "../../tools/sdl_tools/sdl_tools.h"
#include "../../video/screen/screen.h"
#include "../../input/sdl_input/sdl_input.h"
#include "../class/align.h"

/*
//TODO: We should have a script to help output this sort of thing...
g++ tests/drivers/align.cpp -o align.out -ansi -pedantic -I ../log/ -L ./ -L ../log/ -lDanSDL2 -llog  -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lGL -Wall -Wextra -fno-rtti --std=c++11 
*/

int main(int /*argc*/, char ** /*argv*/) {

	ldt::sdl_init();

	//Basic skeleton: video and input...
	//TODO: This motherfucker should be called "window".
	ldv::screen screen(600, 400);
	//TODO: Should just be named "input".
	ldi::sdl_input input;

	//TODO: Perhaps test should inherit from something.
	ldtest::align_test test;
	test.run(screen, input);


	return 0;
}

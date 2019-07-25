#include <vector>

/*
This program will test the text representation aligment capabilities.
*/

//TODO: These are (and always will be) horrible. Add EVERYTHING to a single namespace thingy.
#include "../tools/sdl_tools/sdl_tools.h"

#include "../video/screen/screen.h"
#include "../input/sdl_input/sdl_input.h"
#include "../video/representation/primitive/box/box_representation.h"
#include "../video/ttf_font/ttf_font.h"
#include "../video/representation/raster/ttf/ttf_representation.h"

/*
//TODO: We should have a script to help output this sort of thing...
g++ tests/align.cpp -o align.out -ansi -pedantic -I ../log/ -L ./ -L ../log/ -lDanSDL2 -llog  -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lGL -Wall -Wextra -fno-rtti --std=c++11 
 -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual  --std=c++11
*/

struct pair {
	ldv::representation_alignment::h			h;
	ldv::representation_alignment::v			v;
};

std::vector<pair>	generate_pairs();

int main(int /*argc*/, char ** /*argv*/) {

	ldt::sdl_init();

	//Basic skeleton: video and input...
	//TODO: This motherfucker should be called "window".
	ldv::screen screen(600, 400);
	ldi::sdl_input input;

	//Prepare representations...
	ldv::box_representation frame{
		{0, 0, 300, 200},
		ldv::rgba8(255,255,255,255),
	//TODO: The whole fill/line is horrid too.
		ldv::polygon_representation::type::line
	};

	frame.align(
		screen.get_rect(), {
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::center
		}
	);

	ldv::box_representation box{
		{0,0,20,20},
		ldv::rgba8(255,255,255,128)
	};

	ldv::ttf_font font("akashi.ttf", 14);
	ldv::ttf_representation text(font, ldv::rgba8(255,0,255,128), "");
	text.set_text("This is\na multiline string\nfor you");

	auto types=generate_pairs();

	size_t index=0;

	//Absurdist loop...
	while(true) {

		input.loop();
		if(input.is_exit_signal() || input.is_key_down(SDL_SCANCODE_ESCAPE)) {
			break;
		}

		if(input.is_key_down(SDL_SCANCODE_SPACE)) {
			++index;
			if(index >= types.size()) {
				index=0;
			}
		}

		if(input.is_key_down(SDL_SCANCODE_RETURN)) {
			switch(text.get_text_align()) {
				case ldv::ttf_representation::text_align::left: text.set_text_align(ldv::ttf_representation::text_align::center); break;
				case ldv::ttf_representation::text_align::center: text.set_text_align(ldv::ttf_representation::text_align::right); break;
				case ldv::ttf_representation::text_align::right: text.set_text_align(ldv::ttf_representation::text_align::left); break;
			}
		}

		//TODO: Perform alignment...
		box.align(frame, ldv::representation_alignment(types[index].h, types[index].v));
		text.align(frame, ldv::representation_alignment(types[index].h, types[index].v));

		screen.clear(ldv::rgba8(0, 0, 0, 255));
		frame.draw(screen);
		box.draw(screen);
		text.draw(screen);
		screen.update();

	}

	return 0;
}

std::vector<pair>	generate_pairs() {

	std::vector<ldv::representation_alignment::h> ha={
		ldv::representation_alignment::h::outer_left, 
		ldv::representation_alignment::h::inner_left, 
		ldv::representation_alignment::h::center,
		ldv::representation_alignment::h::inner_right, 
		ldv::representation_alignment::h::outer_right
	};

	std::vector<ldv::representation_alignment::v> va={
		ldv::representation_alignment::v::outer_top, 
		ldv::representation_alignment::v::inner_top, 
		ldv::representation_alignment::v::center,
		ldv::representation_alignment::v::inner_bottom, 
		ldv::representation_alignment::v::outer_bottom
	};

	std::vector<pair> types;
	for(const auto& h : ha) {
		for(const auto& v : va) {
			types.push_back({h, v});
		}
	}

	return types;
}

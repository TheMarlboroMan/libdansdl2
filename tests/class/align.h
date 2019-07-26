#include <vector>

//TODO: These are (and always will be) horrible. Add EVERYTHING to a single namespace thingy.
#include "../../video/screen/screen.h"
#include "../../input/sdl_input/sdl_input.h"
#include "../../video/representation/primitive/box/box_representation.h"
#include "../../video/ttf_font/ttf_font.h"
#include "../../video/representation/raster/ttf/ttf_representation.h"

namespace ldtest {

//TODO: Should implement some shitty interface.
class align_test {
	
	public:

	struct 				pair {
		ldv::representation_alignment::h			h;
		ldv::representation_alignment::v			v;
	};

	std::vector<pair>	types;

						align_test():
		types{generate_pairs()} {

	}

	void 				run(ldv::screen& _screen, ldi::sdl_input& _input) {

		//Preparing test resources...
		ldv::box_representation frame{
			{0, 0, 300, 200},
			ldv::rgba8(255,255,255,255),
		//TODO: The whole fill/line is horrid too.
			ldv::polygon_representation::type::line
		};

		frame.align(
			_screen.get_rect(), {
				ldv::representation_alignment::h::center,
				ldv::representation_alignment::v::center
			}
		);

		ldv::box_representation box{
			{0,0,20,20},
			ldv::rgba8(255,255,255,128)
		};

		ldv::ttf_font font("tests/resources/alcubierre.otf", 14);
		ldv::ttf_representation text(font, ldv::rgba8(255,0,255,128), "");
		text.set_text("This is\na multiline string\nfor you");

		size_t index=0;

		//Absurdist loop...
		while(true) {

			_input.loop();
			if(_input.is_exit_signal() || _input.is_key_down(SDL_SCANCODE_ESCAPE)) {
				break;
			}

			if(_input.is_key_down(SDL_SCANCODE_SPACE)) {
				index=next_index(index);
			}

			if(_input.is_key_down(SDL_SCANCODE_RETURN)) {
				text.set_text_align(next_alignment(text.get_text_align()));
			}

			box.align(frame, ldv::representation_alignment(types[index].h, types[index].v));
			text.align(frame, ldv::representation_alignment(types[index].h, types[index].v));

			_screen.clear(ldv::rgba8(0, 0, 0, 255));
			frame.draw(_screen);
			box.draw(_screen);
			text.draw(_screen);
			_screen.update();
		}
	}

	ldv::ttf_representation::text_align	next_alignment(ldv::ttf_representation::text_align _align) {

		switch(_align) {
			case ldv::ttf_representation::text_align::left: 	return ldv::ttf_representation::text_align::center;
			case ldv::ttf_representation::text_align::center: 	return ldv::ttf_representation::text_align::right;
			case ldv::ttf_representation::text_align::right: 	return ldv::ttf_representation::text_align::left;
		}

		return ldv::ttf_representation::text_align::center;
	}

	unsigned			next_index(unsigned _index) {

		++_index;
		if(_index >= types.size()) {
			return 0;
		}

		return _index;
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

		std::vector<pair> res;
		for(const auto& h : ha) {
			for(const auto& v : va) {
				res.push_back({h, v});
			}
		}

		return res;
	}
};

}

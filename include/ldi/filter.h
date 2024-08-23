#pragma once

#include "sdl_input.h"

namespace ldi {

//!Simplified event that can be polled from get_event methods. These are
//!extracted from the internal buffers of the ldi::sld_input class so their
//!order is fixed.
struct event {

	enum codes{c_exit=1};
	enum sources {
		source_none=0, 
		source_mouse=1, 
		source_keyboard=2, 
		source_joystick=4, 
		source_system=8,
		source_any=15
	};
	enum types {
		type_none=0, 
		type_up=1, 
		type_down=2, 
		type_system=4,
		type_any=7
	};

	int source{0}, //origin of event.
		type{0},
		code{0}, //Code of the event (button, key...).
		source_index{0};//Index of the source, used for joysticks

	operator bool() const {return 0!=source;}
};

//input filter
class filter {

	public:

	        filter(const sdl_input&);

	//!Searches the input state for an event of the given source(s) and type(s)
	//!The first code and source index found is returned. Source events are
	//!searched in a fixed order: system, keyboard, mouse, joystick.
	event   find_one(int=15, int=7) const;

	private:

	const sdl_input&  input;

	event   find_one_system() const;
	event   find_one_keyboard(int=7) const;
	event   find_one_mouse(int=7) const;
	event   find_one_joystick(int=7) const;
	
};
}

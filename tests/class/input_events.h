#include <iostream>
#include <string>

#include "../../include/ldi/filter.h"

namespace ldtest {

class input_events {
	
	public:

	void 				run(ldv::screen& _screen, ldi::sdl_input& _input) {

		std::cout<<"watch the stdout for events..."<<std::endl;

		for(std::size_t i=0; i<_input.get_joysticks_size(); i++) {

			_input.virtualize_joystick_hats(i); 
			_input.virtualize_joystick_axis(i, 15000); 
		}

		auto type_to_str=[](int _type) {

			switch(_type) {
				case ldi::event::types::type_none: return "none";
				case ldi::event::types::type_up: return "up";
				case ldi::event::types::type_down: return "down";
				case ldi::event::types::type_system: return "system";
			}

			return "???";
		};

		ldi::filter filter{_input};

		while(true) {

			_input.loop();

			if(_input.is_event_joystick_connected()) {

				std::cout<<"joystick connected, virtualizing..."<<std::endl;
				for(std::size_t i=0; i<_input.get_joysticks_size(); i++) {

					_input.virtualize_joystick_hats(i); 
					_input.virtualize_joystick_axis(i, 15000); 
				}
			}

			if(_input.is_exit_signal() || _input.is_key_down(SDL_SCANCODE_ESCAPE)) {

				break;
			}

			auto ev=filter.find_one();
			if(!ev) {

				_screen.clear(ldv::rgba8(0, 0, 0, 255));
				_screen.update();
				continue;
			}

			switch(ev.source) {

				case ldi::event::sources::source_keyboard:
					std::cout<<"keyboard key "<<type_to_str(ev.type)<<" "<<ev.code<<std::endl;
				break;
				case ldi::event::sources::source_mouse:
					std::cout<<"mouse button "<<type_to_str(ev.type)<<" "<<ev.code<<std::endl;
				break;
				case ldi::event::sources::source_joystick:
					std::cout<<"joystick "<<ev.source_index<<" button "<<type_to_str(ev.type)<<" "<<ev.code<<std::endl;
				break;
			}

			_screen.clear(ldv::rgba8(0, 128, 0, 255));
			_screen.update();
		}
	}
};

}

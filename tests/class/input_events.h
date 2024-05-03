#include <iostream>
#include <string>

namespace ldtest {

class input_events {
	
	public:

	void 				run(ldv::screen& _screen, ldi::sdl_input& _input) {

		std::cout<<"watch the std out for events..."<<std::endl;

		auto type_to_str=[](ldi::sdl_input::event::types _type) {

			switch(_type) {
				case ldi::sdl_input::event::types::none: return "none";
				case ldi::sdl_input::event::types::up: return "up";
				case ldi::sdl_input::event::types::down: return "down";
				case ldi::sdl_input::event::types::system: return "system";
			}

			return "???";
		};

		while(true) {

			_input.loop();
			if(_input.is_exit_signal() || _input.is_key_down(SDL_SCANCODE_ESCAPE)) {

				break;
			}

			auto ev=_input.get_event();
			if(ev.source==ldi::sdl_input::event::sources::none) {

				_screen.clear(ldv::rgba8(0, 0, 0, 255));
				_screen.update();
				continue;
			}

			switch(ev.source) {

				case ldi::sdl_input::event::sources::keyboard:
					std::cout<<"keyboard key "<<type_to_str(ev.type)<<" "<<ev.code<<std::endl;
				break;
				case ldi::sdl_input::event::sources::mouse:
					std::cout<<"mouse button "<<type_to_str(ev.type)<<" "<<ev.code<<std::endl;
				break;
				case ldi::sdl_input::event::sources::joystick:
					std::cout<<"joystick "<<ev.source_index<<" button "<<type_to_str(ev.type)<<" "<<ev.code<<std::endl;
				break;
			}

			_screen.clear(ldv::rgba8(0, 128, 0, 255));
			_screen.update();
		}
	}
};

}

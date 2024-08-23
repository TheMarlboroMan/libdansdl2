#include <iostream>

namespace ldtest {

class gl_version {
	
	public:

	void 				run(ldv::screen& _screen, ldi::sdl_input& _input) {

		std::cout<<"press escape to exit"<<std::endl;

		while(true) {

			_input.loop();

			if(_input.is_exit_signal() || _input.is_key_down(SDL_SCANCODE_ESCAPE)) {

				break;
			}

			_screen.clear(ldv::rgba8(0, 0, 0, 255));
			_screen.update();
		}
	}
};

}

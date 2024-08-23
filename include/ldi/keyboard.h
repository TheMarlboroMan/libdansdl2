#pragma once

#include <SDL2/SDL.h>
#include <array>

namespace ldi {

//!Private structure representing the keyboard layout and state.
class keyboard {

	public:
				keyboard();
	void        init_keys();
	void        reset_keys();

	std::array<bool, SDL_NUM_SCANCODES>	keys_up,
						keys_down,
						keys_pressed,
						keys_released;
	int                 keys_pressed_size; //This goes up when a key down and down when a key up. As long as it is larger than zero there are keypresses.
};

}

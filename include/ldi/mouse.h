#pragma once

#include <SDL2/SDL.h>
#include <array>

namespace ldi {

//!Represents the mouse position.
struct mouse_position {
	int x=0,	//!< X position for the mouse.
		y=0;	//!< Y position for the mouse.
};

//!Private structure to interact with mouse parameters.

class mouse {

	public:	//Public porque esto si se devuelve es como "const" y es más cómodo que andar con accedentes a este nivel.

	mouse_position  position;

	int             get_x() const {return position.x;}
	int             get_y() const {return position.y;}
	const mouse_position&	get_position() const {return position;}

	bool            movement;

	static const unsigned int max_buttons=6;
	std::array<bool, max_buttons>	buttons_up,
					buttons_down,
					buttons_pressed,
					buttons_released;

	mouse();
	void init();
};

}

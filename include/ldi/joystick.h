#pragma once

#include <lm/log.h>
#include <SDL2/SDL.h>
#include <vector>
#include <ostream>

namespace ldi {

//!Private structure to interact with joysticks and controllers.
class joystick {

	public:

	typedef 	std::vector<bool> vbuttons;
	typedef 	std::vector<Sint16> vaxis;
	typedef 	std::vector<int> vhats;

	SDL_Joystick * 	structure;
	SDL_JoystickID	id;
	unsigned int	device_id,
			buttons,
			axis_size,
			hats_size;
	vbuttons 	buttons_up,
			buttons_down,
			buttons_pressed,
			buttons_released;
	vaxis		axis;
	vhats		hats;
	size_t 		virtualized_hats,
			virtualized_axis;
	int		threshold_virtual_axis_button;

	joystick(SDL_JoystickID pid, int pdevice_id);
	~joystick();

	//Joysticks are inside a map that copies the object so it
	//would be deleted and freed if emplaced in the constructor.
	//So far we'll just pass the structure after construction.

	void init(SDL_Joystick * joy);

	//Hats are treated as buttons.
	void virtualize_hats();
	void virtualize_axis(int virtual_threshold);
	//true is down, false is up.
	void register_button(bool, unsigned int v_button);
	void register_axis(unsigned int v_axis, Sint16 v_value);
	void register_hat(unsigned int v_hat, int v_value);
	void init_state();
/**
 * Outputs the state of this joystick as a series of digits, representing 
 * the up, down, pressed and released buttons (0 means nothing, 1 means present)
 * separated by a space and preceded by their label (up: down: pressed: released:).
 */
	void debug_state(std::ostream&) const;
	void debug_state(lm::logger&) const;
};
}

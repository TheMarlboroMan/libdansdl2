#include "ldi/joystick.h"
#include <algorithm>

using namespace ldi;

joystick::joystick(
	SDL_JoystickID pid, 
	int pdevice_id
)
	:structure(nullptr), id(pid), device_id(pdevice_id), 
	buttons(0), axis_size(0), hats_size(0),
	virtualized_hats(0), virtualized_axis(0),
	threshold_virtual_axis_button(0)
{ }

joystick::~joystick() {

	if(structure) {

		SDL_JoystickClose(structure);
	}
}

void joystick::init(SDL_Joystick * joy) {

	structure=joy;
	buttons=SDL_JoystickNumButtons(structure);
	axis_size=SDL_JoystickNumAxes(structure);
	hats_size=SDL_JoystickNumHats(structure);

	if(buttons) {

		buttons_up.resize(buttons, false);
		buttons_down.resize(buttons, false);
		buttons_pressed.resize(buttons, false);
		buttons_released.resize(buttons, false);
	}

	if(axis_size) {

		axis.resize(axis_size, 0);
	}

	if(hats_size) {

		hats.resize(hats_size, 0);
	}

	init_state();

	//Warning: this should not be moved to "init_state":
	//if done we'd lose the axis movement or pressed buttons.

	if(buttons) {

		buttons_up.insert(std::begin(buttons_up), buttons, false);
		buttons_pressed.insert(std::begin(buttons_pressed), buttons, false);
		buttons_down.insert(std::begin(buttons_down), buttons, false);
		buttons_released.insert(std::begin(buttons_released), buttons, true);
	}

	if(axis_size) {

		axis.insert(std::begin(axis), axis_size, 0);
	}

	if(hats_size) {

		hats.insert(std::begin(hats), hats_size, SDL_HAT_CENTERED);
	}
}

void joystick::virtualize_hats() {

	if(virtualized_hats) return;
	int nbuttons=4 * hats_size;

	buttons_up.insert(std::end(buttons_up), nbuttons, false);
	buttons_pressed.insert(std::end(buttons_pressed), nbuttons, false);
	buttons_down.insert(std::end(buttons_down), nbuttons, false);
	buttons_released.insert(std::end(buttons_released), nbuttons, true);

	virtualized_hats=buttons; //First virtualised hat index.
	buttons+=nbuttons;
}

void joystick::virtualize_axis(
	int virtual_threshold
) {

	if(virtualized_axis) return;

	threshold_virtual_axis_button=virtual_threshold;
	int nbuttons=2 * axis_size;

	buttons_up.insert(std::end(buttons_up), nbuttons, false);
	buttons_pressed.insert(std::end(buttons_pressed), nbuttons, false);
	buttons_down.insert(std::end(buttons_down), nbuttons, false);
	buttons_released.insert(std::end(buttons_released), nbuttons, true);

	virtualized_axis=buttons; //First virtualised axis index.
	buttons+=nbuttons;
}

void joystick::register_button(
	unsigned int v_tipo, 
	unsigned int v_button
) {

	if(v_tipo==0) {

		buttons_down[v_button]=true;
		buttons_pressed[v_button]=true;
		buttons_released[v_button]=false;
	}
	else {

		buttons_up[v_button]=true;
		buttons_released[v_button]=true;
		buttons_pressed[v_button]=false;
	}
}

void joystick::register_axis(
	unsigned int v_axis, 
	Sint16 v_value
) {

	axis[v_axis]=v_value;

	if(virtualized_axis) {

		size_t index=virtualized_axis + (2 * v_axis);

		if(abs(v_value) > threshold_virtual_axis_button) {

			std::fill(std::begin(buttons_down)+index, std::begin(buttons_down)+index+2, false);
			std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+2, false);
			std::fill(std::begin(buttons_released)+index, std::begin(buttons_released)+index+2, true);

			auto f=[this](size_t v_button) {

				buttons_down[v_button]=true;
				buttons_pressed[v_button]=true;
				buttons_released[v_button]=false;
			};

			if(v_value > 0) f(index);
			else f(index+1);
		}
		else {

			auto f=[this](size_t v_button) {

				if(buttons_pressed[v_button]) {

					buttons_released[v_button]=true;
					buttons_up[v_button]=true;
				}
			};

			f(index);
			f(index+1);

			std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+2, false);
		}
	}
}

void joystick::register_hat(
	unsigned int v_hat, 
	int v_value
) {

	hats[v_hat]=v_value;

	if(virtualized_hats) {

		size_t index=virtualized_hats + (4 * v_hat);

		if(v_value==SDL_HAT_CENTERED) {

			for(size_t helper=0; helper < 4; ++helper) {

				if(buttons_pressed[index+helper]) {

					buttons_released[index+helper]=true;
					buttons_up[index+helper]=true;
				}
			}

			std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+4, false);
		}
		else {

			std::fill(std::begin(buttons_down)+index, std::begin(buttons_down)+index+4, false);
			std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+4, false);
			std::fill(std::begin(buttons_released)+index, std::begin(buttons_released)+index+4, true);

			auto f=[this](size_t v_button) {
				buttons_down[v_button]=true;
				buttons_pressed[v_button]=true;
				buttons_released[v_button]=false;
			};

			if(v_value & SDL_HAT_UP) f(index);
			if(v_value & SDL_HAT_RIGHT) f(index+1);
			if(v_value & SDL_HAT_DOWN) f(index+2);
			if(v_value & SDL_HAT_LEFT) f(index+3);
		}
	}
}

void joystick::init_state() {

	if(buttons) {

		std::fill(std::begin(buttons_up), std::end(buttons_up), false);
		std::fill(std::begin(buttons_down), std::end(buttons_down), false);
	}
}

///////////////


//////////////



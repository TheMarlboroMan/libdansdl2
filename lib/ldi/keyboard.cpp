#include "ldi/keyboard.h"
#include <algorithm>

using namespace ldi;

keyboard::keyboard()
	:keys_pressed_size{0}
{}

void keyboard::init_keys() {

	auto f=[](std::array<bool, SDL_NUM_SCANCODES>& a) {std::fill(std::begin(a), std::end(a), false);};
	f(keys_up);
	f(keys_down);
	f(keys_pressed);
	f(keys_released);
}

void keyboard::reset_keys() {

	auto f=[](std::array<bool, SDL_NUM_SCANCODES>& a) {std::fill(std::begin(a), std::end(a), false);};
	f(keys_up);
	f(keys_down);
}

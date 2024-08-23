#include "ldi/mouse.h"
#include <algorithm>

using namespace ldi;

mouse::mouse()
	:position(),
	movement(false) {

	std::fill(std::begin(buttons_up), std::end(buttons_up), false);
	std::fill(std::begin(buttons_down), std::end(buttons_down), false);
	std::fill(std::begin(buttons_pressed), std::end(buttons_pressed), false);
	std::fill(std::begin(buttons_released), std::end(buttons_released), false);
}

void mouse::init() {

	std::fill(std::begin(buttons_up), std::end(buttons_up), false);
	std::fill(std::begin(buttons_down), std::end(buttons_down), false);
	movement=false;
}

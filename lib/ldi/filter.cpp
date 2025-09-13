#include "ldi/filter.h"

using namespace ldi;

filter::filter(
	const sdl_input& _input
)
	:input{_input}
{}

event filter::find_one(
	int _source,
	int _type
) const {

	if(event::sources::source_system & _source) {

		auto ev=find_one_system();
		if(ev) {

			return ev;
		}
	}

	if(event::sources::source_keyboard & _source) {

		auto ev=find_one_keyboard(_type);
		if(ev) {

			return ev;
		}
	}

	if(event::sources::source_mouse & _source) {

		auto ev=find_one_mouse(_type);
		if(ev) {

			return ev;
		}
	}

	if(event::sources::source_joystick & _source) {

		auto ev=find_one_joystick(_type);
		if(ev) {

			return ev;
		}
	}

	return event{};
}

event filter::find_one_system() const {
 
	if(input.is_exit_signal()) {

		return event{event::sources::source_system, event::types::type_system, event::c_exit, 0};
	}

	if(input.is_event_activity()) {

		//TODO:
	}

	if(input.is_event_activity_focus()) {

		//TODO:
	}

	return event{};
}

event filter::find_one_keyboard(
	int _type
) const {

	//First the keydowns
	if((event::type_down & _type) && input.is_event_keyboard_down()) {

		return event{event::sources::source_keyboard, event::types::type_down, input.get_key_down_index(), 0};
	}
	
	///Then the keyups
	if((event::type_up & _type) && input.is_event_keyboard_up()) {

		return event{event::sources::source_keyboard, event::types::type_up, input.get_key_up_index(), 0};
	}

	return event{};
}

event filter::find_one_mouse(
	int _type
) const {

	if((event::type_down & _type) && input.is_event_mouse_button_down()) {

		return event{event::sources::source_mouse, event::types::type_down, input.get_mouse_button_down_index(), 0};
	}

	if((event::type_up & _type) && input.is_event_mouse_button_up()) {

		return event{event::sources::source_mouse, event::types::type_up, input.get_mouse_button_up_index(), 0};
	}
	
	return event{};
}

event filter::find_one_joystick(
	int _type
) const {

	//pointer to sdl_input method taking nothing, returning bool, which is const.
	bool (sdl_input::*input_event_type_ptr)()const=nullptr;
	//pointer to sdl_input method taking int, returning int, const again.
	int (sdl_input::*input_btn_index_ptr)(int)const=nullptr;
	auto type{event::types::type_down};

	if(event::type_down & _type) {

		input_event_type_ptr=&sdl_input::is_event_joystick_button_down;
		input_btn_index_ptr=&sdl_input::get_joystick_button_down_index;
		type=event::types::type_down;
	}
	else if(event::type_up & _type) {

		input_event_type_ptr=&sdl_input::is_event_joystick_button_up;
		input_btn_index_ptr=&sdl_input::get_joystick_button_up_index;
		type=event::types::type_up;
	}
	else {

		return event{};
	}

	if(! (input.*input_event_type_ptr)()) {

		return event{};
	}

	for(const auto& pair : input.joysticks) {

		int joy_index=pair.first;
		int btn_index=(input.*input_btn_index_ptr)(joy_index);
		if(-1==btn_index) {

			continue;
		}

		return event{
			event::sources::source_joystick, 
			type,
			btn_index,
			joy_index
		};
	}

	return event{};
}


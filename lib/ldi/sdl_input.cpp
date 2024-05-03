#include <ldi/sdl_input.h>

#include <ldt/log.h>

#include <lm/log.h>
#include <algorithm>

using namespace ldi;

//!Default constructor.
sdl_input::sdl_input():
	keydown_control_text_filter(false),
	exit_signal(false), joysticks_size(0)
{
	f_default_process_event=[this](SDL_Event& e){
		process_event(e);
	};

	reset_event_processing_function();
	SDL_StopTextInput();

	events_cache.resize(max_cache_index, false);

	device_keyboard.init_keys();
	init_joysticks();
}

//!Class destructor.
sdl_input::~sdl_input()
{
	joysticks.clear();
}

//!Starts all joysticks.
void sdl_input::init_joysticks()
{
	this->joysticks_size=SDL_NumJoysticks();

	if(this->joysticks_size > 0)
	{
		SDL_JoystickEventState(SDL_ENABLE);

		lm::log(ldt::log_lsdl::get()).info()<<"Located "<<joysticks_size<<" joysticks"<<std::endl;

		for(int i=0; i<joysticks_size; i++)
		{
			SDL_Joystick * joy=SDL_JoystickOpen(i);	//De alguna manera el valgrind saca aquí una pérdida.
			init_joystick(joy, i);
		}
	}
}

//!Individual joystick init subroutine.
void sdl_input::init_joystick(SDL_Joystick * estructura, int index)
{
	SDL_JoystickID id=SDL_JoystickInstanceID(estructura);
	joysticks.insert(std::pair<int, joystick>(index, joystick(id, index) ) ) ;
	joysticks.at(index).init(estructura);
	id_joystick_to_index[id]=index;

	auto& j=joysticks.at(index);

	lm::log(ldt::log_lsdl::get()).info()<<"Init joystick "<<index<<" with "<<
		j.buttons<<" buttons, "<<
		j.hats_size<<" hats, "<<
		j.axis_size<<" axis."<<std::endl;

}

//!Manually pumps an event into the first parameter, optionally processing it
//!if the second parameter is true.

//!Returns if there was an event to pump.

bool sdl_input::pump_events(SDL_Event &pevent, bool pprocess)
{
	bool result=SDL_PollEvent(&pevent);
	if(pprocess && result) {
		f_process_event(pevent, f_default_process_event);
	}

	return result;
}

void sdl_input::reset_event_processing_function()
{
	f_process_event=[this](SDL_Event& e, tf_default){
		process_event(e);
		return true;
	};
}

//!Main event loop. Must be called once per application tick.

void sdl_input::loop()
{
	clear_loop();
	SDL_Event sdl_event;
	while(SDL_PollEvent(&sdl_event)) {
		if(!f_process_event(sdl_event, f_default_process_event)) {
			return;
		}
	}
}

//!Called for each event, alters the internal structure of the class according to input.

void sdl_input::process_event(SDL_Event& _event) {

	switch(_event.type) {

		case SDL_QUIT:
			exit_signal=true;
		break;

		case SDL_MOUSEMOTION:
			events_cache[mousemove]=true;
			device_mouse.position.x=_event.motion.x;
			device_mouse.position.y=_event.motion.y;
			device_mouse.movement=true;
		break;

		case SDL_MOUSEBUTTONDOWN:
			events_cache[mousedown]=true;
			device_mouse.buttons_down[_event.button.button]=1;
			device_mouse.buttons_pressed[_event.button.button]=1;
			device_mouse.buttons_released[_event.button.button]=0;
		break;

		case SDL_MOUSEBUTTONUP:
			events_cache[mouseup]=true;
			device_mouse.buttons_up[_event.button.button]=1;
			device_mouse.buttons_pressed[_event.button.button]=0;
			device_mouse.buttons_released[_event.button.button]=1;
		break;

		case SDL_JOYBUTTONDOWN:
			events_cache[joystick_button_down]=true;
			joysticks.at(id_joystick_to_index[_event.jbutton.which]).register_button(0, _event.jbutton.button);
		break;

		case SDL_JOYBUTTONUP:
			events_cache[joystick_button_up]=true;
			joysticks.at(id_joystick_to_index[_event.jbutton.which]).register_button(1, _event.jbutton.button);
		break;

		case SDL_JOYAXISMOTION:
			if(_event.jaxis.value < -min_noise || _event.jaxis.value > max_noise)
			{
				events_cache[joystick_axis]=true;
				joysticks.at(id_joystick_to_index[_event.jbutton.which]).register_axis(_event.jaxis.axis, _event.jaxis.value);
			}
			else
			{
				joysticks.at(id_joystick_to_index[_event.jbutton.which]).register_axis(_event.jaxis.axis, 0);
			}

			if(joysticks.at(id_joystick_to_index[_event.jhat.which]).virtualized_axis)
			{
				set_virtualized_input();
			}
		break;

		case SDL_JOYHATMOTION:
			events_cache[joystick_hat]=true;
			joysticks.at(id_joystick_to_index[_event.jhat.which]).register_hat(_event.jhat.hat, _event.jhat.value);

			if(joysticks.at(id_joystick_to_index[_event.jhat.which]).virtualized_hats)
			{
				set_virtualized_input();
			}

		break;

		case SDL_JOYDEVICEADDED:
		case SDL_CONTROLLERDEVICEADDED:
			lm::log(ldt::log_lsdl::get()).info()<<"New joystick detected."<<std::endl;

			if(!is_joystick_registered_by_device_id(_event.cdevice.which))
			{
				init_joystick(SDL_JoystickOpen(_event.cdevice.which), joysticks.size());
				++joysticks_size;
				events_cache[joystick_connected]=true;
				lm::log(ldt::log_lsdl::get()).info()<<"Joystick registered"<<std::endl;
			}
			else
			{
				lm::log(ldt::log_lsdl::get()).info()<<"The joystick had been already registered."<<std::endl;
			}
		break;

		case SDL_JOYDEVICEREMOVED:
		case SDL_CONTROLLERDEVICEREMOVED:
			lm::log(ldt::log_lsdl::get()).info()<<"Joystick removal detected."<<std::endl;
			joysticks.erase(id_joystick_to_index[_event.cdevice.which]);
			--joysticks_size;
			events_cache[joystick_disconnected]=true;
		break;

		case SDL_TEXTINPUT:
//			if(SDL_IsTextInputActive())
//			{
				if(!f_text_filter || f_text_filter(_event)) {

					events_cache[text]=true;
					input_text+=_event.text.text;
				}
//			}
		break;

//		case SDL_ACTIVEEVENT:
//			this->event_actividad.recibir_input((bool) event.active.gain, event.active.state);
//		break;

		case SDL_KEYDOWN:
		{
			if(_event.key.repeat) {
				return;
			}

			unsigned int index=_event.key.keysym.scancode;

				events_cache[keyboard_down]=true;

				device_keyboard.keys_down[index]=true;
				device_keyboard.keys_pressed[index]=true;
				device_keyboard.keys_released[index]=true;

				++device_keyboard.keys_pressed_size;

			if(keydown_control_text_filter && SDL_IsTextInputActive())
			{
				switch(_event.key.keysym.sym)
				{
					case SDLK_BACKSPACE:
						if(input_text.length() > 0)
						{
							input_text.pop_back();
							events_cache[text]=true;
						}
					break;
					case SDLK_RETURN: input_text+="\n"; break;
				}
			}
		}
		break;

		case SDL_KEYUP:
		{
			unsigned int index=_event.key.keysym.scancode;

			events_cache[keyboard_up]=true;

			device_keyboard.keys_up[index]=true;
			device_keyboard.keys_released[index]=true;
			device_keyboard.keys_pressed[index]=false;

			--device_keyboard.keys_pressed_size;
		}
		break;

		default: break;
	}
}

//!Internally loops joystick state to update the event cache.

void sdl_input::set_virtualized_input()
{
	for(const auto& pj: joysticks)
	{
		const auto& j=pj.second;

		if(j.virtualized_hats || j.virtualized_axis)
		{
			if(std::any_of(std::begin(j.buttons_pressed), std::end(j.buttons_pressed), [](bool v) {return v;})) events_cache[joystick_button_up]=true;
			if(std::any_of(std::begin(j.buttons_down), std::end(j.buttons_down), [](bool v) {return v;})) events_cache[joystick_button_down]=true;
		}
	}
}

//!Checks if a joystick with the given device id is registered.

bool sdl_input::is_joystick_registered_by_device_id(unsigned int d_id)
{
	for(const auto& j : joysticks)
	{
		if(j.second.device_id==d_id) return true;
	}

	return false;
}

//!Internally resets joystick state.

void sdl_input::clear_joysticks_state()
{
	for(auto& p: joysticks) p.second.init_state();
}

//!Internally checks joystick button existence.

bool sdl_input::check_joystick_button(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!joysticks.size()) return false;
	else if(!joysticks.count(p_joystick)) return false;
	else if(pbutton > joysticks.at(p_joystick).buttons) return false;
	else return true;
}

//!Checks if joystick button is down.

bool sdl_input::is_joystick_button_down(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else
	{
		return joysticks.at(p_joystick).buttons_down[pbutton];
	}
}

//!Checks if joystick button is up.

bool sdl_input::is_joystick_button_up(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else
	{
		return joysticks.at(p_joystick).buttons_up[pbutton];
	}
}

//!Checks if joystick button is pressed.

bool sdl_input::is_joystick_button_pressed(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else
	{
		return joysticks.at(p_joystick).buttons_pressed[pbutton];
	}
}

//!Checks if joystick button is released.

bool sdl_input::is_joystick_button_released(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else
	{
		return joysticks.at(p_joystick).buttons_released[pbutton];
	}
}

//!Checks joystick axis existence.

bool sdl_input::check_joystick_axis(unsigned int p_joystick, unsigned int p_eje) const
{
	if(!joysticks.size()) return false;
	else if(joysticks.count(p_joystick)) return false;
	else if(p_eje > joysticks.at(p_joystick).axis_size) return false;
	else return true;
}

//!Checks joystick hat existence.

bool sdl_input::check_joystick_hat(unsigned int p_joystick, unsigned int p_hat) const
{
	if(!joysticks.size()) return false;
	else if(joysticks.count(p_joystick)) return false;
	else if(p_hat > joysticks.at(p_joystick).hats_size) return false;
	else return true;
}

//!Gets joystick axis value.

Sint16 sdl_input::get_joystick_axis(unsigned int p_joystick, unsigned int p_eje) const
{
	if(!this->check_joystick_axis(p_joystick, p_eje)) return false;
	else
	{
		return joysticks.at(p_joystick).axis[p_eje];
	}
}

//!Gets joystick hat value.

int sdl_input::get_joystick_hat(unsigned int p_joystick, unsigned int p_hat) const {

	if(!this->check_joystick_hat(p_joystick, p_hat)) {

		return false;
	}
	else {
		return joysticks.at(p_joystick).hats[p_hat];
	}
}

//!Internally clears state for next tic.

void sdl_input::clear_loop() {

	//Fucking elegant, tough memset would be faster :P.
	std::fill(std::begin(events_cache), std::end(events_cache), false);

	//Alimentamos las teclas pulsadas y trabajamos con ellas.
//	device_keyboard.set_pressed_keys(SDL_GetKeyboardState(nullptr));
	device_keyboard.reset_keys();
	clear_joysticks_state();
	device_mouse.init();
}


//!Returns the first key down index registered. Of limited use, actually.
int sdl_input::get_key_down_index() const {

	int i=0;
	while(i < SDL_NUM_SCANCODES) {
		if(device_keyboard.keys_down[i]) return i;
		++i;
	}

	return -1;
}

int sdl_input::get_key_up_index() const {

	int i=0;
	while(i < SDL_NUM_SCANCODES) {
		if(device_keyboard.keys_up[i]) return i;
		++i;
	}

	return -1;
}

//!Returns the first mouse button down index registered. Sees little use.

int sdl_input::get_mouse_button_down_index() const {

	unsigned int i=0;
	while(i < mouse::max_buttons) {
		if(device_mouse.buttons_down[i]) return i;
		++i;
	}
	return -1;
}

int sdl_input::get_mouse_button_up_index() const {

	unsigned int i=0;
	while(i < mouse::max_buttons) {
		if(device_mouse.buttons_up[i]) return i;
		++i;
	}
	return -1;
}

//!Returns the first joystick button down index registered. Used very sparingly.

int sdl_input::get_joystick_button_down_index(int index) const {

	const joystick& j=joysticks.at(index);
	unsigned int i=0;
	while(i < j.buttons) {
		if(j.buttons_down[i]) {
			return i;
		}
		++i;
	}

	return -1;
}

int sdl_input::get_joystick_button_up_index(int index) const {

	const joystick& j=joysticks.at(index);
	unsigned int i=0;
	while(i < j.buttons) {
		if(j.buttons_up[i]) {
			return i;
		}
		++i;
	}

	return -1;
}

//Returns the joystick index from its id. Useful to "learn" controls, specifically to convert event.jbutton.which to id.
int sdl_input::get_joystick_index_from_id(SDL_JoystickID id) const
{
	if(!id_joystick_to_index.count(id)) throw std::runtime_error("Invalid joystick id in get_joystick_index_from_id");
	return id_joystick_to_index.at(id);
}

sdl_input::event sdl_input::get_event() const {

	auto ev=get_system_event();
	if(ev.source!=event::sources::none) {

		return ev;
	}

	ev=get_keyboard_event();
	if(ev.source!=event::sources::none) {

		return ev;
	}

	ev=get_mouse_event();
	if(ev.source!=event::sources::none) {

		return ev;
	}

	ev=get_joystick_event();
	if(ev.source!=event::sources::none) {

		return ev;
	}

	return event{event::sources::none, event::types::none, 0, 0};
}

sdl_input::event sdl_input::get_system_event() const {
 
	if(is_exit_signal()) {

		return event{event::sources::system, event::types::system, event::c_exit, 0};
	}

	if(is_event_activity()) {

		//TODO:
	}

	if(is_event_activity_focus()) {

		//TODO:
	}

	return event{event::sources::none, event::types::none, 0, 0};
}

sdl_input::event sdl_input::get_keyboard_event() const {

	//First the keydowns
	if(is_event_keyboard_down()) {

		return event{event::sources::keyboard, event::types::down, get_key_down_index(), 0};
	}
	
	///Then the keyups
	if(is_event_keyboard_up()) {

		return event{event::sources::keyboard, event::types::up, get_key_up_index(), 0};
	}

	return event{event::sources::none, event::types::none, 0, 0};
}

sdl_input::event sdl_input::get_mouse_event() const {

	if(is_event_mouse_button_down()) {

		return event{event::sources::mouse, event::types::down, get_mouse_button_down_index(), 0};
	}

	if(is_event_mouse_button_up()) {

		return event{event::sources::mouse, event::types::up, get_mouse_button_up_index(), 0};
	}
	
	return event{event::sources::none, event::types::none, 0, 0};
}

sdl_input::event sdl_input::get_joystick_event() const {

	if(is_event_joystick_button_down()) {

		for(const auto& pair : joysticks) {

			int joy_index=pair.first;
			return event{event::sources::joystick, event::types::down, get_joystick_button_down_index(joy_index), joy_index};
		}
	}

	if(is_event_joystick_button_up()) {

		for(const auto& pair : joysticks) {

			int joy_index=pair.first;
			return event{event::sources::joystick, event::types::up, get_joystick_button_up_index(joy_index), joy_index};
		}
	}

	return event{event::sources::none, event::types::none, 0, 0};
}

//////////////////////

sdl_input::joystick::joystick(
	SDL_JoystickID pid, 
	int pdevice_id
)
	:structure(nullptr), id(pid), device_id(pdevice_id), 
	buttons(0), axis_size(0), hats_size(0),
	virtualized_hats(0), virtualized_axis(0),
	threshold_virtual_axis_button(0)
{ }

sdl_input::joystick::~joystick() {

	if(structure) {

		SDL_JoystickClose(structure);
	}
}

void sdl_input::joystick::init(SDL_Joystick * joy) {

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

void sdl_input::joystick::virtualize_hats() {

	if(virtualized_hats) return;
	int nbuttons=4 * hats_size;

	buttons_up.insert(std::end(buttons_up), nbuttons, false);
	buttons_pressed.insert(std::end(buttons_pressed), nbuttons, false);
	buttons_down.insert(std::end(buttons_down), nbuttons, false);
	buttons_released.insert(std::end(buttons_released), nbuttons, true);

	virtualized_hats=buttons; //First virtualised hat index.
	buttons+=nbuttons;
}

void sdl_input::joystick::virtualize_axis(
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

void sdl_input::joystick::register_button(
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

void sdl_input::joystick::register_axis(
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


void sdl_input::joystick::register_hat(
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

void sdl_input::joystick::init_state() {

	if(buttons) {

		std::fill(std::begin(buttons_up), std::end(buttons_up), false);
		std::fill(std::begin(buttons_down), std::end(buttons_down), false);
	}
}

///////////////

sdl_input::mouse::mouse()
	:position(),
	movement(false) {

	std::fill(std::begin(buttons_up), std::end(buttons_up), false);
	std::fill(std::begin(buttons_down), std::end(buttons_down), false);
	std::fill(std::begin(buttons_pressed), std::end(buttons_pressed), false);
	std::fill(std::begin(buttons_released), std::end(buttons_released), false);
}

void sdl_input::mouse::init() {

	std::fill(std::begin(buttons_up), std::end(buttons_up), false);
	std::fill(std::begin(buttons_down), std::end(buttons_down), false);
	movement=false;
}

//////////////

sdl_input::keyboard::keyboard()
	:keys_pressed_size{0}
{}

void sdl_input::keyboard::init_keys() {

	auto f=[](std::array<bool, SDL_NUM_SCANCODES>& a) {std::fill(std::begin(a), std::end(a), false);};
	f(keys_up);
	f(keys_down);
	f(keys_pressed);
	f(keys_released);
}


void sdl_input::keyboard::reset_keys() {

	auto f=[](std::array<bool, SDL_NUM_SCANCODES>& a) {std::fill(std::begin(a), std::end(a), false);};
	f(keys_up);
	f(keys_down);
}

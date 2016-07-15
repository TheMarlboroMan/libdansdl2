#include "sdl_input.h"

#include <algorithm>

using namespace ldi;

sdl_input::sdl_input():
	exit_signal(false), joysticks_size(0)

{
	SDL_StopTextInput();

	events_cache.reserve(max_cache_index);
	for(size_t i=0; i<events_cache.size(); ++i) events_cache[i]=false;

	device_keyboard.init_keys(true);
	init_joysticks();
}

sdl_input::~sdl_input()
{
	joysticks.clear();
}

void sdl_input::init_joysticks()
{
	this->joysticks_size=SDL_NumJoysticks();

	if(this->joysticks_size > 0)
	{
		SDL_JoystickEventState(SDL_ENABLE);

		ldt::log_lsdl::get()<<"Located "<<joysticks_size<<" joysticks"<<std::endl;

		for(int i=0; i<joysticks_size; i++)
		{
			SDL_Joystick * joy=SDL_JoystickOpen(i);	//De alguna manera el valgrind saca aquí una pérdida.
			init_joystick(joy, i);
		}
	}
}

void sdl_input::init_joystick(SDL_Joystick * estructura, int index)
{
	SDL_JoystickID id=SDL_JoystickInstanceID(estructura);
	joysticks.insert(std::pair<int, joystick>(index, joystick(id, index) ) ) ;
	joysticks.at(index).init(estructura);
	id_joystick_to_index[id]=index;

	auto& j=joysticks.at(index);

	ldt::log_lsdl::get()<<"Init joystick "<<index<<" with "<<
		j.buttons<<" buttons, "<<
		j.hats_size<<" hats, "<<
		j.axis_size<<" axis."<<std::endl;

}

bool sdl_input::pump_events(SDL_Event &pevent, bool pprocess)
{
	bool result=SDL_PollEvent(&pevent);
	if(pprocess && result) this->process_event(pevent);
	return result;
}

void sdl_input::loop()
{
	this->clear_loop();
			
	SDL_Event event;
	//Eventos...
	while(SDL_PollEvent(&event))
	{
		this->process_event(event);
	}
}

/*
A esto se le llama una vez por cada evento que haya en "recoger". Lo hemos
incluido para poderlo llamar desde otros puntos como el "Escritor_texto" y 
poder seguir usando la clase de Controles desde fuera de la última.
*/

void sdl_input::process_event(SDL_Event& event)
{
	switch(event.type)
	{
		case SDL_QUIT:
			exit_signal=true; 
		break;

		case SDL_MOUSEMOTION:

			events_cache[mousemove]=true;
			device_mouse.x=event.motion.x; 
			device_mouse.y=event.motion.y; 
			device_mouse.movement=true;
		break;

		case SDL_MOUSEBUTTONDOWN:
			events_cache[mouseclick]=true;
			device_mouse.buttons_down[event.button.button]=1;
			device_mouse.buttons_pressed[event.button.button]=1;
		break;

		case SDL_MOUSEBUTTONUP:
			events_cache[mouseclick]=true;
			device_mouse.buttons_pressed[event.button.button]=1;
			device_mouse.buttons_pressed[event.button.button]=0;
		break;

		case SDL_JOYBUTTONDOWN:
			events_cache[joystick_button_down]=true;
			joysticks.at(id_joystick_to_index[event.jbutton.which]).register_button(0, event.jbutton.button);
		break;

		case SDL_JOYBUTTONUP:
			events_cache[joystick_button_up]=true;
			joysticks.at(id_joystick_to_index[event.jbutton.which]).register_button(1, event.jbutton.button);
		break;

		case SDL_JOYAXISMOTION:
			if(event.jaxis.value < -min_noise || event.jaxis.value > max_noise)
			{
				events_cache[joystick_axis]=true;
				joysticks.at(id_joystick_to_index[event.jbutton.which]).register_axis(event.jaxis.axis, event.jaxis.value);
			}
			else
			{
				joysticks.at(id_joystick_to_index[event.jbutton.which]).register_axis(event.jaxis.axis, 0);
			}

			if(joysticks.at(id_joystick_to_index[event.jhat.which]).virtualized_axis)
			{
				set_virtualized_input();
			}
		break;

		case SDL_JOYHATMOTION:
			events_cache[joystick_hat]=true;
			joysticks.at(id_joystick_to_index[event.jhat.which]).register_hat(event.jhat.hat, event.jhat.value);

			if(joysticks.at(id_joystick_to_index[event.jhat.which]).virtualized_hats)
			{
				set_virtualized_input();
			}

		break;

		case SDL_JOYDEVICEADDED:
		case SDL_CONTROLLERDEVICEADDED:
			ldt::log_lsdl::get()<<"New joystick detected."<<std::endl;
		
			if(!is_joystick_registered_by_device_id(event.cdevice.which))
			{
				init_joystick(SDL_JoystickOpen(event.cdevice.which), joysticks.size());
				++joysticks_size;
				events_cache[joystick_connected]=true;
			}
			else
			{
				ldt::log_lsdl::get()<<"The joystick had been already registered."<<std::endl;
			}
		break;

		case SDL_JOYDEVICEREMOVED:
		case SDL_CONTROLLERDEVICEREMOVED:
			ldt::log_lsdl::get()<<"Joystick removal detected."<<std::endl;
			joysticks.erase(id_joystick_to_index[event.cdevice.which]);
			--joysticks_size;
		break;

		case SDL_TEXTINPUT:
			events_cache[text]=true;
			input_text+=event.text.text;
		break;

//		case SDL_ACTIVEEVENT:
//			this->event_actividad.recibir_input((bool) event.active.gain, event.active.state);
//		break;

		case SDL_KEYDOWN:
		{
			unsigned int index=event.key.keysym.scancode;

			if(SDL_IsTextInputActive())
			{
				switch(event.key.keysym.sym)
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

			if(!device_keyboard.keys_down_locking[index])
			{
				events_cache[keyboard_down]=true;
				device_keyboard.keys_down[index]=1;
				device_keyboard.keys_down_locking[index]=1;
			}
		}
		break;

		case SDL_KEYUP:
		{
			unsigned int index=event.key.keysym.scancode;

			events_cache[keyboard_up]=true;
			device_keyboard.keys_up[index]=1;
			device_keyboard.keys_down_locking[index]=0;
		}
		break;

		default: break;
	}
}

void sdl_input::set_virtualized_input()
{
	for(const auto& pj: joysticks)
	{	
		const auto& j=pj.second;

		if(j.virtualized_hats || j.virtualized_axis)
		{
			if(std::any_of(std::begin(j.buttons_pressed), std::end(j.buttons_pressed), [](bool v) {return v;})) events_cache[joystick_button_up]=true; 
			if(std::any_of(std::begin(j.buttons_down), std::end(j.buttons_down), [](bool v) {return v;})) events_cache[joystick_button_down]=true; 
//			if(std::any_of(std::begin(j.buttons_pressed), std::end(j.buttons_pressed), [](bool v) {return v;}))
//			if(std::any_of(std::begin(j.buttons_released), std::end(j.buttons_released), [](bool v) {return v;}))
		}
	}
}

bool sdl_input::is_joystick_registered_by_device_id(unsigned int d_id)
{
	for(const auto& j : joysticks)
	{
		if(j.second.device_id==d_id) return true;
	}

	return false;
}

void sdl_input::clear_joysticks_state()
{
	for(auto& p: joysticks) p.second.init_state();
}

bool sdl_input::check_joystick_button(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!joysticks.size()) return false;
	else if(!joysticks.count(p_joystick)) return false;
	else if(pbutton > joysticks.at(p_joystick).buttons) return false;
	else return true;
}

bool sdl_input::is_joystick_button_down(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else 
	{
		return joysticks.at(p_joystick).buttons_down[pbutton];
	}
}

bool sdl_input::is_joystick_button_up(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else 
	{
		return joysticks.at(p_joystick).buttons_up[pbutton];	
	}
}

bool sdl_input::is_joystick_button_pressed(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else 
	{
		return joysticks.at(p_joystick).buttons_pressed[pbutton];
	}
}

bool sdl_input::is_joystick_button_released(unsigned int p_joystick, unsigned int pbutton) const
{
	if(!this->check_joystick_button(p_joystick, pbutton)) return false;
	else 
	{
		return joysticks.at(p_joystick).buttons_released[pbutton];	
	}
}

bool sdl_input::check_joystick_axis(unsigned int p_joystick, unsigned int p_eje) const
{
	if(!joysticks.size()) return false;
	else if(joysticks.count(p_joystick)) return false;
	else if(p_eje > joysticks.at(p_joystick).axis_size) return false;
	else return true;
}

bool sdl_input::check_joystick_hat(unsigned int p_joystick, unsigned int p_hat) const
{
	if(!joysticks.size()) return false;
	else if(joysticks.count(p_joystick)) return false;
	else if(p_hat > joysticks.at(p_joystick).hats_size) return false;
	else return true;
}

Sint16 sdl_input::get_joystick_axis(unsigned int p_joystick, unsigned int p_eje) const
{
	if(!this->check_joystick_axis(p_joystick, p_eje)) return false;
	else
	{
		return joysticks.at(p_joystick).axis[p_eje];
	}
}

int sdl_input::get_joystick_hat(unsigned int p_joystick, unsigned int p_hat) const
{
	if(!this->check_joystick_hat(p_joystick, p_hat)) return false;
	else
	{
		return joysticks.at(p_joystick).hats[p_hat];
	}
}

void sdl_input::clear_loop()
{
	for(size_t i=0; i<events_cache.size(); ++i) events_cache[i]=false;

	//Alimentamos las teclas pulsadas y trabajamos con ellas.
	device_keyboard.set_pressed_keys(SDL_GetKeyboardState(NULL));
	device_keyboard.init_keys(false);
	clear_joysticks_state();
	device_mouse.init();
}

int sdl_input::get_key_down_index() const
{
	int i=0;
	while(i < SDL_NUM_SCANCODES)
	{
		if(device_keyboard.keys_down[i]) return i;
		++i;
	}
	
	return 1;
}

int sdl_input::get_mouse_button_down_index() const
{
	unsigned int i=0;
	while(i < mouse::max_buttons)
	{
		if(device_mouse.buttons_down[i]) return i;
		++i;
	}
	return -1;
}

int sdl_input::get_joystick_button_down_index(int index) const
{
	const joystick& j=joysticks.at(index);
	unsigned int i=0;

	while(i < j.buttons)
	{
		if(j.buttons_down[i]) 
		{
			return i;
		}
		++i;
	}

	return -1;
}

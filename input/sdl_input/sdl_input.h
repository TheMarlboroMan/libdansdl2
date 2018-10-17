#ifndef LIBDANSDL2_SDL_INPUT_H
#define LIBDANSDL2_SDL_INPUT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib> //Para abs en windows.
#include <vector>
#include <functional>
#include <map>
#include <array>
#include <string>
#include "../../tools/log/log.h"

namespace ldi
{

//!Complete input inferface.

//!Allows access to keyboard, mouse and joysticks. Additionally provides hooks
//!for custom event processing and text input. Text input allows the interface
//!to take control keystrokes and build a std::string with them.

class sdl_input
{
	public:

	//!Represents the mouse position.
	struct mouse_position
	{
		int 		x=0,	//!< X position for the mouse.
				y=0;	//!< Y position for the mouse.
	};

	private:

	//!Private structure representing the keyboard layout and state.
	class keyboard
	{
		public:
					keyboard()
			:keys_pressed_size{0}
		{}
			
		void			init_keys()
		{
			auto f=[](std::array<bool, SDL_NUM_SCANCODES>& a) {std::fill(std::begin(a), std::end(a), false);};
			f(keys_up); 
			f(keys_down);
			f(keys_pressed);
		}

		void			reset_keys()
		{
			auto f=[](std::array<bool, SDL_NUM_SCANCODES>& a) {std::fill(std::begin(a), std::end(a), false);};
			f(keys_up); 
			f(keys_down);
		}

		std::array<bool, SDL_NUM_SCANCODES>	keys_up,
							keys_down,
							keys_pressed;
		int					keys_pressed_size; //This goes up when a key down and down when a key up. As long as it is larger than zero there are keypresses.
	};

	//!Private structure to interact with mouse parameters.

	class mouse
	{
		public:	//Public porque esto si se devuelve es como "const" y es más cómodo que andar con accedentes a este nivel.

		mouse_position 		position;

		int 			get_x() const {return position.x;}
		int 			get_y() const {return position.y;}
		const mouse_position&	get_position() const {return position;}

		bool 			movement;

		static const unsigned int max_buttons=6;
		std::array<bool, max_buttons>	buttons_up,
						buttons_down,
						buttons_pressed;

	
		mouse():
			position(),
			movement(false)
		{
			std::fill(std::begin(buttons_pressed), std::end(buttons_pressed), false);
		}

		void init()
		{
			std::fill(std::begin(buttons_up), std::end(buttons_up), false);
			std::fill(std::begin(buttons_down), std::end(buttons_down), false);
			movement=false;
		}
	};

	//!Private structure to interact with joysticks and controllers.

	class joystick
	{
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
				

		joystick(SDL_JoystickID pid, int pdevice_id)
			:structure(nullptr), id(pid), device_id(pdevice_id), 
			buttons(0), axis_size(0), hats_size(0),
			virtualized_hats(0), virtualized_axis(0),
			threshold_virtual_axis_button(0)
		{
		}

		//Joysticks are inside a map that copies the object so it
		//would be deleted and freed if emplaced in the constructor.
		//So far we'll just pass the structure after construction.

		void init(SDL_Joystick * joy)
		{
			structure=joy;
			buttons=SDL_JoystickNumButtons(structure);
			axis_size=SDL_JoystickNumAxes(structure);
			hats_size=SDL_JoystickNumHats(structure);

			if(buttons)
			{
				buttons_up.resize(buttons, false);
				buttons_down.resize(buttons, false);
				buttons_pressed.resize(buttons, false);
				buttons_released.resize(buttons, false);
			}

			if(axis_size)
			{
				axis.resize(axis_size, 0);
			}

			if(hats_size)
			{
				hats.resize(hats_size, 0);
			}

			init_state();

			//Warning: this should not be moved to "init_state":
			//if done we'd lose the axis movement or pressed buttons.

			if(buttons)
			{
				buttons_up.insert(std::begin(buttons_up), buttons, false);
				buttons_pressed.insert(std::begin(buttons_pressed), buttons, false);
				buttons_down.insert(std::begin(buttons_down), buttons, false);
				buttons_released.insert(std::begin(buttons_released), buttons, true);
			}

			if(axis_size) 
			{
				axis.insert(std::begin(axis), axis_size, 0);
			}

			if(hats_size) 
			{
				hats.insert(std::begin(hats), hats_size, SDL_HAT_CENTERED);
			}
		}

		//Hats are treated as buttons.

		void virtualize_hats()
		{
			if(virtualized_hats) return;
			int nbuttons=4 * hats_size;

			buttons_up.insert(std::end(buttons_up), nbuttons, false);
			buttons_pressed.insert(std::end(buttons_pressed), nbuttons, false);
			buttons_down.insert(std::end(buttons_down), nbuttons, false);
			buttons_released.insert(std::end(buttons_released), nbuttons, true);

			virtualized_hats=buttons; //First virtualised hat index.
			buttons+=nbuttons;
		}

		void virtualize_axis(int virtual_threshold)
		{
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

		void register_button(unsigned int v_tipo, unsigned int v_button)
		{
			if(v_tipo==0)
			{
				buttons_down[v_button]=true;
				buttons_pressed[v_button]=true;
				buttons_released[v_button]=false;
			}
			else
			{
				buttons_up[v_button]=true;
				buttons_released[v_button]=true;
				buttons_pressed[v_button]=false;
			}
		}
	
		void register_axis(unsigned int v_axis, Sint16 v_value)
		{
			axis[v_axis]=v_value;

			if(virtualized_axis)
			{
				size_t index=virtualized_axis + (2 * v_axis);

				if(abs(v_value) > threshold_virtual_axis_button)
				{
					std::fill(std::begin(buttons_down)+index, std::begin(buttons_down)+index+2, false);
					std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+2, false);
					std::fill(std::begin(buttons_released)+index, std::begin(buttons_released)+index+2, true);

					auto f=[this](size_t v_button)
					{
						buttons_down[v_button]=true;
						buttons_pressed[v_button]=true;
						buttons_released[v_button]=false;
					};

					if(v_value > 0) f(index);
					else f(index+1);
				}
				else
				{
					auto f=[this](size_t v_button)
					{
						if(buttons_pressed[v_button]) 
						{
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

		void register_hat(unsigned int v_hat, int v_value)
		{
			hats[v_hat]=v_value;

			if(virtualized_hats)
			{
				size_t index=virtualized_hats + (4 * v_hat);

				if(v_value==SDL_HAT_CENTERED)
				{
					for(size_t helper=0; helper < 4; ++helper)
					{
						if(buttons_pressed[index+helper]) 
						{
							buttons_released[index+helper]=true;
							buttons_up[index+helper]=true;
						}
					}

					std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+4, false);
				}
				else
		 		{
					std::fill(std::begin(buttons_down)+index, std::begin(buttons_down)+index+4, false);
					std::fill(std::begin(buttons_pressed)+index, std::begin(buttons_pressed)+index+4, false);
					std::fill(std::begin(buttons_released)+index, std::begin(buttons_released)+index+4, true);

					auto f=[this](size_t v_button)
					{
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

		void init_state()
		{
			if(buttons)
			{
				std::fill(std::begin(buttons_up), std::end(buttons_up), false);
				std::fill(std::begin(buttons_down), std::end(buttons_down), false);
			}
		}

		~joystick()
		{
			if(structure)
			{
				SDL_JoystickClose(structure);
			}
		}		
	};

	//!Keeps track of all activity events (minimize, maximize, lose focus...).

	//!So far, this interface allows for only one simultaneous activity 
	//!event. To support more, we'd need a queue of these events.
	//!Supposedly get_state returns the SDL state index.

	class activity_event
	{
		public:

		activity_event():state(0), focus(false), activity_event_registered(false){}
		~activity_event(){}

		void 				get_input(bool p_focus, Uint8 p_state)
		{
			focus=p_focus;
			state=p_state;
			activity_event_registered=true;
		}

		void 				reset()
		{
			activity_event_registered=false;
			focus=false;
			state=0;
		}
	
		bool 				is_activity_event_registered() const {return activity_event_registered;}
		bool 				is_focused() const {return focus;}
		Uint8 				get_state() const {return state;}

		private:

		Uint8 				state;
		bool 				focus, 
						activity_event_registered;

	};

	public:

	//!Defines a default event handling function that can be passed to a 
	//!custom event handling function.
	typedef				std::function<void(SDL_Event& event)> tf_default;

	//!Defines a custom event handling function that receives the event and
	//!the default function defined by this class.
	typedef				std::function<bool(SDL_Event& event, tf_default&)> tf_event;

	private:

	keyboard		 	device_keyboard;
	mouse 				device_mouse;		//There's always a mouse... right?.
	activity_event	 		activity_event_instance;

	std::map<int, joystick> 	joysticks;
	std::map<SDL_JoystickID, int>	id_joystick_to_index;

	std::string 			input_text;
	bool				keydown_control_text_filter; //Indicates whether or not to pass backspace or enter to the input_text when text input is active.

	bool 				exit_signal; //Basically SDL_QUIT.
	unsigned short int 		joysticks_size;

	tf_event			f_process_event;		//!< Custom event processing function to be set with a call to set_event_processing_function.
	tf_default			f_default_process_event;	//!< Default event processing function, which is to call "process_event".

	void 				init_joysticks();
	void 				clear_joysticks_state();
	void 				clear_activity_event_state() {activity_event_instance.reset();}
	void				init_joystick(SDL_Joystick *, int);
	bool 				check_joystick_button(unsigned int, unsigned int) const;
	bool 				check_joystick_axis(unsigned int, unsigned int) const;
	bool 				check_joystick_hat(unsigned int, unsigned int) const;
	bool 				is_joystick_registered_by_device_id(unsigned int);
	void				set_virtualized_input();
	void 				clear_loop();
	void 				process_event(SDL_Event &);

	enum events_index{text=0, 
		mousemove, mousedown, mouseup,
		keyboard_down, keyboard_up, 
		joystick_axis, joystick_hat, joystick_button_up, joystick_button_down, joystick_connected, 
		max_cache_index};

	std::vector<bool> events_cache;

	public:

	//!Defines the joystick maximum and minimum axis and noise values.
	enum axis_values{
		min_axis=-32768,	//!< Minimum joystick axis value.
		max_axis=32767,		//!< Maximum joystick axis value.
		min_noise=-3200,	//!< Minimum joystick noise value threshold (they constantly generate values).
		max_noise=3200};	//!< Maximum joystick noise value threshold (they constantly generate values).

				sdl_input();
				~sdl_input();

	//!Activates text input. 
	void 			start_text_input() {SDL_StartTextInput();}
	//!Deactivates text input.
	void 			stop_text_input() {SDL_StopTextInput();}
	//!Clears the internal text buffer.
	void 			clear_text_input() {input_text=std::string("");}
	//!Returns the internal text buffer.
	const std::string 	get_text_input() const {return input_text;}
	//!Checks whether text input is activated.
	bool 			is_text_input() const {return SDL_IsTextInputActive();}
	//!Sets whether or not to pass backspace or enter to the input_text when text input is active.
	void			set_keydown_control_text_filter(bool v) {keydown_control_text_filter=v;}

	bool 	 		pump_events(SDL_Event &, bool=true);
	//!Gets the full mouse structure.
	const mouse& 		get_mouse() const {return device_mouse;}
	//!Gets the indicated joystick. May throw if not available.
	const joystick& 	get_joystick(int index) const {return joysticks.at(index);}
	//!Gets the indicated joystick. May throw if not available.
	joystick& 		get_joystick(int index) {return joysticks.at(index);}
	//!Instruct the joystick to virtualize its hats as buttons.
	void			virtualize_joystick_hats(int index) {joysticks.at(index).virtualize_hats();}
	//!Instruct the joystick to virtualize its axes as buttons.
	void			virtualize_joystick_axis(int index, int threshold) {joysticks.at(index).virtualize_axis(threshold);}
	void 			loop();
	//!Indicates if a new joystick was connected
	bool			is_event_joystick_connected() const {return events_cache[joystick_connected];}
	//!Moves the mouse cursor to the specified position.
	void 			warp_mouse(SDL_Window * w, unsigned int p_x, unsigned int p_y) {SDL_WarpMouseInWindow(w, p_x, p_y);}
	//!Checks if the key is pressed as expressed in SDL scancodes.
	bool 			is_key_pressed(int pkey) const {return device_keyboard.keys_pressed[pkey];}
	//!Checks if the key is down as expressed in SDL scancodes.
	bool 			is_key_down(int pkey) const {return device_keyboard.keys_down[pkey];}
	//!Checks if the key is up as expressed in SDL scancodes.
	bool 			is_key_up(int pkey) const {return device_keyboard.keys_up[pkey];}
	//!Checks if the mouse button is up.
	bool 			is_mouse_button_up(int pbutton) const {return device_mouse.buttons_up[pbutton];}
	//!Checks if the mouse button is down.
	bool			is_mouse_button_down(int pbutton) const {return device_mouse.buttons_down[pbutton];}
	//!Checks if the mouse button is pressed.
	bool 			is_mouse_button_pressed(int pbutton) const {return device_mouse.buttons_pressed[pbutton];}
	//!Checks if the mouse has been moved.
	bool 			is_mouse_movement() const {return device_mouse.movement;}
	bool 			is_joystick_button_down(unsigned int, unsigned int) const;
	bool 			is_joystick_button_up(unsigned int, unsigned int) const;
	bool 			is_joystick_button_pressed(unsigned int, unsigned int) const;
	bool 			is_joystick_button_released(unsigned int, unsigned int) const;
	Sint16 			get_joystick_axis(unsigned int, unsigned int) const;
	int			get_joystick_hat(unsigned int, unsigned int) const;
	//!Returns the number of connected joysticks.
	unsigned short int 	get_joysticks_size() const {return joysticks_size;}
	//!Returns the current mouse position.
	const mouse_position& 	get_mouse_position() const {return device_mouse.get_position();}
	//!Checks if the "close window" button has been pressed.
	bool 			is_exit_signal() const {return exit_signal;}
	//!Checks if there has been an acivity event (minimize, maximize...).
	bool 			is_event_activity() const {return activity_event_instance.is_activity_event_registered();}
	//!Checks if the activity event has focus. //TODO: What does this even mean????
	bool 			is_event_activity_focus() const {return activity_event_instance.is_focused();}
	//!Returns the kind of activity event. //TODO: Again, what is this???
	Uint8 			get_activity_event() const {return activity_event_instance.get_state();}
	int			get_key_down_index() const;
	int			get_mouse_button_down_index() const;
	int			get_joystick_button_down_index(int) const;

	//!Indicates if a text event has taken place.
	bool 			is_event_text() const {return events_cache[text];}
	//!Indicates if a mouse event has taken place.
	bool 			is_event_mouse() const {return events_cache[mousemove] || events_cache[mousedown] || events_cache[mouseup];}
	//!Indicates if a mouse movement event has taken place.
	bool 			is_event_mouse_movement() const {return events_cache[mousemove];}
	//!Indicates if a mouse button down event has taken place.
	bool 			is_event_mouse_button_down() const {return events_cache[mousedown];}
	//!Indicates if a mouse button up event has taken place.
	bool 			is_event_mouse_button_up() const {return events_cache[mouseup];}
	//!Indicates if any keyboard event has taken place.
	bool 			is_event_keyboard() const {return events_cache[keyboard_up] || events_cache[keyboard_down];}
	//!Indicates if a keyboard down event has taken place.
	bool 			is_event_keyboard_down() const {return events_cache[keyboard_down];}
	//!Indicates if a keyboard up event has taken place.
	bool 			is_event_keyboard_up() const {return events_cache[keyboard_up];}
	//!Indicates if a keyboard pressed event has taken place.
	bool 			is_event_keyboard_pressed() const {return device_keyboard.keys_pressed_size;}
	//!Indicates any joystick event has taken place.
	bool 			is_event_joystick() const {return events_cache[joystick_axis] || events_cache[joystick_hat] || events_cache[joystick_button_up] || events_cache[joystick_button_down] ;}
	//!Indicates any joystick axis event has taken place.
	bool 			is_event_joystick_axis() const {return events_cache[joystick_axis];}
	//!Indicates any joystick hat event has taken place.
	bool 			is_event_joystick_hat() const {return events_cache[joystick_hat];}
	//!Indicates any joystick button event has taken place.
	bool 			is_event_joystick_button() const {return events_cache[joystick_button_up] || events_cache[joystick_button_down];}
	//!Indicates a joystick button up event has taken place.
	bool 			is_event_joystick_button_up() const {return events_cache[joystick_button_up];}
	//!Indicates a joystick button down event has taken place.
	bool 			is_event_joystick_button_down() const {return events_cache[joystick_button_down];}
	//!Indicates if any input is received.
	bool 			is_event_input() const {return is_event_mouse() || is_event_keyboard() || is_event_joystick();}
	//!Same as before, but with key presses (not really events) too.
	bool 			is_event_input_with_pressed() const {return is_event_mouse() || is_event_keyboard() || is_event_joystick() || is_event_keyboard_pressed();}

		
	//!Resets the custom processing function to its default value, which
	//!is to call "process_event".
	void			reset_event_processing_function();

	//!Sets a new event processing function. Replaces callback template. 

	//!The function must process the event and return true. If it returns 
	//!false, the event loop will exit until the next step. 
	//!If the parameter function is called, the default function defined
	//!by this class will be used to process the event with the event parameter.
	void			set_event_processing_function(tf_event f) {f_process_event=f;}

	//!Returns the internal joystic index from the SDL id handler.
	int			get_joystick_index_from_id(SDL_JoystickID) const;
};

}

#endif

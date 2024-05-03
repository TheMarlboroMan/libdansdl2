#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib> //Para abs en windows.
#include <vector>
#include <functional>
#include <map>
#include <array>
#include <string>

namespace ldi
{

//!Complete input inferface.

//!Allows access to keyboard, mouse and joysticks. Additionally provides hooks
//!for custom event processing and text input. Text input allows the interface
//!to take control keystrokes and build a std::string with them.

class sdl_input {

	private:

	class mouse;
	class joystick;

	public:
	
	//!Represents the mouse position.
	struct mouse_position {
		int x=0,	//!< X position for the mouse.
	        y=0;	//!< Y position for the mouse.
	};


	//!Defines a default event handling function that can be passed to a 
	//!custom event handling function.
	using               tf_default=std::function<void(SDL_Event& event)> ;

	//!Defines a custom event handling function that receives the event and
	//!the default function defined by this class. The event is a pure
	//!SDL one.
	using               tf_event=std::function<bool(SDL_Event& event, tf_default&)> ;

	//!Defines a text filter event.
	using               text_filter=std::function<bool(const SDL_Event&)>;

	//!Defines the joystick maximum and minimum axis and noise values.
	enum axis_values{
		min_axis=-32768,	//!< Minimum joystick axis value.
		max_axis=32767,		//!< Maximum joystick axis value.
		min_noise=-3200,	//!< Minimum joystick noise value threshold (they constantly generate values).
		max_noise=3200	//!< Maximum joystick noise value threshold (they constantly generate values).
	};

	                sdl_input();
	                ~sdl_input();

	//!Activates text input. 
	void                start_text_input() {SDL_StartTextInput();}
	//!Deactivates text input.
	void                stop_text_input() {SDL_StopTextInput();}
	//!Clears the internal text buffer.
	void                clear_text_input() {input_text=std::string("");}
	//!Returns the internal text buffer.
	const std::string& 	get_text_input() const {return input_text;}
	//!Checks whether text input is activated.
	bool                is_text_input() const {return SDL_IsTextInputActive();}
	//!Returns true if the backspace and enter are passed to text input.
	bool                get_keydown_control_text_filter() const {return keydown_control_text_filter;}
	//!Sets whether or not to pass backspace or enter to the input_text when text input is active.
	void                set_keydown_control_text_filter(bool v) {keydown_control_text_filter=v;}
	//!Sets the text filter function, which must return true for the input to be accepted.
	void                set_text_filter(text_filter _f) { f_text_filter=_f; }
	//!Removes the text filter function
	void                clear_text_filter() { f_text_filter=nullptr; }
	//!Loads an event from the queue into the first parameter. Returns true if there was an event in the queue.
	bool                pump_events(SDL_Event &, bool=true);
	//!Gets the full mouse structure.
	const mouse& 		get_mouse() const {return device_mouse;}
	//!Returns true if a joystick with the given id exists
	bool                has_joystick(int index) const {return joysticks.count(index);}
	//!Gets the indicated joystick. May throw if not available.
	const joystick&     get_joystick(int index) const {return joysticks.at(index);}
	//!Gets the indicated joystick. May throw if not available.
	joystick&           get_joystick(int index) {return joysticks.at(index);}
	//!Instruct the joystick to virtualize its hats as buttons.
	void                virtualize_joystick_hats(int index) {joysticks.at(index).virtualize_hats();}
	//!Instruct the joystick to virtualize its axes as buttons.
	void                virtualize_joystick_axis(int index, int threshold) {joysticks.at(index).virtualize_axis(threshold);}
	//!Must be called before each app iteration that needs fresh input!
	void                loop();
	//!Indicates if a new joystick was connected
	bool                is_event_joystick_connected() const {return events_cache[joystick_connected];}
	/**
	* Indicates if a joystick was removed. No way to know which, actually, it
	* falls upon the application to keep track of this with calls to 
	* get_joystick_indexes.
	*/
	bool                is_event_joystick_disconnected() const {return events_cache[joystick_disconnected];}
	//!Moves the mouse cursor to the specified position.
	void                warp_mouse(SDL_Window * w, unsigned int p_x, unsigned int p_y) {SDL_WarpMouseInWindow(w, p_x, p_y);}
	//!Checks if the key is pressed as expressed in SDL scancodes.
	bool                is_key_pressed(int pkey) const {return device_keyboard.keys_pressed[pkey];}
	//!Checks if the key is released as expressed in SDL scancodes.
	bool                is_key_released(int pkey) const {return device_keyboard.keys_released[pkey];}
	//!Checks if the key is down as expressed in SDL scancodes.
	bool                is_key_down(int pkey) const {return device_keyboard.keys_down[pkey];}
	//!Checks if the key is up as expressed in SDL scancodes.
	bool                is_key_up(int pkey) const {return device_keyboard.keys_up[pkey];}
	//!Checks if the mouse button is up.
	bool                is_mouse_button_up(int pbutton) const {return device_mouse.buttons_up[pbutton];}
	//!Checks if the mouse button is down.
	bool                is_mouse_button_down(int pbutton) const {return device_mouse.buttons_down[pbutton];}
	//!Checks if the mouse button is pressed.
	bool                is_mouse_button_pressed(int pbutton) const {return device_mouse.buttons_pressed[pbutton];}
	//!Checks if the mouse button is released.
	bool                is_mouse_button_released(int pbutton) const {return device_mouse.buttons_released[pbutton];}
	//!Checks if the mouse has been moved.
	bool                is_mouse_movement() const {return device_mouse.movement;}
	bool                is_joystick_button_down(unsigned int, unsigned int) const;
	bool                is_joystick_button_up(unsigned int, unsigned int) const;
	bool                is_joystick_button_pressed(unsigned int, unsigned int) const;
	bool                is_joystick_button_released(unsigned int, unsigned int) const;
	Sint16              get_joystick_axis(unsigned int, unsigned int) const;
	int                 get_joystick_hat(unsigned int, unsigned int) const;
	//!Returns the number of connected joysticks.
	unsigned short int  get_joysticks_size() const {return joysticks_size;}
	//!Returns the current mouse position.
	const mouse_position& 	get_mouse_position() const {return device_mouse.get_position();}
	//!Checks if the "close window" button has been pressed.
	bool            is_exit_signal() const {return exit_signal;}
	//!Checks if there has been an acivity event (minimize, maximize...).
	bool            is_event_activity() const {return activity_event_instance.is_activity_event_registered();}
	//!Checks if the current window has focus. I guess...
	bool            is_event_activity_focus() const {return activity_event_instance.is_focused();}
	//!Returns the kind of activity event. //TODO: Again, what is this???
	Uint8           get_activity_event() const {return activity_event_instance.get_state();}
	int             get_key_down_index() const;
	int             get_key_up_index() const;
	int             get_mouse_button_down_index() const;
	int             get_mouse_button_up_index() const;
	int             get_joystick_button_down_index(int) const;
	int             get_joystick_button_up_index(int) const;

	//!Indicates if a text event has taken place.
	bool            is_event_text() const {return events_cache[text];}
	//!Indicates if a mouse event has taken place.
	bool            is_event_mouse() const {return events_cache[mousemove] || events_cache[mousedown] || events_cache[mouseup];}
	//!Indicates if a mouse movement event has taken place.
	bool            is_event_mouse_movement() const {return events_cache[mousemove];}
	//!Indicates if a mouse button down event has taken place.
	bool            is_event_mouse_button_down() const {return events_cache[mousedown];}
	//!Indicates if a mouse button up event has taken place.
	bool            is_event_mouse_button_up() const {return events_cache[mouseup];}
	//!Indicates if any keyboard event has taken place.
	bool            is_event_keyboard() const {return events_cache[keyboard_up] || events_cache[keyboard_down];}
	//!Indicates if a keyboard down event has taken place.
	bool            is_event_keyboard_down() const {return events_cache[keyboard_down];}
	//!Indicates if a keyboard up event has taken place.
	bool            is_event_keyboard_up() const {return events_cache[keyboard_up];}
	//!Indicates if a keyboard pressed event has taken place.
	bool            is_event_keyboard_pressed() const {return device_keyboard.keys_pressed_size;}
	//!Indicates any joystick event has taken place.
	bool            is_event_joystick() const {return events_cache[joystick_axis] || events_cache[joystick_hat] || events_cache[joystick_button_up] || events_cache[joystick_button_down] ;}
	//!Indicates any joystick axis event has taken place.
	bool            is_event_joystick_axis() const {return events_cache[joystick_axis];}
	//!Indicates any joystick hat event has taken place.
	bool            is_event_joystick_hat() const {return events_cache[joystick_hat];}
	//!Indicates any joystick button event has taken place.
	bool            is_event_joystick_button() const {return events_cache[joystick_button_up] || events_cache[joystick_button_down];}
	//!Indicates a joystick button up event has taken place.
	bool            is_event_joystick_button_up() const {return events_cache[joystick_button_up];}
	//!Indicates a joystick button down event has taken place.
	bool            is_event_joystick_button_down() const {return events_cache[joystick_button_down];}
	//!Indicates if any input is received from mouse, keyboard or joystick.
	bool            is_event_input() const {return is_event_mouse() || is_event_keyboard() || is_event_joystick();}
	//!Same as before, but with key presses (not really events) too.
	bool            is_event_input_with_pressed() const {return is_event_mouse() || is_event_keyboard() || is_event_joystick() || is_event_keyboard_pressed();}
	//!Simplified event that can be polled from get_event methods. These are
	//!extracted from the internal buffers of the ldi::sld_input class so their
	//!order is fixed.
	struct event {

		enum codes{c_exit=1}; 

		enum class sources {none, mouse, keyboard, joystick, system} source; //origin of event.
		enum class types {none, up, down, system} type;
		int code{0}; //Code of the event (button, key...).
		int source_index{0}; //Index of the source, used for joysticks
	};
	//!Gets the first of any events in the system,keyboard,mouse,joystick order.
	event           get_event() const;
	event           get_system_event() const;
	event           get_keyboard_event() const;
	event           get_mouse_event() const;
	event           get_joystick_event() const;

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

	private:

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
		void register_button(unsigned int v_tipo, unsigned int v_button);
		void register_axis(unsigned int v_axis, Sint16 v_value);
		void register_hat(unsigned int v_hat, int v_value);
		void init_state();
	};

	//!Keeps track of all activity events (minimize, maximize, lose focus...).

	//!So far, this interface allows for only one simultaneous activity 
	//!event. To support more, we'd need a queue of these events.
	//!Supposedly get_state returns the SDL state index.

	class activity_event {

		public:

		activity_event():state(0), focus(false), activity_event_registered(false){}
		~activity_event(){}

		void 				get_input(bool p_focus, Uint8 p_state) {

	        focus=p_focus;
	        state=p_state;
	        activity_event_registered=true;
		}

		void                reset() {

	        activity_event_registered=false;
	        focus=false;
			state=0;
		}

		bool                is_activity_event_registered() const {return activity_event_registered;}
		bool                is_focused() const {return focus;}
		Uint8               get_state() const {return state;}

		private:

		Uint8               state;
		bool                focus,
		                    activity_event_registered;
	};

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
	text_filter         f_text_filter; //!Custom text processing function.

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
		joystick_axis, joystick_hat, joystick_button_up, joystick_button_down, joystick_connected, joystick_disconnected,
		max_cache_index};

	std::vector<bool> events_cache;
};

}


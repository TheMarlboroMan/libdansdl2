#ifndef LIBDANSDL2_SDL_INPUT_H
#define LIBDANSDL2_SDL_INPUT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <cstring> //Para memset.
#include <cstdlib> //Para abs en windows.
#include <vector>
#include <map>
#include <array>
#include <string>
#include "../../tools/log/log.h"

namespace ldi
{

class sdl_input
{
	public:

	struct mouse_position
	{
		int x, y;
		mouse_position(): x(0), y(0) {}
	};

	private:

	//El bloque de control es quien interactúa realmente con la capa inferior de la SDL.
	class keyboard
	{
		public:
					keyboard()
			:pressed_keys(nullptr)
		{

		}
			
		void 			set_pressed_keys(const Uint8 * v)  {pressed_keys=v;}
		Uint8 const * 		get_pressed_keys() const {return pressed_keys;}
		bool 			is_key_pressed(int v) const {return pressed_keys[v];}
		void			init_keys(bool locking)
		{
			memset(keys_up.data(), 0, SDL_NUM_SCANCODES);
			memset(keys_down.data(), 0, SDL_NUM_SCANCODES);
			if(locking) memset(keys_down_locking.data(), 0, SDL_NUM_SCANCODES);
		}

		std::array<char, SDL_NUM_SCANCODES>	keys_up,
							keys_down,
							keys_down_locking;


		private:

		//No es un leak: la estructura pertenece a SDL.
		const Uint8 * 		pressed_keys;

	};

	//Aquí dentro se guarda todo lo que tiene que ver con el raton.

	class mouse
	{
		public:	//Public porque esto si se devuelve es como "const" y es más cómodo que andar con accedentes a este nivel.

		mouse_position 		position;

		int& 			x, y;	//Referencias, por compatibilidad de cuando x e y flotaban fuera de Posicion.

		//Para los puristas, versiones acc...
		int 			get_x() const {return position.x;}
		int 			get_y() const {return position.y;}
		const mouse_position&	get_position() const {return position;}

		bool 			movement;

		static const unsigned int max_buttons=6;
		std::array<char, max_buttons>	buttons_up,
						buttons_down,
						buttons_pressed;

	
		mouse():
			position(),
			x(position.x), y(position.y),
			movement(false)
		{
			memset(buttons_pressed.data(), 0, max_buttons);
		}

		void init()
		{
			memset(buttons_up.data(), 0, max_buttons);
			memset(buttons_down.data(), 0, max_buttons);
			movement=false;
		}
	};

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

		/**
		* Los joysticks están metidos en un mapa que hace copia del objeto
		* de forma que se borra y se liberaría la estructura si la emplazamos
		* en el constructor. La solución de momento es pasar la estructura 
		* después de construido el objeto.
		*/

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

			/*OJO: No vamos a mover esto a "inicializar state!!!",
			si lo hacemos se perderá el state del input de botones
			pulsados o movements de ejes!!!.*/

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

		//Crea botones virtuales para los hats.
		void virtualize_hats()
		{
			if(virtualized_hats) return;
			int nbuttons=4 * hats_size;

			buttons_up.insert(std::end(buttons_up), nbuttons, false);
			buttons_pressed.insert(std::end(buttons_pressed), nbuttons, false);
			buttons_down.insert(std::end(buttons_down), nbuttons, false);
			buttons_released.insert(std::end(buttons_released), nbuttons, true);

			virtualized_hats=buttons; //Primer índice de los hats virtualizados.
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

			virtualized_axis=buttons; //Primer índice de los ejes virtualizados.
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
/*
		void debug()
		{
			std::cout<<std::endl<<"UP\t";
			for(auto v : buttons_up) std::cout<<(int(v));
			std::cout<<std::endl<<"PULSA\t";
			for(auto v : buttons_pressed) std::cout<<(int(v));
			std::cout<<std::endl<<"DOWN\t";
			for(auto v : buttons_down) std::cout<<(int(v));
			std::cout<<std::endl<<"SOLTA\t";
			for(auto v : buttons_released) std::cout<<(int(v));
			std::cout<<std::endl;
		}
*/
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

	/*Esta clase controla los eventos de actividad (minimizar, maximizar,
	pérdida de foco...). Será un miembro de la clase Controles de modo que
	siempre estará ahí con los últimos valores que tuviera. Para usarla
	correctamente preguntaremos siempre si hay algún evento de actividad
	y luego, si lo hay, preguntaremos ya por el state y el foco.

	Conservamos una sana actitud escéptica a que se den varios eventos de
	actividad a la vez. Si lo hubiera tendríamos que cambiar toda la 
	interface pública por una cola de eventos.
	*/

	class activity_event
	{
		private:

		Uint8 state;
		bool focus;
		bool activity_event_registered;

		public:

		activity_event():state(0), focus(false), activity_event_registered(false){}
		~activity_event(){}

		void get_input(bool p_focus, Uint8 p_state)
		{
			focus=p_focus;
			state=p_state;
			activity_event_registered=true;
		}

		void reset()
		{
			activity_event_registered=false;
			focus=false;
			state=0;
		}
	
		bool is_activity_event_registered() const {return activity_event_registered;}
		bool is_focused() const {return focus;}
		Uint8 get_state() const {return state;}
	};

	private:

	keyboard		 	device_keyboard;
	mouse 				device_mouse;		//Siempre hay un ratón, no?.
	activity_event	 		activity_event_instance;

	std::map<int, joystick> 	joysticks;
	std::map<SDL_JoystickID, int>	id_joystick_to_index;

	std::string 			input_text;	//El input de texto...
	bool				keydown_control_text_filter; //Indicates whether or not to pass backspace or enter to the input_text when text input is active.

	bool 				exit_signal; //SDL_QUIT; básicamente...
	unsigned short int 		joysticks_size;

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


	/*Esto registra simplemente si hay algún evento de este tipo por cada
	ciclo de eventos.*/

	enum events_index{text=0, 
		mousemove, mousedown, mouseup,
		keyboard_down, keyboard_up,
		joystick_axis, joystick_hat, joystick_button_up, joystick_button_down, joystick_connected, 
		max_cache_index};

	std::vector<bool> events_cache;

	/*Registra el foco...*/


	public:

	enum axis_values{min_axis=-32768, max_axis=32767, min_noise=-3200, max_noise=3200};

				sdl_input();
				~sdl_input();

	void 			start_text_input() {SDL_StartTextInput();}
	void 			stop_text_input() {SDL_StopTextInput();}
	void 			clear_text_input() {input_text=std::string("");}
	const std::string 	get_text_input() const {return input_text;}
	bool 			is_text_input() const {return SDL_IsTextInputActive();}
	void			set_keydown_control_text_filter(bool v) {keydown_control_text_filter=v;}

	bool 			pump_events(SDL_Event &, bool=true);
	//const SDL_Event& 	get_event() const {return event;}
	const mouse& 		get_mouse() const {return device_mouse;}
	const joystick& 	get_joystick(int index) const {return joysticks.at(index);}
	joystick& 		get_joystick(int index) {return joysticks.at(index);}
	void			virtualize_joystick_hats(int index) {joysticks.at(index).virtualize_hats();}
	void			virtualize_joystick_axis(int index, int threshold) {joysticks.at(index).virtualize_axis(threshold);}
	void 			loop();
	bool			is_event_joystick_connected() const {return events_cache[joystick_connected];}

	/*Recibe como parámetro una functión f que toma como parámetro un evento y
	devuelve true o false... Si devuelve true se sale del loop. Es una forma 
	de obtener un control a bajo nivel sobre los eventos.

	Un ejemplo de un callback para poder leer manualmente el input que se recibe...  Se
	puede usar para, por ejemplo, aprender las teclas que se quieran usar.

	bool test;
	struct Cosa
	{
		int val;
		bool operator()(SDL_Event& ev)
		{
			if(ev.type==SDL_KEYDOWN)
			{
				std::cout<<"CODIGO DE TECLA ES "<<ev.key.keysym.scancode<<std::endl;
				return true;
			}

			++val;
			std::cout<<"HOLA "<<val<<std::endl;
			return false;
		}
		
		Cosa():val(0) {}		
	}c;

	virtual void turno()
	{
		if(!test)
		{
			Input_base::turno();
		}
		else
		{
			acc_controles_sdl().recoger_callback(c);
			std::cout<<c.val<<std::endl;
			if(c.val > 100) test=false;
		}
	}
	*/

	template <typename TipoFunc> void loop_callback(TipoFunc& f)
	{
		this->clear_loop();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(f(event)) break;
		}
	}

	void 			warp_mouse(SDL_Window * w, unsigned int p_x, unsigned int p_y) {SDL_WarpMouseInWindow(w, p_x, p_y);}
	bool 			is_key_pressed(int pkey) const {return device_keyboard.is_key_pressed(pkey);}
	bool 			is_key_down(int pkey) const {return device_keyboard.keys_down[pkey];}
	bool 			is_key_up(int pkey) const {return device_keyboard.keys_up[pkey];}
	bool 			is_mouse_button_up(int pbutton) const {return device_mouse.buttons_up[pbutton];}
	bool			is_mouse_button_down(int pbutton) const {return device_mouse.buttons_down[pbutton];}
	bool 			is_mouse_button_pressed(int pbutton) const {return device_mouse.buttons_pressed[pbutton];}
	bool 			is_mouse_movement() const {return device_mouse.movement;}
	bool 			is_joystick_button_down(unsigned int, unsigned int) const;
	bool 			is_joystick_button_up(unsigned int, unsigned int) const;
	bool 			is_joystick_button_pressed(unsigned int, unsigned int) const;
	bool 			is_joystick_button_released(unsigned int, unsigned int) const;
	Sint16 			get_joystick_axis(unsigned int, unsigned int) const;
	int			get_joystick_hat(unsigned int, unsigned int) const;
	unsigned short int 	get_joysticks_size() const {return joysticks_size;}
	mouse_position 		get_mouse_position() const {return device_mouse.get_position();}
	bool 			is_exit_signal() const {return exit_signal;}
	bool 			is_event_activity() const {return activity_event_instance.is_activity_event_registered();}
	bool 			is_event_activity_focus() const {return activity_event_instance.is_focused();}
	Uint8 			get_activity_event() const {return activity_event_instance.get_state();}
	int			get_key_down_index() const;
	int			get_mouse_button_down_index() const;
	int			get_joystick_button_down_index(int) const;

	//Quick cache access.
	bool 			is_event_text() const {return events_cache[text];}
	bool 			is_event_mouse() const {return events_cache[mousemove] || events_cache[mousedown] || events_cache[mouseup];}
	bool 			is_event_mouse_movement() const {return events_cache[mousemove];}
	bool 			is_event_mouse_button_down() const {return events_cache[mousedown];}
	bool 			is_event_mouse_button_up() const {return events_cache[mouseup];}
	bool 			is_event_keyboard() const {return events_cache[keyboard_up] || events_cache[keyboard_down];}
	bool 			is_event_keyboard_down() const {return events_cache[keyboard_down];}
	bool 			is_event_keyboard_up() const {return events_cache[keyboard_up];}

	bool 			is_event_joystick() const {return events_cache[joystick_axis] || events_cache[joystick_hat] || events_cache[joystick_button_up] || events_cache[joystick_button_down] ;}
	bool 			is_event_joystick_axis() const {return events_cache[joystick_axis];}
	bool 			is_event_joystick_hat() const {return events_cache[joystick_hat];}
	bool 			is_event_joystick_button() const {return events_cache[joystick_button_up] || events_cache[joystick_button_down];}
	bool 			is_event_joystick_button_up() const {return events_cache[joystick_button_up];}
	bool 			is_event_joystick_button_down() const {return events_cache[joystick_button_down];}
	bool 			is_event_input() const {return is_event_mouse() || is_event_keyboard() || is_event_joystick();}
};

} //Fin namespace DLibI

#endif

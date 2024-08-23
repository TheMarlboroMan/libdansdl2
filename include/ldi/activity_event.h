#pragma once

#include <SDL2/SDL.h>

namespace ldi {

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

}

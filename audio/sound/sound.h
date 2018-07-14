#ifndef LIBDANSDL2_SOUND_H
#define LIBDANSDL2_SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "../../tools/log/log.h"

namespace lda
{

//!Wrapper for Mix_Chunk data.

class sound
{
	public:

				sound();
				sound(const std::string&);
	~sound();

	void 			load(const std::string&);

	//!Returns the original file path.
	const std::string&	get_path() const {return path;}
	//!Indicates whether the file was loaded correctly.
	bool			is_ready() const {return ready;}
	//!Returns a raw pointer to the sound data. This pointer must not be freed.
	Mix_Chunk * 		get_data() {return sound_data;}

	private:

	void 			free();

	Mix_Chunk * 		sound_data;
	std::string 		path;
	bool 			ready;
};

//!Simple structure to calculate stereo panning.

struct sound_panning {

	int 			left,	//!< Left panning, 127 is the max value.
				right;	//!< Right panning, 127 is the max value.
	
	//!Gets a stereo sound panning object from the left value.
	static sound_panning 	from_left(int v) {
		sound_panning res{v, 254-v};
		return res;
	}
};

//!Additional information for sound.

//!Includes a pointer to the sound, plus volume, repeats, fade and everything
//!else needed to interact with the audio_controller. It is recommended that
//!applications use this class routinely to play sound.

struct sound_struct
{
	sound *	 		sound_ptr; 	//!< This has to be copy constructible, ergo the pointer.
	short int 		volume;		//!< -1 means "no changes" to the channel volume.
	short int 		repeat; 	//!< -1 means "infinite".
	sound_panning		panning;	//!< Stereo panning.
	int 			ms_fade;	//!< Milliseconds of fadeout, as provided by SDL audio.
	
	//!Checks whether the sound_ptr is ready. Will crash if there is no sound pointer.
	bool 			is_ready() const {return sound_ptr->is_ready();}

	//!A sound structure. 

	//!Default volume is -1 (unchanged)
	//!Default repetition is 0.
	//!Default left and right volume is 127 (max for both).
	//!Default fade is 0milliseconds.

	sound_struct(sound& s, int v=-1, int r=0, sound_panning sp={255,255}, int msf=0)
		:sound_ptr(&s), volume(v), repeat(r), panning(sp), ms_fade(msf) {
	}
};

}

#endif

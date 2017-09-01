#ifndef LIBDANSDL2_MUSIC_H
#define LIBDANSDL2_MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

namespace lda
{

//!Wrapper for a Mix_Music pointer.

//!The sad thing is that it can't be copied, since the destructor really
//!does free the music itself.

class music
{
	public:

				music();
				music(const std::string&);
				~music();

	void	 		load(const std::string&);

	//!Returns the original audio file path.
	std::string	 	get_path() const {return path;}
	//!Returns whether the audio file was loaded correctly.
	bool 			is_ready() const {return ready;}
	//!Returns a raw pointer to the music data. The pointer must not be freed.
	Mix_Music * 		get_data() {return music_data;}

	private:

	void 			free();

	Mix_Music * 		music_data;
	std::string 		path;
	bool 			ready;
};

//!Additional information for music.

//!Includes a pointer to the music, plus volume and repeats. It is recommended 
//!that applications use this class routinely to play music.

struct music_struct
{
	music *	 		sound_ptr; //!< This has to be copy constructible, ergo the pointer.
	short int 		volume;
	short int 		repeat;
	
	//!Checks whether the music_ptr is ready. Will crash if there is no sound pointer.
	bool 			is_ready() 	{return music_ptr->is_ready();}

	//!A sound structure. 

	//!Default volume is -1 (unchanged from the previous one)
	//!Default repetition is -1 (loop until manually stopped)

	music_struct(music& s, int v=-1, int r=-1)
		:music_ptr(&s), volume(v), repeat(r)
	{
	}
};

}

#endif

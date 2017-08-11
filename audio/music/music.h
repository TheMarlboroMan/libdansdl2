#ifndef LIBDANSDL2_MUSIC_H
#define LIBDANSDL2_MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

namespace lda
{

//!Wrapper for a Mix_Music pointer.

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

}

#endif

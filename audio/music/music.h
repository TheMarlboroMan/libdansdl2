#ifndef LIBDANSDL2_MUSIC_H
#define LIBDANSDL2_MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>

namespace lda
{
class music
{
	public:

				music();
				music(const std::string&);
				~music();

	void	 		load(const std::string&);

	std::string	 	get_path() const {return path;}
	bool 			is_ready() const {return ready;}
	Mix_Music * 		get_data() {return music_data;}

	private:

	void 			free();

	Mix_Music * 		music_data;
	std::string 		path;
	bool 			ready;
};

}

#endif

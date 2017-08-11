#ifndef LIBDANSDL2_AUDIO_RESOURCE_MANAGER_H
#define LIBDANSDL2_AUDIO_RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../sound/sound.h"
#include "../music/music.h"

namespace lda
{

//!An object to store music and sound classes.

//!Its design mirrors the one on the video resource manager. No sounds or
//!music loaded can be unloaded, indexes are not shared between audio files
//!and music objects.

//TODO: Unloading sounds and music would be a good idea.

class resource_manager
{
	public:

	void 		insert_sound(int, const std::string&);
	void		insert_music(int, const std::string&);

	music& 		get_music(int);
	sound&		get_sound(int);

	//!Returns the number of music files loaded.
	size_t		size_music() const {return musics.size();}
	//!Returns the number of sound files loaded.
	size_t		size_sound() const {return sounds.size();}

	private:

	std::map<int, std::unique_ptr<sound>>			sounds;
	std::map<int, std::unique_ptr<music>>			musics;
};

}

#endif

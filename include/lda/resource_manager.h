#pragma once

#include "sound.h"
#include "music.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <map>
#include <memory>
#include <string>

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

	void        insert_sound(int, const std::string&);
	void        unload_sound(int);
	bool        has_sound(int _id) const {return sounds.count(_id);}
	void        insert_music(int, const std::string&);
	void        unload_music(int);
	bool        has_music(int _id) const {return musics.count(_id);}

	music&      get_music(int) const;
	sound&      get_sound(int) const;

	//!Returns the number of music files loaded.
	size_t      size_music() const {return musics.size();}
	//!Returns the number of sound files loaded.
	size_t      size_sound() const {return sounds.size();}

	private:

	std::map<int, std::unique_ptr<sound>>			sounds;
	std::map<int, std::unique_ptr<music>>			musics;
};

}

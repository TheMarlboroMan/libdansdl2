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

class resource_manager
{
	public:

	void 		insert_sound(int, const std::string&);
	void		insert_music(int, const std::string&);

	music& 		get_music(int);
	sound&		get_sound(int);

	private:

	std::map<int, std::unique_ptr<sound>>			sounds;
	std::map<int, std::unique_ptr<music>>			musics;
};

}

#endif

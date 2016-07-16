#include "sound_queue.h"

using namespace lda;

/* 
Lo ideal es que esto no sea sólo una cola de sonidos sino una cola de bloques
de sonido que incluyan cosas como el canal, el volumen o lo que sea. De momento
lo podemos dejar así pero me gustaría estudiarlo en el futuro.
*/

sound_queue::sound_queue()
{
	controller=audio_controller::get();
}

sound_queue::~sound_queue()
{
	controller=nullptr;
}

/*
OJO: El mero de hecho de llamar a este método (que se hace sólo nada más 
construir un objeto de esta clase) implica que se inicializará el controlador
de audio sdl.
*/

void sound_queue::clear()
{
	queue.clear();
}

//Los sonidos se insertan, preparados o no.

void sound_queue::insert(const sound_struct& p_e)
{
	queue.push_back(p_e);
}
	
sound_struct sound_queue::front()
{
	return queue.front();
}

void sound_queue::erase_front()
{
	queue.erase(std::begin(queue));
}

void sound_queue::process()
{
	if(controller && queue.size())
	{
		for(auto& c : queue)
		{
			controller->play_sound(c);
		}
		queue.clear();
	}
}

#ifndef PRIMITIVA_GRAFICA_PUNTOS_H
#define PRIMITIVA_GRAFICA_PUNTOS_H

#include "../representacion_primitiva.h"
#include <vector>

namespace DLibV
{
class Representacion_primitiva_puntos:
	public Representacion_primitiva
{
	public:
	
	void 					preparar();
	virtual bool 				es_estatica() const=0;

	bool 					volcado(SDL_Renderer *);
	bool 					volcado(SDL_Renderer *, const SDL_Rect&, const SDL_Rect&); 

						Representacion_primitiva_puntos(Uint8, Uint8, Uint8);
						Representacion_primitiva_puntos(int, int, Uint8, Uint8, Uint8);
						Representacion_primitiva_puntos(const Representacion_primitiva_puntos&);
	virtual 				~Representacion_primitiva_puntos();
						Representacion_primitiva_puntos& operator=(const Representacion_primitiva_puntos&);

	void 					insertar(int x, int y);

	protected:

	void 					generar_posicion();
	void 					preparar_posicion();

	private:

	std::vector<SDL_Point>			puntos;
};

class Representacion_primitiva_puntos_dinamica:
	public Representacion_primitiva_puntos
{
	public:

	bool es_estatica() const {return false;}

	Representacion_primitiva_puntos_dinamica(Uint8 pr, Uint8 pg, Uint8 pb):Representacion_primitiva_puntos(pr, pg, pb){}
	Representacion_primitiva_puntos_dinamica(int x, int y, Uint8 pr, Uint8 pg, Uint8 pb):Representacion_primitiva_puntos(x, y, pr, pg, pb){}
	Representacion_primitiva_puntos_dinamica(const Representacion_primitiva_puntos_dinamica& p_otra):Representacion_primitiva_puntos(p_otra) {}
	virtual ~Representacion_primitiva_puntos_dinamica() {}

	Representacion_primitiva_puntos_dinamica& operator=(const Representacion_primitiva_puntos_dinamica& p_otro)
	{
		Representacion_primitiva_puntos::operator=(p_otro);
		return *this;
	}
};

class Representacion_primitiva_puntos_estatica:
	public Representacion_primitiva_puntos
{
	public:

	bool es_estatica() const {return true;}

	Representacion_primitiva_puntos_estatica(Uint8 pr, Uint8 pg, Uint8 pb):Representacion_primitiva_puntos(pr, pg, pb){}
	Representacion_primitiva_puntos_estatica(int x, int y, Uint8 pr, Uint8 pg, Uint8 pb):Representacion_primitiva_puntos(x, y, pr, pg, pb){}
	Representacion_primitiva_puntos_estatica(const Representacion_primitiva_puntos_estatica& p_otra):Representacion_primitiva_puntos(p_otra) {}
	virtual ~Representacion_primitiva_puntos_estatica() {}
	Representacion_primitiva_puntos_estatica& operator=(const Representacion_primitiva_puntos_estatica& p_otro)
	{
		Representacion_primitiva_puntos::operator=(p_otro);
		return *this;
	}
};
}

#endif

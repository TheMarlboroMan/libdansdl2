#ifndef PRIMITIVA_GRAFICA_LINEA_H
#define PRIMITIVA_GRAFICA_LINEA_H

#include "../representacion_primitiva.h"

namespace DLibV
{
class Representacion_primitiva_linea:public Representacion_primitiva
{
	private:

	int x1, y1, x2, y2;	

	protected:

	void generar_posicion();
	void preparar_posicion();

	public:
	
	void preparar();
	virtual bool es_estatica() const=0;

	bool volcado(SDL_Renderer *);
	bool volcado(SDL_Renderer *, const SDL_Rect&, const SDL_Rect&); 

	Representacion_primitiva_linea(int x1, int y1, int x2, int y2, Uint8, Uint8, Uint8);
	Representacion_primitiva_linea(const Representacion_primitiva_linea&);
	virtual ~Representacion_primitiva_linea();
	Representacion_primitiva_linea& operator=(const Representacion_primitiva_linea&);

	void establecer_puntos(int x1, int y1, int x2, int y2);
};

class Representacion_primitiva_linea_dinamica:public Representacion_primitiva_linea
{
	public:

	bool es_estatica() const {return false;}

	Representacion_primitiva_linea_dinamica(int x1, int y1, int x2, int y2, Uint8 pr, Uint8 pg, Uint8 pb):Representacion_primitiva_linea(x1, y1, x2, y2, pr, pg, pb){}
	Representacion_primitiva_linea_dinamica(const Representacion_primitiva_linea_dinamica& p_otra):Representacion_primitiva_linea(p_otra) {}
	virtual ~Representacion_primitiva_linea_dinamica() {}
	Representacion_primitiva_linea_dinamica& operator=(const Representacion_primitiva_linea_dinamica& p_otro)
	{
		Representacion_primitiva_linea::operator=(p_otro);
		return *this;
	}
};

class Representacion_primitiva_linea_estatica:public Representacion_primitiva_linea
{
	public:

	bool es_estatica() const {return true;}

	Representacion_primitiva_linea_estatica(int x1, int y1, int x2, int y2, Uint8 pr, Uint8 pg, Uint8 pb):Representacion_primitiva_linea(x1, y1, x2, y2, pr, pg, pb){}
	Representacion_primitiva_linea_estatica(const Representacion_primitiva_linea_estatica& p_otra):Representacion_primitiva_linea(p_otra) {}
	virtual ~Representacion_primitiva_linea_estatica() {}
	Representacion_primitiva_linea_estatica& operator=(const Representacion_primitiva_linea_estatica& p_otro)
	{
		Representacion_primitiva_linea::operator=(p_otro);
		return *this;
	}
};
}

#endif

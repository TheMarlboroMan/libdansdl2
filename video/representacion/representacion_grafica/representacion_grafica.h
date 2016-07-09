#ifndef REPRESENTACION_GRAFICA_H
#define REPRESENTACION_GRAFICA_H

#include <GL/gl.h>
#include "../representacion.h"
#include "../../textura/textura.h"
#include "../../../herramientas/log_base/log_base.h"

/*
Es la clase base de todas las representaciones que usan un SDL_Texture 
(encapsulado en un recurso) para ser representado. 

Viene acompañada de una estructura de transformación que aprovecha algunas de las cualidades
de SDL_2*/

namespace DLibV
{

struct Representacion_grafica_transformacion
{
	bool invertir_horizontal;
	bool invertir_vertical;

	Representacion_grafica_transformacion():
		invertir_horizontal(false), invertir_vertical(false)
	{}

	bool es_transformacion() const {return invertir_horizontal || invertir_vertical;}

	void reiniciar()
	{
		invertir_horizontal=false;
		invertir_vertical=false;
	}
};

class Representacion_grafica:public Representacion
{
	public:

	enum 			FLAGS_RECT{FRECT_X=1, FRECT_Y=2, FRECT_W=4, FRECT_H=8};

				Representacion_grafica();
				Representacion_grafica(ColorRGBA);
				Representacion_grafica(const Representacion_grafica&);
				Representacion_grafica& operator=(const Representacion_grafica &);
	virtual 		~Representacion_grafica() {}

	Textura * ref_textura() const {return textura;}

	int			acc_w_textura() const {return textura->acc_w();}
	int			acc_h_textura() const {return textura->acc_h();}

	void 			transformar_invertir_horizontal(bool v) {transformacion.invertir_horizontal=v;}
	void 			transformar_invertir_vertical(bool v) {transformacion.invertir_vertical=v;}
	virtual void 		reiniciar_transformacion() {Representacion::reiniciar_transformacion(); transformacion.reiniciar();}
	virtual bool 		es_transformada() const {return Representacion::es_transformada() || transformacion.es_transformacion();}

	void			establecer_pincel(int w, int h) {pincel.w=w; pincel.h=h;}

	const Rect& 		acc_posicion() const {return this->posicion;}
	Rect 			copia_posicion() const {return Rect{posicion.x, posicion.y, posicion.w, posicion.h};}
	void			establecer_posicion(int, int, int=-1, int=-1, int=15);
	void 			establecer_posicion(Rect);

	const Rect& 		acc_recorte() const {return this->recorte;}
	Rect 			copia_recorte() const {return Rect {recorte.x, recorte.y, recorte.w, recorte.h};}
	void 			establecer_recorte(Sint16, Sint16, Uint16, Uint16, int=15);
	void 			establecer_recorte(Rect);
	void 			establecer_dimensiones_posicion_por_recorte();

	Representacion_grafica_transformacion& acc_transformacion() {return transformacion;}

	virtual void 		establecer_textura(Textura const * p_textura) {this->textura=const_cast <Textura *> (p_textura);}

	virtual void 		ir_a(int x, int y);
	virtual Punto		obtener_posicion() const;
	virtual Rect		obtener_base_posicion_vista() const;

	private:

	Representacion_grafica_transformacion transformacion;
	void			calcular_puntos();

	//TODO: Change this for a reference. Or not: we wouldn't be able to change textures.
	Textura * textura;	//Este es el puntero a su superficie de memoria.

	struct 	{int w, h;}	pincel;
	struct puntotex {GLfloat x, y;};
	struct punto{int x, y;};

	std::vector<punto>	puntos;
	std::vector<puntotex>	final_ptex;


	protected:

	Rect 			posicion; 	//Lugar en que se muestra de la pantalla.
	Rect	 		recorte;	//Considerando la dimensión total de la representación, la parte que mostramos.

	void 			recorte_a_medidas_textura();
	void 			liberar_textura();
	void 			anular_textura() {textura=nullptr;}

	virtual void 		volcado();
};

} //Fin namespace DLibV

#endif

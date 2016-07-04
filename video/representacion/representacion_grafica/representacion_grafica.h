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

	float angulo_rotacion;
	float x_centro_rotacion;
	float y_centro_rotacion;

	Representacion_grafica_transformacion():
		invertir_horizontal(false), invertir_vertical(false),
		angulo_rotacion(0.f), x_centro_rotacion(0.f), y_centro_rotacion(0.f)
	{}

	//TODO... I don't want SDL here.
	SDL_Point obtener_centro_rotacion() const
	{
		SDL_Point p;
		p.x=x_centro_rotacion;
		p.y=y_centro_rotacion;
		return p;
	}

	bool es_transformacion() const {return invertir_horizontal || invertir_vertical || angulo_rotacion!=0.f;}
	bool es_cambia_centro_rotacion() const {return x_centro_rotacion!=0.f || y_centro_rotacion!=0.f;}
	void centro_rotacion(float px, float py)
	{
		x_centro_rotacion=px;
		y_centro_rotacion=py;
	}

	void cancelar_centro_rotacion()
	{
		x_centro_rotacion=0.f;
		y_centro_rotacion=0.f;
	}

	void reiniciar()
	{
		invertir_horizontal=false;
		invertir_vertical=false;
		angulo_rotacion=0.f;
		x_centro_rotacion=0.f;
		y_centro_rotacion=0.f;
	}
};

class Representacion_grafica:public Representacion
{
	public:

				Representacion_grafica();
				Representacion_grafica(ColorRGBA);
				Representacion_grafica(const Representacion_grafica&);
				Representacion_grafica& operator=(const Representacion_grafica &);
	virtual 		~Representacion_grafica();

	Rect 		copia_posicion_rotada() const;
	Textura * ref_textura() const {return textura;}
	virtual Rect		obtener_caja_clip() const {return acc_posicion();}
	int			acc_w_textura() const {return textura->acc_w();}
	int			acc_h_textura() const {return textura->acc_h();}			

	void 			reiniciar_transformacion() {transformacion.reiniciar();}
	void 			transformar_invertir_horizontal(bool v) {transformacion.invertir_horizontal=v;}
	void 			transformar_invertir_vertical(bool v) {transformacion.invertir_vertical=v;}

	void 			transformar_rotar(float v);
	void 			transformar_cancelar_rotar();
	void 			transformar_centro_rotacion(float x, float y);
	void 			transformar_centro_rotacion_cancelar();
	bool 			es_transformada() const {return transformacion.es_transformacion();}
	void			establecer_pincel(int w, int h) {pincel.w=w; pincel.h=h;}

	virtual void		establecer_posicion(int, int, int=-1, int=-1, int=15);
	virtual void 		establecer_posicion(Rect);

	Representacion_grafica_transformacion& acc_transformacion() {return transformacion;}

	virtual void 		establecer_textura(Textura const * p_textura) {this->textura=const_cast <Textura *> (p_textura);}

	private:

	Representacion_grafica_transformacion transformacion;

	//TODO: Change this for a reference. Or not: we wouldn't be able to change textures.
	Textura * textura;	//Este es el puntero a su superficie de memoria.

	//Copia de posición alterada según rotación. La usaremos para ver si estamos en cámara.
	Rect	 		posicion_rotada;
	struct 	{int w, h;}	pincel;

	protected:

	void 			recorte_a_medidas_textura();
	void 			liberar_textura();
	void 			anular_textura() {textura=nullptr;}
	void 			actualizar_caja_rotacion();

	virtual void 		volcado();
};

} //Fin namespace DLibV

#endif

#ifndef REPRESENTACION_H
#define REPRESENTACION_H

#include <SDL2/SDL.h>
#include "../color/color.h"
#include "../rect/rect.h"
#include "../camara/camara.h"
#include "../info_volcado/info_volcado.h"
#include "../pantalla/pantalla.h"
#include "../../herramientas/poligono_2d/poligono_2d.h"

namespace DLibV
{

struct Representacion_transformacion
{
	float 				angulo_rotacion;
	DLibH::Punto_2d<float>		centro_rotacion;

	Representacion_transformacion():
		angulo_rotacion(0.f), centro_rotacion(0.f, 0.f)
	{}

	bool 				es_transformacion() const {return angulo_rotacion!=0.f;}
	bool 				es_cambia_centro_rotacion() const {return centro_rotacion.x!=0.f || centro_rotacion.y!=0.f;}

	void 				reiniciar()
	{
		angulo_rotacion=0.f;
		centro_rotacion.x=0.f;
		centro_rotacion.y=0.f;
	}
};

class Representacion
{
	public:

	enum class blends {BLEND_NADA,BLEND_ALPHA};
	enum FLAGS_RECT{FRECT_X=1, FRECT_Y=2, FRECT_W=4, FRECT_H=8};

				Representacion();
				Representacion(ColorRGBA);
				Representacion(const Representacion&);
	Representacion& 	operator=(const Representacion &);
	virtual 		~Representacion() {}

	bool 			en_toma(const Camara& p_cam) const;
	bool 			es_en_posicion(Sint16 p_x, Sint16 p_y) const {return posicion.x==p_x && posicion.y==p_y;}

	Rect 			copia_posicion_rotada() const;
	const Rect& 		acc_posicion() const {return this->posicion;}
	Rect 			copia_posicion() const {return Rect{posicion.x, posicion.y, posicion.w, posicion.h};}
	virtual void		establecer_posicion(int, int, int=-1, int=-1, int=15);
	virtual void 		establecer_posicion(Rect);

	const Rect& 		acc_recorte() const {return this->recorte;}
	Rect 			copia_recorte() const {return Rect {recorte.x, recorte.y, recorte.w, recorte.h};}
	void 			establecer_recorte(Sint16, Sint16, Uint16, Uint16, int=15);
	void 			establecer_recorte(Rect);
	void 			establecer_dimensiones_posicion_por_recorte();

	//TODO: Really???. Check this, I mean... the virtual thing.
	virtual void 		ir_a(int x, int y){establecer_posicion(x,y);} //Es virtual porque algunas igual redefinen el comportamiento (especialmente las primitivas)....
	void 			desplazar(Sint16 p_x, Sint16 p_y);
	void 			hacer_invisible() {this->visible=false;}
	void 			hacer_visible() {this->visible=true;}
	void 			intercambiar_visibilidad() {this->visible=!this->visible;}
	void 			cambiar_visibilidad(bool p_valor) {this->visible=p_valor;}
	bool 			es_visible() const {return this->visible;}

	void 			volcar(Pantalla&, const Camara&);
	void 			volcar(Pantalla&);

	void			mut_rgba(ColorRGBA v) {rgba=v;}
	void 			establecer_alpha(unsigned int v) {rgba.a=colorfi(v);}
	void 			establecer_color(unsigned int pr, unsigned int pg, unsigned int pb)
	{
		rgba.r=colorfi(pr);
		rgba.g=colorfi(pg);
		rgba.b=colorfi(pb);
	}

	ColorRGBA		acc_rgba() const {return rgba;}
	unsigned short int	acc_alpha() const {return colorfi(rgba.a);}
	unsigned short int 	acc_color_r() const {return colorfi(rgba.r);}
	unsigned short int 	acc_color_g() const {return colorfi(rgba.g);}
	unsigned short int 	acc_color_b() const {return colorfi(rgba.b);}

	void 			establecer_modo_blend(blends v) {this->modo_blend=v;}
	blends		 	acc_modo_blend() const {return this->modo_blend;}

	virtual void 		reiniciar_transformacion() {transformacion.reiniciar();}

	void 			transformar_rotar(float v);
	void 			transformar_cancelar_rotar();
	void 			transformar_centro_rotacion(float x, float y);
	void 			transformar_centro_rotacion_cancelar();
	virtual bool 		es_transformada() const {return transformacion.es_transformacion();}

	private:

	void			transformacion_pre_render(const Info_volcado);

	Representacion_transformacion transformacion;

	bool 			visible;
	blends		 	modo_blend;
	ColorRGBA		rgba;
	Rect 			posicion; 	//Lugar en que se muestra de la pantalla.
	Rect	 		recorte;	//Considerando la dimensión total de la representación, la parte que mostramos.
	Rect	 		posicion_rotada;

	protected:

	void 			reiniciar_posicion();
	void 			reiniciar_recorte();
	void 			reiniciar_rect(Rect&);
	void 			actualizar_caja_rotacion();

	virtual void 		volcado(const Info_volcado)=0;
};

} //Fin namespace DLibV

#endif

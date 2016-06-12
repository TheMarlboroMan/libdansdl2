#ifndef REPRESENTACION_H
#define REPRESENTACION_H

#include <SDL2/SDL.h>
#include "../color/color.h"
#include "../rect/rect.h"
#include "../camara/camara.h"
#include "../pantalla/pantalla.h"
#include "../../herramientas/herramientas_sdl/herramientas_sdl.h"

namespace DLibV
{

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

	//TODO: Is this ever used???
	bool 			en_toma(const Rect& p_caja) const {return posicion.es_en_colision_con(p_caja, true);}
	bool 			es_en_posicion(Sint16 p_x, Sint16 p_y) const 
	{
		return this->posicion.x==p_x && 
		this->posicion.y==p_y;
	}

	const Rect& 		acc_posicion() const {return this->posicion;}
	Rect 			copia_posicion() const {return Rect{posicion.x, posicion.y, posicion.w, posicion.h};}
	virtual void		establecer_posicion(int, int, int=-1, int=-1, int=15);
	virtual void 		establecer_posicion(Rect);

	const Rect& 		acc_recorte() const {return this->recorte;}
	Rect 			copia_recorte() const {return Rect {recorte.x, recorte.y, recorte.w, recorte.h};}
	void 			establecer_recorte(Sint16, Sint16, Uint16, Uint16, int=15);
	void 			establecer_recorte(Rect);
	void 			establecer_dimensiones_posicion_por_recorte();

	virtual void 		ir_a(int x, int y){establecer_posicion(x,y);} //Es virtual porque algunas igual redefinen el comportamiento (especialmente las primitivas....
	void 			desplazar(Sint16 p_x, Sint16 p_y);					
	void 			hacer_invisible() {this->visible=false;}
	void 			hacer_visible() {this->visible=true;}
	void 			intercambiar_visibilidad() {this->visible=!this->visible;}
	void 			cambiar_visibilidad(bool p_valor) {this->visible=p_valor;}
	bool 			es_visible() const {return this->visible;}

	//Se pasa el rectángulo de pantalla... Básicamente se comprueba si está dentro. Estática o no.

	void 			volcar(const Pantalla&, const Camara&);
	void 			volcar(const Pantalla&);

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

	//TODO: Ya veremos...
	static void 		procesar_zoom(Rect& pos, const Rect& p_posicion, const Rect& p_enfoque);
	static void 		procesar_zoom(Rect&, double);

	private:

	bool 			visible;
	blends		 	modo_blend;
	ColorRGBA		rgba;
	Rect 			posicion; 	//Lugar en que se muestra de la pantalla.
	Rect	 		recorte;	//Considerando la dimensión total de la representación, la parte que mostramos.

	protected:

	void 			reiniciar_posicion();
	void 			reiniciar_recorte();
	void 			reiniciar_rect(Rect&);

	virtual void 		volcado()=0;
};

} //Fin namespace DLibV

#endif

#ifndef REPRESENTACION_H
#define REPRESENTACION_H

#include <SDL2/SDL.h>
#include "../color/color.h"
#include "../camara/camara.h"
#include "../pantalla/pantalla.h"
#include "../../herramientas/herramientas_sdl/herramientas_sdl.h"

namespace DLibV
{

class Representacion
{
	public:

	enum blends {BLEND_NADA,BLEND_ALPHA,BLEND_SUMA,BLEND_MODULAR};
	enum FLAGS_RECT{FRECT_X=1, FRECT_Y=2, FRECT_W=4, FRECT_H=8};

				Representacion();
				Representacion(ColorRGBA);
				Representacion(const Representacion&);
	Representacion& 	operator=(const Representacion &);
	virtual 		~Representacion() {}

	bool 			en_toma(const SDL_Rect& p_caja) const {return DLibH::Herramientas_SDL::rectangulos_superpuestos(p_caja, this->posicion, true);} 
	bool 			es_en_posicion(Sint16 p_x, Sint16 p_y) const 
	{
		return this->posicion.x==p_x && 
		this->posicion.y==p_y;
	}

	const SDL_Rect& 	acc_posicion() const {return this->posicion;}
	SDL_Rect 		copia_posicion() const {return SDL_Rect{posicion.x, posicion.y, posicion.w, posicion.h};}
	virtual void		establecer_posicion(int, int, int=-1, int=-1, int=15);
	virtual void 		establecer_posicion(SDL_Rect);

	const SDL_Rect& 	acc_recorte() const {return this->recorte;}
	SDL_Rect 		copia_recorte() const {return SDL_Rect {recorte.x, recorte.y, recorte.w, recorte.h};}
	void 			establecer_recorte(Sint16, Sint16, Uint16, Uint16, int=15);
	void 			establecer_recorte(SDL_Rect);
	void 			establecer_dimensiones_posicion_por_recorte();

	virtual void 		ir_a(int x, int y){establecer_posicion(x,y);} //Es virtual porque algunas igual redefinen el comportamiento (especialmente las primitivas....
	void 			desplazar(Sint16 p_x, Sint16 p_y);					
	void 			hacer_invisible() {this->visible=false;}
	void 			hacer_visible() {this->visible=true;}
	void 			intercambiar_visibilidad() {this->visible=!this->visible;}
	void 			cambiar_visibilidad(bool p_valor) {this->visible=p_valor;}
	bool 			es_visible() const {return this->visible;}

	//Se pasa el rectángulo de pantalla... Básicamente se comprueba si está dentro. Estática o no.
	void 			volcar(SDL_Renderer *, const SDL_Rect&);
	void 			volcar(SDL_Renderer *, const Camara&);
	void 			volcar(const Pantalla&, const Camara&);
	void 			volcar(const Pantalla&);
	void 			volcar(SDL_Renderer *, const SDL_Rect&, const SDL_Rect&, double);
	void 			volcar(SDL_Renderer *);

	void			mut_rgba(ColorRGBA v) {rgba=v;}
	void 			establecer_alpha(unsigned int v) {rgba.a=colorif(v);}
	void 			establecer_color(unsigned int pr, unsigned int pg, unsigned int pb)
	{
		rgba.r=colorif(pr);
		rgba.g=colorif(pg);
		rgba.b=colorif(pb);
	}

	ColorRGBA		acc_rgba() const {return rgba;}
	unsigned short int	acc_alpha() const {return colorfi(rgba.a);}
	unsigned short int 	acc_color_r() const {return colorfi(rgba.r);}
	unsigned short int 	acc_color_g() const {return colorfi(rgba.g);}
	unsigned short int 	acc_color_b() const {return colorfi(rgba.b);}

	void 			establecer_modo_blend(unsigned short int v) {this->modo_blend=v;}
	unsigned short int 	acc_modo_blend() const {return this->modo_blend;}

	//TODO: Ya veremos...
	static void 		procesar_zoom(SDL_Rect& pos, const SDL_Rect& p_posicion, const SDL_Rect& p_enfoque);
	static void 		procesar_zoom(SDL_Rect&, double);

	private:

	bool 			visible;
	unsigned short int 	modo_blend;
	ColorRGBA		rgba;
	SDL_Rect 		posicion; 	//Lugar en que se muestra de la pantalla.
	SDL_Rect 		recorte;	//Considerando la dimensión total de la representación, la parte que mostramos.

	protected:

	void 			reiniciar_posicion();
	void 			reiniciar_recorte();
	void 			reiniciar_rect(SDL_Rect&);

	virtual void 		volcado(SDL_Renderer *, const SDL_Rect&, const SDL_Rect&, double)=0;
	virtual void 		volcado(SDL_Renderer *)=0;
};

} //Fin namespace DLibV

#endif

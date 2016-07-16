#ifndef LIBDANSDL2_RASTER_REPRESENTATION_H
#define LIBDANSDL2_RASTER_REPRESENTATION_H

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include "../representation.h"
#include "../../texture/texture.h"
#include "../../../tools/log/log.h"

/*
Es la clase base de todas las representaciones que usan una texture_instance
(encapsulada en un recurso) para ser representado. 

Viene acompañada de una estructura de transformación para su rotación.
*/

namespace ldv
{

struct invert_transform
{
	bool 		horizontal,
		 	vertical;

	invert_transform():
		horizontal(false), vertical(false)
	{}

	bool is_transformed() const {return horizontal || vertical;}

	void reset()
	{
		horizontal=false;
		vertical=false;
	}
};

class raster_representation:
	public representation
{
	public:

	enum 			flags_rect{frx=1, fry=2, frw=4, frh=8};
	enum class		sampling{
					atlas, 	//Añade medio píxel al sampling.
					complete}; //No añade nada.

				raster_representation(rect={0,0,0,0}, rect={0,0,0,0}, sampling=sampling::complete);
				raster_representation(rgba_color, rect={0,0,0,0}, rect={0,0,0,0}, sampling=sampling::complete);
				raster_representation(const raster_representation&);
				raster_representation& operator=(const raster_representation &);
	virtual 		~raster_representation() {}

	texture * 		get_texture() const {return texture_instance;}
	int			get_w_texture_instance() const {return texture_instance!=nullptr ? texture_instance->get_w() : -1;}
	int			get_h_texture_instance() const {return texture_instance!=nullptr ? texture_instance->get_h() : -1;}
	
	const rect& 		get_location() const {return location;}
	rect 			get_location() {return location;}
	const rect& 		get_clip() const {return clip;}
	rect 			get_clip() {return clip;}
	const invert_transform& get_transformation_invert() const {return transformation;}

	virtual bool 		is_transformed() const {return representation::is_transformed() || transformation.is_transformed();}

	void 			set_invert_horizontal(bool v) {transformation.horizontal=v;}
	void 			set_invert_vertical(bool v) {transformation.vertical=v;}
	void			set_sampling(sampling v) {reset_calculations(); sampling_type=v;}
	void			set_brush(int w, int h) {brush.w=w; brush.h=h;}
	void			set_location(int, int, int=-1, int=-1, int=15);
	void 			set_location(rect);
	void 			set_clip(Sint16, Sint16, Uint16, Uint16, int=15);
	void 			set_clip(rect);
	void 			set_texture(texture const * v) {texture_instance=const_cast <texture *> (v);}

	virtual void 		reset_transform() {representation::reset_transform(); transformation.reset();}

	void 			clip_to_texture();

	virtual void 		go_to(int x, int y);
	virtual point		get_position() const;
	virtual rect		get_base_view_position() const;

	private:

	struct puntotex {GLfloat x, y;};

	void			calculate_points();


	invert_transform 	transformation;

	texture * 		texture_instance;	//Este es el puntero a su superficie de memoria. Es un puntero y no una referencia para permitir cambiarlo.

	struct 	{int w, h;}	brush;

	std::vector<point>	points;
	std::vector<puntotex>	tex_points;
	sampling		sampling_type;

	protected:

	rect 			location; 	//Lugar en que se muestra de la pantalla.
	rect	 		clip;	//Considerando la dimensión total de la representación, la parte que mostramos.

	void			reset_calculations();
	void 			free_texture();
	void 			reset_texture() {texture_instance=nullptr;}

	virtual void 		do_draw();
};

} //Fin namespace DLibV

#endif

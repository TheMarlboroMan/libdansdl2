#ifndef LIBDANSDL2_REPRESENTATION_H
#define LIBDANSDL2_REPRESENTATION_H

#include <SDL2/SDL.h>
#include "../color/color.h"
#include "../rect/rect.h"
#include "../camera/camera.h"
#include "../draw_info/draw_info.h"
#include "../screen/screen.h"
#include "../../tools/polygon_2d/polygon_2d.h"

namespace ldv
{

struct rotation_transform
{
	float 				angle;
	ldt::point_2d<float>		center;

	rotation_transform():
		angle(0.f), center(0.f, 0.f)
	{}

	bool 				is_transformed() const {return angle!=0.f;}
	bool 				is_center_displaced() const {return center.x!=0.f || center.y!=0.f;}

	void 				reset()
	{
		angle=0.f;
		center.x=0.f;
		center.y=0.f;
	}
};

class representation
{
	public:

	enum class 		blends{none, alpha};
	enum 			alpha{alpha_min=0, alpha_max=255};

				representation(int=alpha_max);
				representation(const representation&);
	representation& 	operator=(const representation &);
	virtual 		~representation() {}

	blends		 	get_blend() const {return blend_mode;}
	const rect&		get_view_position() const {return view_position;}
	int			get_alpha() const {return colorif(val_alpha);}
	float			get_alphaf() const {return val_alpha;}
	const rotation_transform& get_transformation_rotation() const {return transformation;}

	bool 			is_in_focus(const rect& prect) const {return prect.collides_with(view_position, true);}
	bool 			is_visible() const {return visible;}
	virtual bool 		is_transformed() const {return transformation.is_transformed();}

	void 			set_rotation(float v);
	void 			set_rotation_center(float x, float y);
	void 			set_visible(bool v) {visible=v;}
	void 			set_blend(blends v) {blend_mode=v;}
	void 			set_alpha(int v) {val_alpha=colorfi(v);}
	void			set_alphaf(float v) {val_alpha=v;}

	virtual void 		reset_transform() {transformation.reset();}
	void			toogle_visible() {visible=!visible;}
	void 			go_to(point p) {go_to(p.x, p.y);}


	//Estas hay que definirlas.
	virtual void 		go_to(int x, int y)=0;
	virtual point		get_position() const=0;
	virtual rect		get_base_view_position() const=0;
	void			debug_trace_box() const;

	//Virtuales por el caso especial de las agrupadas...
	virtual void		draw(screen&, const camera&, bool=false);
	virtual void		draw(screen&, bool=false);

	private:

	rotation_transform 	transformation;

	bool 			visible;
	blends		 	blend_mode;

	float			val_alpha;
	rect	 		view_position;

	protected:
	
	void 			update_view_position();
	rect			calculate_view_position() const;
	virtual void 		do_draw()=0;
	void			pre_render_transform(const draw_info&);
};

} //Fin namespace DLibV

#endif

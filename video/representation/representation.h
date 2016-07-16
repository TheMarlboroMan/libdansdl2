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

				representation();
				representation(rgba_color);
				representation(const representation&);
	representation& 	operator=(const representation &);
	virtual 		~representation() {}

	blends		 	get_blend() const {return blend_mode;}
	const rect&		get_view_position() const {return view_position;}
	rgba_color		get_rgba() const {return rgba;}
	int			get_alpha() const {return colorif(rgba.a);}
	const rotation_transform& get_transformation_rotation() const {return transformation;}

	bool 			is_in_focus(const rect& rect) const {return rect.collides_with(view_position, true);}
	bool 			is_visible() const {return visible;}
	virtual bool 		is_transformed() const {return transformation.is_transformed();}

	void 			set_rotation(float v);
	void 			set_rotation_center(float x, float y);
	void 			set_visible(bool v) {visible=v;}
	void 			set_blend(blends v) {blend_mode=v;}
	void			set_rgba(rgba_color v) {rgba=v;}
	void 			set_alpha(unsigned int v) {rgba.a=colorfi(v);}
	void 			set_color(unsigned int pr, unsigned int pg, unsigned int pb)
	{
		rgba.r=colorfi(pr);
		rgba.g=colorfi(pg);
		rgba.b=colorfi(pb);
	}

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
	rgba_color		rgba;
	rect	 		view_position;

	protected:
	
	void 			update_view_position();
	rect			calculate_view_position() const;
	virtual void 		do_draw()=0;
	void			pre_render_transform(const draw_info&);
};

} //Fin namespace DLibV

#endif

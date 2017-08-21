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

//!Structure that contains rotation information.

//!Provides with a few convenience functions. Used by representations.

struct rotation_transform
{
	float 				angle;		//!< Rotation angle. Defaults to 0.f
	ldt::point_2d<float>		center;		//!< Center of rotation. Defaults to 0,0.

	rotation_transform():
		angle(0.f), center(0.f, 0.f)
	{}

	//!Checks whether any transformation has taken place.
	bool 				is_transformed() const {return angle!=0.f;}
	//!Checks if the center is displaced from its original 0.0 position.
	bool 				is_center_displaced() const {return center.x!=0.f || center.y!=0.f;}
	//!Resets center and angle.
	void 				reset()
	{
		angle=0.f;
		center.x=0.f;
		center.y=0.f;
	}
};

//!Base class for all representations. Defines its most basic parameters.
/**	- Blend mode
	- Alpha value
	- Rotation data.
	- Visibility.
**/

class representation
{
	public:

	//!Indicates blend modes.
	enum class 		blends{none, alpha};
	//!Quick access to minimum and maximum alpha. Minimum is fully transparent.
	enum 			alpha{alpha_min=0, alpha_max=255};

	//!By default all representations have 255 as alpha value.
				representation(int=alpha_max);
				representation(const representation&);
	representation& 	operator=(const representation &);
	virtual 		~representation() {}

	//!Gets blend mode.
	blends		 	get_blend() const {return blend_mode;}
	//!Gets the rectangle that defines its borders (may change when rotated). Used by the camera.
	const rect&		get_view_position() const {return view_position;}
	//!Gets the alpha value in 0-255 range.
	int			get_alpha() const {return colorif(val_alpha);}
	//!Gets the alpha value in 0.f-1.f range.
	float			get_alphaf() const {return val_alpha;}
	//!Gets rotation data.
	const rotation_transform& get_transformation_rotation() const {return transformation;}
	//!Indicates if is visible. An invisible representation is ignored.
	bool 			is_visible() const {return visible;}
	//!Indicates if any rotation transformation has happened (rotation center or angle).
	virtual bool 		is_transformed() const {return transformation.is_transformed();}
	void 			set_rotation(float v);
	void 			set_rotation_center(float x, float y);
	//!Sets the visibility flag.
	void 			set_visible(bool v) {visible=v;}
	//!Sets the blend mode. Alpha codified textures will not work if alpha is not selected. Alpha itself won't work too.
	void 			set_blend(blends v) {blend_mode=v;}
	//!Sets the alpha value in 0-255 range.
	void 			set_alpha(int v) {val_alpha=colorfi(v);}
	//!Sets the alpha value in 0.f-1.f range.
	void			set_alphaf(float v) {val_alpha=v;}
	//!Resets rotation transformation data.
	virtual void 		reset_transform() {transformation.reset();}
	void			debug_trace_box() const;
	//!Toogles visibility flag.
	void			toogle_visible() {visible=!visible;}
	//!Moves the top-left point of the view position to the given point.
	virtual void 		go_to(point p)=0;
	//!Each derived class must define how to get the position.
	virtual point		get_position() const=0;
	//!Each derived class must define how to get the base_view_position (the one without rotation transformations).
	virtual rect		get_base_view_position() const=0;

	//These are virtuals because grouped representations need it.
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
	void			pre_render_transform(const draw_info&);

	//!Real drawing functions must be defined by each subclass.
	virtual void 		do_draw()=0;
};

} //Fin namespace DLibV

#endif

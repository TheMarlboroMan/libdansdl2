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

/**Structure that contains rotation information and provides with a few
convenience functions. Used by representations.
**/

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

///Base class for all representations. Defines its most basic parameters.
/**	- Blend mode
	- Alpha value
	- Rotation data.
	- Visibility.
**/

class representation
{
	public:

	///Indicates blend modes.
	enum class 		blends{none, alpha};
	enum 			alpha{alpha_min=0, alpha_max=255};

				representation(int=alpha_max);
				representation(const representation&);
	representation& 	operator=(const representation &);
	virtual 		~representation() {}

	///Gets blend mode.
	blends		 	get_blend() const {return blend_mode;}
	///Gets the rectangle that defines its borders (may change when rotated). Used by the camera.
	const rect&		get_view_position() const {return view_position;}
	///Gets the alpha value in 0-255 range.
	int			get_alpha() const {return colorif(val_alpha);}
	///Gets the alpha value in 0.f-1.f range.
	float			get_alphaf() const {return val_alpha;}
	///Gets rotation data.
	const rotation_transform& get_transformation_rotation() const {return transformation;}
	///Indicates if the view position collides with the given rect. Used by cameras.
	bool 			is_in_focus(const rect& prect) const {return prect.collides_with(view_position, true);}
	///Indicates if is visible. An invisible representation is ignored.
	bool 			is_visible() const {return visible;}
	///Indicates if any rotation transformation has happened (rotation center or angle).
	virtual bool 		is_transformed() const {return transformation.is_transformed();}
	///Sets rotation angle (0.f-360.f).
	void 			set_rotation(float v);
	///Sets the rotation center.
	void 			set_rotation_center(float x, float y);
	///Sets the visibility flag.
	void 			set_visible(bool v) {visible=v;}
	///Sets the blend mode. Alpha codified textures will not work if alpha is not selected. Alpha itself won't work too.
	void 			set_blend(blends v) {blend_mode=v;}
	///Sets the alpha value in 0-255 range.
	void 			set_alpha(int v) {val_alpha=colorfi(v);}
	///Sets the alpha value in 0.f-1.f range.
	void			set_alphaf(float v) {val_alpha=v;}
	///Resets rotation transformation data.
	virtual void 		reset_transform() {transformation.reset();}
	///Directly uses openGL to trace the view position.
	void			debug_trace_box() const;
	///Toogles visibility flag.
	void			toogle_visible() {visible=!visible;}
	///Moves the top-left point of the view position to the given point.
	void 			go_to(point p) {go_to(p.x, p.y);}

	///Each derived class must define how to move the position.
	virtual void 		go_to(int x, int y)=0;
	///Each derived class must define how to get the position.
	virtual point		get_position() const=0;
	///Each derived class must define how to get the base_view_position (the one without rotation transformations).
	virtual rect		get_base_view_position() const=0;

	//These are virtuals because grouped representations need it.
	///Draws to screen with camera. The third parameter skips camera checks and draws anyway.
	virtual void		draw(screen&, const camera&, bool=false);
	///Draws to screen. The second parameter skip window bound tests and tries to draw anyway.
	virtual void		draw(screen&, bool=false);

	private:

	rotation_transform 	transformation;

	bool 			visible;
	blends		 	blend_mode;

	float			val_alpha;
	rect	 		view_position;

	protected:
	
	///Updates the view_position rect with the base or transformed rects.
	void 			update_view_position();
	///Calculates the view position when rotation transformations are involved.
	rect			calculate_view_position() const;
	///Applies all SDL transformations of zoom, position and rotation.
	void			pre_render_transform(const draw_info&);

	///Real drawing functions must be defined by each subclass.
	virtual void 		do_draw()=0;
};

} //Fin namespace DLibV

#endif

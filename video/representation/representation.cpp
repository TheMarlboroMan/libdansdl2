#include "representation.h"

#include <iostream>
#include <algorithm>

using namespace ldv;

representation::representation(int valpha):
	visible(true),
	blend_mode(blends::none), 
	val_alpha{colorfi(valpha)},
	view_position{0,0,0,0}
{

}

representation::representation(const representation& o):
	visible(o.visible), 
	blend_mode(blends::none),
	val_alpha(o.val_alpha),
	view_position(o.view_position)
{

}

representation& representation::operator=(const representation& o)
{
	visible=o.visible;
	val_alpha=o.val_alpha;
	view_position=o.view_position;
	blend_mode=o.blend_mode;
	return *this;
}

//!Draws to screen with camera. The third parameter skips camera checks and draws anyway.

//!This function actually delegates to do_draw on each derived class.
//!skip_take is mostly useful for group repesentations, specially when rotations 
//are applied to them.

void representation::draw(screen& pscreen, const camera& pcamera, bool skip_take)
{
	if(visible && (skip_take || is_in_focus(pcamera.get_focus_box()))) 
	{
		pscreen.set_camera(pcamera);
		pre_render_transform(pcamera.get_draw_info());
		do_draw();
	}

	//Do not forget this.
	glLoadIdentity();
}

//!Draws to screen with no camera bounds.

//!This function actually delegates to do_draw on each derived class.
//!The second parameter skips window bound tests and tries to draw anyway.

void representation::draw(screen& pscreen, bool skip_take)
{
	auto in_screen=[this](screen &screen)
	{
		int ex=view_position.origin.x+view_position.w,
			ey=view_position.origin.y+view_position.h;

		return 	ex >= 0
			&& view_position.origin.x <= (int) screen.get_w()
			&& ey >= 0
			&& view_position.origin.y <= (int) screen.get_h();
	};

	if(visible && (skip_take || in_screen(pscreen)))
	{
		if(pscreen.has_camera())
		{
			pscreen.reset_clip();
		}

		pre_render_transform(pscreen.get_draw_info());
		do_draw();
	}

	//Es importante que esto siempre esté presente...
	glLoadIdentity();
}

//!Directly uses openGL to trace the view position.

//!This function should not be used beyond debug purposes.

void representation::debug_trace_box() const
{
	auto rect=calculate_view_position();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.f, 0.f, 0.f, 0.25f);
	glEnableClientState(GL_VERTEX_ARRAY);

	int 	fx=rect.origin.x+rect.w,
		fy=rect.origin.y+rect.h;

	std::vector<point> points{ {rect.origin.x, rect.origin.y},
			{fx, rect.origin.y},
			{fx, fy},
			{rect.origin.x, fy}};

	glVertexPointer(2, GL_INT, 0, points.data());
	glDrawArrays(GL_POLYGON, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

//!Applies all SDL transformations of zoom, position and rotation.

void representation::pre_render_transform(const draw_info& iv)
{
	glMatrixMode(GL_MODELVIEW);
	
	auto pos=get_position();
	int 	x=iv.pos_x+pos.x-iv.rel_x, 
		y=iv.pos_y+pos.y-iv.rel_y;

	//Compensating the movement that zoom and scale causes... We displace
	//one half, two thirds, three fourths...
	if(iv.zoom!=1.0)
	{
		glScaled(iv.zoom, iv.zoom, 1.0);
		//Empirical knowledge.
		glTranslatef((iv.pos_x / iv.zoom) - iv.pos_x, (iv.pos_y / iv.zoom) - iv.pos_y, 0.f);
	}

	glTranslatef(x, y, 0.f);

	if(transformation.angle != 0.f)
	{
		glTranslatef(transformation.center.x, transformation.center.y, 0.f);
		glRotatef(transformation.angle, 0.f, 0.f, 1.f);
		glTranslatef(-transformation.center.x, -transformation.center.y, 0.f);
	}
}

//!Sets rotation angle (0.f-360.f).

//!Rotated representations have different view_position boxes that 
//!not transformed ones.

void representation::set_rotation(float v) 
{
	transformation.angle=fmod(v, 360.f);
	update_view_position();
}

//!Sets the rotation center

//!The rotation center is the axis around which a representation rotates. By
//!default it is its top-left corner.

void representation::set_rotation_center(float x, float y) 
{
	transformation.center.x=x;
	transformation.center.y=y;
	update_view_position();
}

//!Updates the view_position rect with the base or transformed rects.

void representation::update_view_position()
{
	if(!transformation.is_transformed())
	{
		view_position=get_base_view_position();
	}
	else
	{
		view_position=calculate_view_position();
	}
}

//!Calculates the view position when rotation transformations are involved.

rect representation::calculate_view_position() const
{
	const auto p=get_base_view_position();
	const auto pos=get_position();

	auto c=transformation.center;

	polygon_2d_vertexes<double> polig(
		{ 
			{(double)p.origin.x, (double)p.origin.y},
			{(double)(p.origin.x+p.w), (double)p.origin.y},
			{(double)(p.origin.x+p.w), (double)(p.origin.y+p.h)},
			{(double)p.origin.x, (double)(p.origin.y+p.h)},        
		}, {(double)c.x+pos.x, (double)c.y+pos.y});
		//Las rotaciones son "clockwise"... Las reales son "counter-clockwise"...
	float a=transformation.angle;

	polig.rotate(a);

	//Sacar las medidas para la nueva caja...
	std::vector<double> xs={polig.get_vertex(0).x, polig.get_vertex(1).x, polig.get_vertex(2).x, polig.get_vertex(3).x};
	std::vector<double> ys={polig.get_vertex(0).y, polig.get_vertex(1).y, polig.get_vertex(2).y, polig.get_vertex(3).y};

	rect res{0,0,0,0};
	res.origin.x=*std::min_element(std::begin(xs), std::end(xs));
	res.origin.y=*std::min_element(std::begin(ys), std::end(ys));
	res.w=*std::max_element(std::begin(xs), std::end(xs))-res.origin.x;
	res.h=*std::max_element(std::begin(ys), std::end(ys))-res.origin.y;

	return res;
}

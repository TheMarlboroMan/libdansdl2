#include <ldv/representation.h>

#include <iostream>
#include <algorithm>

using namespace ldv;

//!Class constructor.

representation::representation(int valpha):
	visible(true),
	blend_mode(blends::none),
	val_alpha{colorfi(valpha)},
	transformed_view_position{0,0,0,0},
	base_view_position{0,0,0,0}
{

}

representation::representation(int valpha, const rect& _bvp):
	visible(true),
	blend_mode(blends::none),
	val_alpha{colorfi(valpha)},
	transformed_view_position{0,0,0,0},
	base_view_position{_bvp}
{

}
//!Draws to screen with camera. The third parameter skips camera checks and draws anyway.

//!This function actually delegates to do_draw on each derived class.
//!skip_take is mostly useful for group repesentations, specially when rotations
//are applied to them.

void representation::draw(screen& pscreen, const camera& pcamera, bool skip_take)
{
	//Using the draw info allows us to work with cartesian coordinates.
	const auto& cf=pcamera.get_draw_info();

	const auto& vp=get_view_position();

	//We need to force the template typenames, since the parameters don't match.
	if(visible && (skip_take || rects_overlap<int, int>(cf.rel_x, cf.rel_y, cf.view_w, cf.view_h, vp.origin.x, vp.origin.y, vp.w, vp.h, true
		)))
	{
		pscreen.set_camera(pcamera);
		pre_render_transform(cf);
		do_draw();
#ifndef NDEBUG
		pscreen.draw_count++;
#endif
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
		//TODO: There are functions for this, the screen already has a rect...
		const auto& vp=get_view_position();

		const int ex=vp.origin.x+vp.w,
			ey=vp.origin.y+vp.h;

		return 	ex >= 0
			&& vp.origin.x <= (int) screen.get_w()
			&& ey >= 0
			&& vp.origin.y <= (int) screen.get_h();
	};

	if(visible && (skip_take || in_screen(pscreen)))
	{
		if(pscreen.has_camera())
		{
			pscreen.reset_clip();
		}

		pre_render_transform(pscreen.get_draw_info());
		do_draw();
#ifndef NDEBUG
		pscreen.draw_count++;
#endif
	}

	//Es importante que esto siempre esté presente...
	glLoadIdentity();
}

#ifndef NDEBUG
//!Directly uses openGL to trace the view position.

//!This function should not be used beyond debug purposes.

void representation::debug_trace_box() 
{
	calculate_transformed_view_position();
	const auto& rect=transformed_view_position;
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

#endif

//!Applies all SDL transformations of zoom, position and rotation.

void representation::pre_render_transform(const draw_info& iv)
{
	glMatrixMode(GL_MODELVIEW);

	//Here we are always dealing with screen values...

	const auto& pos=get_position();
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
	calculate_transformed_view_position();
}

//!Sets the rotation center

//!The rotation center is the axis around which a representation rotates. By
//!default it is its top-left corner.

void representation::set_rotation_center(float x, float y) {
	transformation.center={x, y};
	calculate_transformed_view_position();
}

//!Calculates the view position when rotation transformations are involved.

void representation::calculate_transformed_view_position() {

	const auto& p=base_view_position;
	const auto& pos=get_position();

	polygon_2d<double> polig(
		{
			{(double)p.origin.x, (double)p.origin.y},
			{(double)(p.origin.x+p.w), (double)p.origin.y},
			{(double)(p.origin.x+p.w), (double)(p.origin.y+p.h)},
			{(double)p.origin.x, (double)(p.origin.y+p.h)},
		}, {(double)transformation.center.x+pos.x, (double)transformation.center.y+pos.y});
		//Las rotaciones son "clockwise"... Las reales son "counter-clockwise"...

	polig.rotate(transformation.angle);

	//Sacar las medidas para la nueva caja...
	const std::vector<double> xs={polig.get_vertex(0).x, polig.get_vertex(1).x, polig.get_vertex(2).x, polig.get_vertex(3).x};
	const std::vector<double> ys={polig.get_vertex(0).y, polig.get_vertex(1).y, polig.get_vertex(2).y, polig.get_vertex(3).y};

	transformed_view_position={
		{
			*std::min_element(std::begin(xs), std::end(xs)),
			*std::min_element(std::begin(ys), std::end(ys))
		},
		*std::max_element(std::begin(xs), std::end(xs))-transformed_view_position.origin.x,
		*std::max_element(std::begin(ys), std::end(ys))-transformed_view_position.origin.y
	};
}

//!Aligns this representation with respect to the parameter

void representation::align(const representation& o, const representation_alignment& ra) {

	align(o.base_view_position, ra);
}

//Aligns this representation relative to the rectangle.

//!In the inner left/top and outer right/bottom position, the margin becomes
//!inverted. This function only takes into account screen coordinates (as
//!it aligns representations, not logic objects).

void representation::align(const rect& r, const representation_alignment& ra) {

	auto inverted_alignment=ra;
	switch(ra.vertical) {
		case representation_alignment::v::none:
		case representation_alignment::v::center:
		break;
		case representation_alignment::v::outer_top:
			inverted_alignment.vertical=representation_alignment::v::outer_bottom;
		break;
		case representation_alignment::v::inner_top:
			inverted_alignment.vertical=representation_alignment::v::inner_bottom;
		break;
		case representation_alignment::v::inner_bottom:
			inverted_alignment.vertical=representation_alignment::v::inner_top;
		break;
		case representation_alignment::v::outer_bottom:
			inverted_alignment.vertical=representation_alignment::v::outer_top;
		break;
	}

	ldt::align(base_view_position, r, inverted_alignment);
	go_to(base_view_position.origin);
}

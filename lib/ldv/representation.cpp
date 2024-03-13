#include <ldv/representation.h>

#include <iostream>
#include <algorithm>

#ifdef LIBDANSDL2_DEBUG
#include <lm/log.h>
#include <ldt/log.h>
#endif

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

bool representation::draw(
	screen& pscreen, 
	const camera& pcamera, 
	bool skip_take
) {

	//Using the draw info allows us to work with cartesian coordinates.
	const auto& cf=pcamera.get_draw_info();
	const auto& vp=get_view_position();

	const bool in_take=rects_overlap<int, int>(
		cf.rel_x, cf.rel_y, cf.view_w, cf.view_h, 
		vp.origin.x, vp.origin.y, vp.w, vp.h, 
		true
	);

	//We need to force the template typenames, since the parameters don't match.
	if(
		visible && ( skip_take || in_take)
	) {

		//TODO: i am sure this fucks up some cycles needlessly.
		pscreen.set_camera(pcamera);
		pre_render_transform(cf);
		do_draw();
#ifdef LIBDANSDL2_DEBUG
		pscreen.debug_draw_count++;
#endif

		glLoadIdentity();
		return true;
	}

#ifdef LIBDANSDL2_DEBUG
	std::stringstream ss;
	ss<<"DID NOT DRAW!"<<std::endl;
	ss<<"visible: "<<(visible ? "true" : "false")<<std::endl;
	ss<<"skip_take: "<<(skip_take ? "true" : "false")<<std::endl;
	ss<<"in_take: "<<(in_take ? "true" : "false")<<std::endl;
	ss<<"view position:"<<vp<<std::endl;
	ss<<"draw info:"<<cf<<std::endl;

	lm::log(ldt::log_lsdl::get()).debug()<<ss.str();
#endif

	//Do not forget this.
	glLoadIdentity();
	return false;
}

void representation::debug_against_camera(
#ifdef LIBDANSDL2_DEBUG
	const camera& _camera
#else
	const camera&
#endif
) {
#ifdef LIBDANSDL2_DEBUG

	calculate_transformed_view_position();
	const auto& cf=_camera.get_draw_info();
	const auto& vp=get_view_position();

	bool overlap=rects_overlap<int, int>(
		cf.rel_x, cf.rel_y, cf.view_w, cf.view_h, 
		vp.origin.x, vp.origin.y, vp.w, vp.h, 
		true
	);

	std::stringstream ss;
	ss<<"overlap: "<<(overlap ? "true" : "false")<<std::endl;
	ss<<"view position: "<<vp<<std::endl;
	ss<<"draw_info: "<<cf<<std::endl;

	lm::log(ldt::log_lsdl::get()).debug()<<ss.str();

#endif
}

//!Draws to screen with no camera bounds.

//!This function actually delegates to do_draw on each derived class.
//!The second parameter skips window bound tests and tries to draw anyway.

bool representation::draw(screen& pscreen, bool skip_take) {

	auto in_screen=[this](screen &screen) {

		//TODO: There are functions for this, the screen already has a rect...
		const auto& vp=get_view_position();

		const int ex=vp.origin.x+vp.w,
			ey=vp.origin.y+vp.h;

		return 	ex >= 0
			&& vp.origin.x <= (int) screen.get_w()
			&& ey >= 0
			&& vp.origin.y <= (int) screen.get_h();
	};

	if(visible && (skip_take || in_screen(pscreen))) {

		if(pscreen.has_camera()) {

			pscreen.reset_clip();
		}

		pre_render_transform(pscreen.get_draw_info());
		do_draw();
#ifndef LIBDANSDL2_DEBUG
		pscreen.debug_draw_count++;
#endif

		glLoadIdentity();
		return true;
	}

	//Es importante que esto siempre esté presente...
	glLoadIdentity();
	return false;
}

//!Directly uses openGL to trace the view position.

//!This function should not be used beyond debug purposes. Colors go from 0
//to one. Uses ABSOLUTE positioning, no cameras.

void representation::debug_trace_box(
#ifdef LIBDANSDL2_DEBUG
	float _r,
	float _g,
	float _b,
	float _a
#else
	float, float, float, float
#endif
) {
#ifdef LIBDANSDL2_DEBUG
	calculate_transformed_view_position();
	const auto& rect=transformed_view_position;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(_r, _g, _b, _a);
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
#endif
}

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

void representation::center_rotation_center() {

	transformation.center={base_view_position.w / 2.f, base_view_position.h / 2.f};
	calculate_transformed_view_position();
}

//!Calculates the view position when rotation transformations are involved.

void representation::calculate_transformed_view_position() {

	const auto& p=base_view_position;
	const auto& pos=get_position();


	double  origin_x=p.origin.x,
	        origin_y=p.origin.y,
	        end_x=origin_x+p.w,
	        end_y=origin_y+p.h,
	        rotation_x=transformation.center.x+pos.x,
	        rotation_y=transformation.center.y+pos.y;

	polygon_2d<double> polig{
		{
			{origin_x, origin_y},
			{end_x, origin_y},
			{end_x, end_y},
			{origin_x, end_y}
		},
		{rotation_x, rotation_y}
	};

	//Las rotaciones son "clockwise"... Las reales son "counter-clockwise"...
	polig.rotate(transformation.angle);

	//Sacar las medidas para la nueva caja...
	const std::vector<double> xs={polig.get_vertex(0).x, polig.get_vertex(1).x, polig.get_vertex(2).x, polig.get_vertex(3).x};
	const std::vector<double> ys={polig.get_vertex(0).y, polig.get_vertex(1).y, polig.get_vertex(2).y, polig.get_vertex(3).y};

	double transformed_origin_x=floor(*std::min_element(std::begin(xs), std::end(xs)));
	double transformed_origin_y=floor(*std::min_element(std::begin(ys), std::end(ys)));

	double max_x=floor(*std::max_element(std::begin(xs), std::end(xs)));
	double max_y=floor(*std::max_element(std::begin(ys), std::end(ys)));

	double w=max_x-transformed_origin_x;
	double h=max_y-transformed_origin_y;

	int final_x=floor(transformed_origin_x),
	    final_y=floor(transformed_origin_y);
	unsigned final_w=floor(w),
	        final_h=floor(h);

	transformed_view_position={
		{final_x, final_y},
		final_w, final_h
	};
}

//!Aligns this representation with respect to the parameter

void representation::align(
	const representation& o,
	const representation_alignment& ra
) {

	align(o.get_align_rect(), ra);
}

//Aligns this representation relative to the rectangle.

//!In the inner left/top and outer right/bottom position, the margin becomes
//!inverted. This function only takes into account screen coordinates (as
//!it aligns representations, not logic objects).

void representation::align(
	const rect& r,
	const representation_alignment& ra) {

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

	ldt::align(get_align_rect(), r, inverted_alignment);
	go_to(get_align_rect().origin);
}

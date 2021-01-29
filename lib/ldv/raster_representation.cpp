#include <ldv/raster_representation.h>
#include <ldt/polygon_2d.h>

#include <algorithm>
using namespace ldv;

//!Constructs a raster_representation with position, clipping and alpha.

raster_representation::raster_representation(
	rect pos,
	rect rec,
	int palpha
):
	representation(palpha),
	texture_instance(nullptr),
	brush{0,0},
	points{{0,0},{0,0},{0,0},{0,0}},
	tex_points{{0.,0.},{0.,0.},{0.,0.},{0.,0.}},
	rgb_colorize{1.f, 1.f, 1.f},
	location(pos),
	clip(rec)
{

}
//!Sets the clip to the full size of the texture.

//!Correspondingly adjusts the location box to the clipping size.
//!Will throw an std::exception if there is no texture.

void raster_representation::clip_to_texture()
{
	set_clip({0,0, get_w_texture_instance(), get_h_texture_instance()});
	location.w=clip.w;
	location.h=clip.h;
}

//!Does the drawing work.

//!Directly invokes openGL routines.
//!A first invocation will take longer, since a series of points need to be
//!calculated for the vertexes and texture.

void raster_representation::do_draw()
{
	glBindTexture(GL_TEXTURE_2D, texture_instance->get_index());
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Alpha...
	switch(get_blend()) {
		case representation::blends::none:
			glDisable(GL_BLEND);
			glColor3f(rgb_colorize.r, rgb_colorize.g, rgb_colorize.b);
		break;
		case representation::blends::alpha:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(rgb_colorize.r, rgb_colorize.g, rgb_colorize.b, get_alphaf());
		break;
	}

	if(calculate) {
		with_brush
			? calculate_points_brush()
			: calculate_points();
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	glVertexPointer(2, GL_INT, 0, points.data());

	//glTexCoordPointer(2, GL_FLOAT, 0, tex_points.data());

	glTexCoordPointer(2, GL_DOUBLE, 0, tex_points.data());
	glDrawArrays(GL_QUADS, 0, points.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glDisable(GL_TEXTURE_2D);
}

//!Calculates and stores vertex and texture points.

void raster_representation::calculate_points() {

	const rect& pos=get_location();

	points[0]={0, 0},
	points[1]={(int)pos.w, 0},
	points[2]={(int)pos.w, (int)pos.h},
	points[3]={0, (int)pos.h};

	const rect& recor=get_clip();
	const double    w_tex=texture_instance->get_w(),
	                h_tex=texture_instance->get_h();

	point_type ptex_x=(point_type)recor.origin.x,
	        ptex_y=(point_type)recor.origin.y,
	        ptex_fx=ptex_x+recor.w,
	        ptex_fy=ptex_y+recor.h;

	tex_points[0]={ptex_x,	ptex_y},
	tex_points[1]={ptex_fx,	ptex_y},
	tex_points[2]={ptex_fx,	ptex_fy},
	tex_points[3]={ptex_x,	ptex_fy};

	if(transformation.horizontal) {

		std::swap(tex_points[0].x, tex_points[1].x);
		std::swap(tex_points[2].x, tex_points[3].x);
	}

	if(transformation.vertical) {

		std::swap(tex_points[0].y, tex_points[2].y);
		std::swap(tex_points[1].y, tex_points[3].y);
	}

	//Convert again to 0:1 ratio.
	for(auto &p : tex_points) {

		p.x/=w_tex;
		p.y/=h_tex;
	}

	calculate=false;
}

void raster_representation::calculate_points_brush() {

	const rect& pos=get_location();
	const rect& recor=get_clip();

	if(!brush.w || !brush.h) {

		throw std::runtime_error("brush must be set to positive values");
	}

	//TODO: We should be able to precalculate these and resize them.

	points.clear();
	tex_points.clear();

	const double    w_tex=texture_instance->get_w(),
	                h_tex=texture_instance->get_h();

	int itx=0;

	for(int x=0; x < (int)pos.w; x+=brush.w) {

		int ity=0;
		const int dif_x=x+brush.w > (int)pos.w ? pos.w - (itx * brush.w)  : brush.w;

		for(int y=0; y < (int)pos.h; y+=brush.h) {

			const int dif_y=y+brush.h > (int)pos.h ? pos.w - (ity * brush.h) : brush.h;

			point pts[]={
				{x, y},
				{x+dif_x, y},
				{x+dif_x, y+dif_y},
				{x, y+dif_y}
			};
			points.insert(std::end(points), pts, pts+4);

			//ptex_fx y ptex_fy calculations are a proportion between the brush
			//and the space to be drawn (rule of three). This will only map
			//the neccesary texture parts.

			point_type ptex_x=(point_type)recor.origin.x,
			        ptex_y=(point_type)recor.origin.y,
			        ptex_fx=ptex_x+( ( (point_type)dif_x * (point_type)recor.w) / (point_type)brush.w),
			        ptex_fy=ptex_y+( ( (point_type)dif_y * (point_type)recor.h) / (point_type)brush.h);

			texpoint ptex[]={
				{ptex_x,	ptex_y},
				{ptex_fx,	ptex_y},
				{ptex_fx,	ptex_fy},
				{ptex_x,	ptex_fy}};

			if(transformation.horizontal) {

				std::swap(ptex[0].x, ptex[1].x);
				std::swap(ptex[2].x, ptex[3].x);
			}

			if(transformation.vertical) {

				std::swap(ptex[0].y, ptex[2].y);
				std::swap(ptex[1].y, ptex[3].y);
			}

			//Convert again to 0:1 ratio.
			for(auto &p : ptex) {

				p.x/=w_tex;
				p.y/=h_tex;
			}

			tex_points.insert(std::end(tex_points), ptex, ptex+4);
			++ity;
		}
		++itx;
	}

	calculate=false;
}

//!Deletes the texture assigned.

//!This should only be called if the representation owns the texture. I cannot
//!think of a single example when this is useful.

void raster_representation::free_texture() {

	if(texture_instance) {
		delete texture_instance;
		texture_instance=nullptr;
	}
}

void raster_representation::set_brush(
	int _w,
	int _h
) {
	brush={_w, _h};
	with_brush=true;
	reset_calculations();
}

void raster_representation::reset_brush() {
	brush={0, 0};
	with_brush=false;
	points.resize(4);
	tex_points.resize(4);
	reset_calculations();
}

//!Sets the position and size of the representation.

//!go_to is used to set the position without altering the size.

void raster_representation::set_location(rect c) {

	location=c;
	update_view_position();
}

//!Sets the texture clip.

void raster_representation::set_clip(rect p_caja) {

	clip=p_caja;
	reset_calculations();
}

//!Sets the position without altering size.

void raster_representation::go_to(point p) {

	location.origin=p;
	update_view_position();
}

//!Gets assigned texture width.

//!Shortcut to get_texture()->get_w(). Will throw if there is no texture
//!assigned.

unsigned int raster_representation::get_w_texture_instance() const {

	if(!texture_instance) {
		throw std::runtime_error("no texture for get_w_texture_instance");
	}

	return texture_instance->get_w();
}

//!Gets assigned texture height.

//!Shortcut to get_texture()->get_h(). Will throw if there is no texture
//!assigned.

unsigned int raster_representation::get_h_texture_instance() const {

	if(!texture_instance) {
		throw std::runtime_error("no texture for get_h_texture_instance");
	}

	return texture_instance->get_h();
}

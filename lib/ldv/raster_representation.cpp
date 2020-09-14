#include <ldv/raster_representation.h>
#include <ldt/polygon_2d.h>

#include <algorithm>
using namespace ldv;

//!Constructs a raster_representation with position, clipping and alpha.

raster_representation::raster_representation(rect pos, rect rec, int palpha)
	:representation(palpha), texture_instance(nullptr),
	brush{0,0}, rgb_colorize{1.f, 1.f, 1.f},
	location(pos), clip(rec)
{

}

//!Class copy constructor.

//!Texture is assigned. It must not be owned by the class, but by an external manager.

raster_representation::raster_representation(const raster_representation& o)
	:representation(o), texture_instance(o.texture_instance),
	brush(o.brush), points(o.points),
	tex_points(o.tex_points),
	rgb_colorize(o.rgb_colorize),
	location(o.location),
	clip(o.clip)
{

}

//!Assignment operator.

//!Texture is assigned. It must not be owned by the class, but by an external manager.

raster_representation& raster_representation::operator=(const raster_representation& o)
{
	representation::operator=(o);
	texture_instance=o.texture_instance;
	location=o.location;
	clip=o.clip;
	brush=o.brush;
	points=o.points;
	tex_points=o.tex_points;

	return *this;
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

	if(!points.size() || tex_points.size()) {
		calculate_points();
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
	const rect& recor=get_clip();

	if(!brush.w) {
		brush.w=pos.w;
	}

	if(!brush.h) {
		brush.h=pos.h;
	}

	const double    w_tex=texture_instance->get_w(),
	                h_tex=texture_instance->get_h();

	points.clear();
	tex_points.clear();

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

/*
			struct extrapt {
				double x=0.f, y=0.f;
			};

			std::vector<extrapt> extra;
			extra.insert(std::begin(extra), 8, extrapt{0.f, 0.f});
			std::ifstream testfile{"/mnt/dev/other/sdl2-opengl-pivot/thing.txt"};
			std::string str;
			for(int line=0; line<8; ++line) {

				std::getline(testfile, str);
				std::stringstream ss(str);
				ss>>extra[line].x>>extra[line].y;
				std::cout<<line<<" -> "<<extra[line].x<<","<<extra[line].y<<std::endl;
			}
*/

			texpoint ptex[]={
				{ptex_x,	ptex_y},
				{ptex_fx,	ptex_y},
				{ptex_fx,	ptex_fy-0.005},
				{ptex_x,	ptex_fy-0.005}};


std::cout<<"---------------------------"<<std::endl;
			//Convert again to 0:1 ratio.
			for(auto &p : ptex) {

				p.x/=w_tex;
				p.y/=h_tex;

std::cout<<p.x<<", "<<p.y<<std::endl;
			}

			if(!transformation.horizontal && !transformation.vertical) {

//					ptex[0].x+=0.1f;
//					ptex[1].x+=0.1f;
//					ptex[2].x+=0.1f;
//					ptex[3].x+=0.1f;

//					ptex[0].y-=0.1f;
//					ptex[1].y-=0.1f;
//					ptex[2].y-=0.1f;
//					ptex[3].y-=0.1f;
			}
			else {

				//Inversion means resampling too.
				if(transformation.horizontal) {

					std::swap(ptex[0].x, ptex[1].x);
					std::swap(ptex[2].x, ptex[3].x);

//					ptex[2].x-=1.f;
//					ptex[3].x-=1.f;

//					ptex[2].y-=0.5f;
//					ptex[3].y-=0.5f;
				}

				if(transformation.vertical) {

					std::swap(ptex[0].y, ptex[2].y);
					std::swap(ptex[1].y, ptex[3].y);
				}
			}

			tex_points.insert(std::end(tex_points), ptex, ptex+4);
			++ity;
		}
		++itx;
	}

std::cout<<"END"<<std::endl;
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

//!Resets all stored fields.

//!Internally called when the clipping box changes.

void raster_representation::reset_calculations() {

	brush={0,0};
	points.clear();
	tex_points.clear();
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

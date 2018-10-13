#include "raster_representation.h"
#include <algorithm>
#include "../../../tools/polygon_2d/polygon_2d.h"

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

	//Alpha...
	switch(get_blend())
	{
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
		
	if(!points.size() || tex_points.size())
	{
		calculate_points();
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	glVertexPointer(2, GL_INT, 0, points.data());
	glTexCoordPointer(2, GL_FLOAT, 0, tex_points.data());
	glDrawArrays(GL_QUADS, 0, points.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glDisable(GL_TEXTURE_2D);
}

//!Calculates and stores vertex and texture points.

void raster_representation::calculate_points()
{
	const rect& pos=get_location();
	const rect& recor=get_clip();
	
	if(!brush.w) brush.w=pos.w;
	if(!brush.h) brush.h=pos.h;

	float w_tex=texture_instance->get_w(), h_tex=texture_instance->get_h();

	points.clear();
	tex_points.clear();

	int itx=0;

	for(int x=0; x < (int)pos.w; x+=brush.w)
	{
		int ity=0;
		const int dif_x=x+brush.w > (int)pos.w ? pos.w - (itx * brush.w)  : brush.w;

		for(int y=0; y < (int)pos.h; y+=brush.h)
		{
			const int dif_y=y+brush.h > (int)pos.h ? pos.w - (ity * brush.h) : brush.h;

			point pts[]={{x, y}, {x+dif_x, y}, {x+dif_x, y+dif_y}, {x, y+dif_y}};

			//ptex_fx y ptex_fy calculations are a proportion between the brush
			//and the space to be drawn (rule of three). This will only map
			//the neccesary texture parts.
 
			GLfloat ptex_x=(GLfloat)recor.origin.x,
				ptex_y=(GLfloat)recor.origin.y, 
				ptex_fx=ptex_x+( ( (GLfloat)dif_x * (GLfloat)recor.w) / (GLfloat)brush.w), 
				ptex_fy=ptex_y+( ( (GLfloat)dif_y * (GLfloat)recor.h) / (GLfloat)brush.h);

			texpoint ptex[]={
				{ptex_x,	ptex_y},
				{ptex_fx,	ptex_y},
				{ptex_fx,	ptex_fy},
				{ptex_x,	ptex_fy}};

			//Inversion means resampling too.
			if(transformation.horizontal)
			{
				std::swap(ptex[0].x, ptex[1].x);
				std::swap(ptex[2].x, ptex[3].x);
			}

			if(transformation.vertical)
			{
				std::swap(ptex[0].y, ptex[2].y);
				std::swap(ptex[1].y, ptex[3].y);
			}

			for(auto &p : ptex)
			{
				p.x/=w_tex; 
				p.y/=h_tex;
			}

			tex_points.insert(std::end(tex_points), ptex, ptex+4);
			points.insert(std::end(points), pts, pts+4);
			++ity;
		}
		++itx;
	}
}

//!Deletes the texture assigned.

//!This should only be called if the representation owns the texture. I cannot
//!think of a single example when this is useful.

void raster_representation::free_texture()
{
	if(texture_instance)
	{
		delete texture_instance;
		texture_instance=nullptr;
	}
}

//!Sets the position and size of the representation.

//!go_to is used to set the position without altering the size.

void raster_representation::set_location(rect c)
{
	location=c;
	update_view_position();
}

//!Sets the texture clip.

void raster_representation::set_clip(rect p_caja)
{
	clip=p_caja;
	reset_calculations();
}

//!Sets the position without altering size.

void raster_representation::go_to(point p)
{
	location.origin=p;
	update_view_position();
}

//!Gets the position.

point raster_representation::get_position() const
{
	return location.origin;
}

//!Gets the base view position.

rect raster_representation::get_base_view_position() const
{
	return location;
}

//!Resets all stored fields.

//!Internally called when the clipping box changes.

void raster_representation::reset_calculations()
{
	brush={0,0};
	points.clear();
	tex_points.clear();
}

//!Gets assigned texture width.

//!Shortcut to get_texture()->get_w(). Will throw if there is no texture
//!assigned.

unsigned int raster_representation::get_w_texture_instance() const 
{
	if(!texture_instance) throw std::runtime_error("no texture for get_w_texture_instance");
	return texture_instance->get_w();
}

//!Gets assigned texture height.

//!Shortcut to get_texture()->get_h(). Will throw if there is no texture
//!assigned.

unsigned int raster_representation::get_h_texture_instance() const 
{
	if(!texture_instance) throw std::runtime_error("no texture for get_h_texture_instance");
	return texture_instance->get_h();
}

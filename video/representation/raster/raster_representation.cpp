#include "raster_representation.h"
#include <algorithm>
#include "../../../tools/polygon_2d/polygon_2d.h"

using namespace ldv;

raster_representation::raster_representation(rect pos, rect rec, sampling ts)
	:representation(), texture_instance(nullptr), 
	brush{0,0}, sampling_type(ts), location(pos), clip(rec)
{

}

raster_representation::raster_representation(rgba_color color, rect pos, rect rec, sampling ts)
	:representation(color), texture_instance(nullptr),
	brush{0,0}, sampling_type(ts), location(pos), clip(rec)
{

}

raster_representation::raster_representation(const raster_representation& o)
	:representation(o), texture_instance(o.texture_instance),
	brush(o.brush), points(o.points), 
	tex_points(o.tex_points),
	sampling_type(o.sampling_type),
	location(o.location), 
	clip(o.clip)
{

}

raster_representation& raster_representation::operator=(const raster_representation& o)
{
	representation::operator=(o);
	texture_instance=o.texture_instance;
	location=o.location;
	clip=o.clip;
	brush=o.brush;
	points=o.points;
	tex_points=o.tex_points;
	sampling_type=o.sampling_type;

	return *this;
}

void raster_representation::clip_to_texture()
{
	set_clip(0,0, texture_instance->get_w(), texture_instance->get_h());
	location.w=clip.w;
	location.h=clip.h;
}

void raster_representation::do_draw()
{
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glBindTexture(GL_TEXTURE_2D, texture_instance->get_index());
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Alpha...
	const auto c=get_rgba();
	switch(get_blend())
	{
		case representation::blends::none:
			glDisable(GL_BLEND);
			glColor3f(c.r, c.g, c.b);
		break;
		case representation::blends::alpha:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(c.r, c.g, c.b, c.a);
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

//Calcula las positiones de los vértices y los points de las texture_instances.
//Dado que las positiones se calculan a partir de 0.0 no debería haber problemas
//al cambiar la posición.
//Los valores calculados se almacenan.

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

			//Samplear los points centrales de las cuatro esquinas. De momento no buscamos el centro del texel. 
			//El cálculo de ptex_fx y ptex_fy está sacando la proporción entre el brush y la cantidad de espacio
			//que queda por dibujar (una simple regla de tres). La finalidad es mapear sólo el trozo de texture_instance
			//necesario.
 
			GLfloat ptex_x=(GLfloat)recor.origin.x,
				ptex_y=(GLfloat)recor.origin.y, 
				ptex_fx=ptex_x+( ( (GLfloat)dif_x * (GLfloat)recor.w) / (GLfloat)brush.w), 
				ptex_fy=ptex_y+( ( (GLfloat)dif_y * (GLfloat)recor.h) / (GLfloat)brush.h);

			puntotex ptex[]={
				{ptex_x,	ptex_y},
				{ptex_fx,	ptex_y},
				{ptex_fx,	ptex_fy},
				{ptex_x,	ptex_fy}};

			//Invertir y resamplear points...
			if(transformation.horizontal)
			{
				std::swap(ptex[0].x, ptex[1].x);
				std::swap(ptex[2].x, ptex[3].x);

				if(sampling_type==sampling::atlas) for(auto &p: ptex) p.x-=0.5f;
			}
			else 
			{
				if(sampling_type==sampling::atlas)
				{
					for(auto &p: ptex) p.x+=0.5f;
				}
			}

			if(transformation.vertical)
			{
				std::swap(ptex[0].y, ptex[2].y);
				std::swap(ptex[1].y, ptex[3].y);

				if(sampling_type==sampling::atlas)
				{ 
					for(auto &p: ptex) p.y-=0.5f;
				}
			}
			else
			{
				if(sampling_type==sampling::atlas) 
				{
					for(auto &p: ptex) p.y+=0.5f;
				}
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

//Eso sólo deberíamos llamarlo en aquellas para las cuales hemos creado una
//texture_instance que poseen. Si es parte de un recurso que no es de su propiedad
//te vas a encontrar con un problema.

void raster_representation::free_texture()
{
	if(texture_instance)
	{
		delete texture_instance;
		texture_instance=nullptr;
	}
}

void raster_representation::set_location(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	if(p_flags & frx) location.origin.x=p_x;
	if(p_flags & fry) location.origin.y=p_y;
	if(p_flags & frw && p_w != -1) location.w=p_w;
	if(p_flags & frh && p_h != -1) location.h=p_h;
	update_view_position();
}

void raster_representation::set_location(rect c)
{
	location=c;
	update_view_position();
}

void raster_representation::set_clip(rect p_caja)
{
	clip=p_caja;
	reset_calculations();
}

void raster_representation::set_clip(Sint16 p_x, Sint16 p_y, Uint16 p_w, Uint16 p_h, int p_flags)
{
	if(p_flags & frx) clip.origin.x=p_x;
	if(p_flags & fry) clip.origin.y=p_y;
	if(p_flags & frw) clip.w=p_w;
	if(p_flags & frh) clip.h=p_h;
	reset_calculations();
}

void raster_representation::go_to(int x, int y)
{
	location.origin.x=x; 
	location.origin.y=y;
	update_view_position();
}

point raster_representation::get_position() const
{
	return point{location.origin.x, location.origin.y};
}

rect raster_representation::get_base_view_position() const
{
	return location;
}

void raster_representation::reset_calculations()
{
	brush={0,0};
	points.clear();
	tex_points.clear();
}

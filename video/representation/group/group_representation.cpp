#include "group_representation.h"

using namespace ldv;

group_representation::group_representation(point p, bool p_poseer)
	:representation(), position{p}, owns_data(p_poseer)
{
	update_view_position();
}

group_representation::group_representation(const group_representation& o)
	:representation(o), owns_data(o.owns_data)
{
	if(!owns_data)
	{
		data=o.data;
	}
}

group_representation::~group_representation()
{
	clear();
}

//!Empties the group.

//!All representations will be destroyed.

void group_representation::clear()
{
	if(owns_data)
	{
		std::vector<representation *>::iterator ini=data.begin(),
						fin=data.end();
		while(ini < fin)
		{
			delete (*ini);
			ini++;
		};
	}

	data.clear();
}

//!Draws the group to the screen using a camera. 

//!Parameters are the same as in a regular representation object.

void group_representation::draw(screen& p_screen, const camera& pcamera, bool skip_take)
{
	if(is_visible() && (skip_take || is_in_focus(pcamera.get_focus_box())))
	{
		draw_internal(p_screen, &pcamera);
	}
}

//!Draws the group to the screen with no camera.

//!Parameters are the same as in a regular representation object.

void group_representation::draw(screen& p_screen, bool skip_take)
{
	auto in_screen=[this](screen &screen)
	{
		const auto& v_position=get_view_position();

		int ex=v_position.origin.x+v_position.w,
			ey=v_position.origin.y+v_position.h;

		return 	ex >= 0
			&& v_position.origin.x <= (int) screen.get_w()
			&& ey >= 0
			&& v_position.origin.y <= (int) screen.get_h();
	};

	if(is_visible() && (skip_take || in_screen(p_screen)))
	{
		draw_internal(p_screen, nullptr);
	}
}

//!Internal draw function.

//!The position of each representation will be added to the group position, so
//!top-left of the group is 0.0. Group alpha is applied to each item whenever 
//!it is not opaque. In other words, it is not accumulative nor it does blend.

void group_representation::draw_internal(screen& p_screen, camera const * pcamera)
{
	//When assigning, matrixes are reset. If assignation occurs during
	//this iteration we can have terrible results because translations not
	//accounted for.
	if(pcamera!=nullptr) p_screen.set_camera(*pcamera);

	for(auto &r : data)
	{
		glMatrixMode(GL_MODELVIEW);

		//Traslación propia del grupo, rotaciones...
		auto tr=get_transformation_rotation();
		auto iv=pcamera != nullptr ? pcamera->get_draw_info() : p_screen.get_draw_info();

		glTranslatef(position.x*iv.zoom, position.y*iv.zoom, 0.f);

		if(pcamera!=nullptr)
		{
			//This is pure empiric knowledge. Try and error.
			if(tr.angle != 0.f)
			{
				float 	tx=iv.pos_x+((tr.center.x-iv.rel_x)*iv.zoom),
					ty=iv.pos_y+((tr.center.y-iv.rel_y)*iv.zoom);

				glTranslatef(tx, ty, 0.f);
				glRotatef(tr.angle, 0.f, 0.f, 1.f);
				glTranslatef(-tx, -ty, 0.f);
			}
		}
		else
		{
			if(tr.angle != 0.f)
			{
				glTranslatef(tr.center.x, tr.center.y, 0.f);
				glRotatef(tr.angle, 0.f, 0.f, 1.f);
				glTranslatef(-tr.center.x, -tr.center.y, 0.f);
			}
		}

		int alpha_original=r->get_alpha();
		auto blend_original=r->get_blend();

		int calculado=get_alpha() ? (alpha_original * get_alpha()) / alpha_max : alpha_min;
		r->set_blend(representation::blends::alpha);
		r->set_alpha(calculado);

		//Camera and screen checks are skipped.
		if(pcamera!=nullptr) r->draw(p_screen, *pcamera, true);
		else r->draw(p_screen, true);

		r->set_blend(blend_original);
		r->set_alpha(alpha_original);
	}
}

//!This function is unreachable.

void group_representation::do_draw()
{

}

//!Inserts a representation in the group.

//!The order of insertion determines the subsequent drawing order, making
//!groups a very rigid structure.

void group_representation::insert(representation * p_rep)
{
	data.push_back(p_rep);
	update_view_position();
}

//!Updates the group position.

void  group_representation::go_to(point p)
{
	position={p.x, p.y};
	update_view_position();
}

//!Gets the group position as a point. 

//!Each part of the group may go into the group negative space.

point group_representation::get_position() const
{
	return position;
}

//!Returns the box of all representations put together.

rect group_representation::get_base_view_position() const
{
	if(!data.size())
	{
		return rect{position.x,position.y,0,0};
	}
	else
	{
		rect res=data[0]->get_base_view_position();

		int 	fx=res.origin.x+res.w,
			fy=res.origin.y+res.h;

		for(const auto& r : data)
		{
			rect pr=r->get_base_view_position();

			if(pr.origin.x < res.origin.x) res.origin.x=pr.origin.x;
			if(pr.origin.y < res.origin.y) res.origin.y=pr.origin.y;

			int 	prfx=pr.origin.x+pr.w,
				prfy=pr.origin.y+pr.h;

			if(prfx > fx) fx=prfx;
			if(prfy > fy) fy=prfy;
		}

		res.w=fx-res.origin.x;
		res.h=fy-res.origin.y;

		res.origin.x+=position.x;
		res.origin.y+=position.y;

		return res;
	}
}

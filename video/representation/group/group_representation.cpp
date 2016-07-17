#include "group_representation.h"

using namespace ldv;

group_representation::group_representation(point p, bool p_poseer)
	:representation(), position{p}, owns_data(p_poseer)
{
	update_view_position();
}

group_representation::group_representation(const group_representation& o):
	owns_data(o.owns_data)
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

void group_representation::draw(screen& p_screen, const camera& pcamera, bool skip_take)
{
	if(is_visible() && (skip_take || is_in_focus(pcamera.get_focus_box())))
	{
		draw_internal(p_screen, &pcamera);
	}
}

void group_representation::draw(screen& p_screen, bool skip_take)
{
	if(is_visible() && is_in_focus(p_screen.get_box()))
	{
		draw_internal(p_screen, nullptr);
	}
}

/*La posición que demos a la representación que se pasa se sumará luego a 
la posición del grupo. Para poner algo en la esquina superior izquierda del 
grupo estableceríamos la posición de rep en 0,0.
El alpha del grupo se aplica directamente a cada componente siempre que tenga
un valor distinto de "opaco".
Sería posible hacerlo "acumulativo" sin muchos problemas de modo que los valores
se sumen y resten dentro del rango 0-255.
*/

void group_representation::draw_internal(screen& p_screen, camera const * pcamera)
{
	//Al asignar se reinician las matrices. Si la asignación ocurre durante 
	//la iteración podríamos tenemos malos resultados ya que antes ha habido
	//un translate que no se ha tenido en cuenta.
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
			//De nuevo, puro empirismo. No tengo ni idea de qué hace esto.
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

		//Indicamos que vamos a saltar el check de en toma.
		if(pcamera!=nullptr) r->draw(p_screen, *pcamera, true);
		else r->draw(p_screen, true);

		r->set_blend(blend_original);
		r->set_alpha(alpha_original);
	}
}

void group_representation::do_draw()
{
	//Lulz circus. Nunca llegaríamos aquí.
}

void group_representation::insert(representation * p_rep)
{
	data.push_back(p_rep);
	update_view_position();
}

void  group_representation::go_to(int px, int py)
{
	position={px, py};
	update_view_position();
}

point group_representation::get_position() const
{
	return position;
}

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
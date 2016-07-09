#include "representacion_agrupada.h"

using namespace DLibV;

Representacion_agrupada::Representacion_agrupada(int px, int py, bool p_poseer)
	:Representacion(), x(px), y(py), posee_las_representaciones(p_poseer),
	impone_alpha(true),
	impone_modo_blend(false)
{
	actualizar_posicion_vista_rotacion();
}

Representacion_agrupada::Representacion_agrupada(const Representacion_agrupada& p_otra):
	posee_las_representaciones(p_otra.posee_las_representaciones)
{
	if(!this->posee_las_representaciones)
	{
		this->grupo=p_otra.grupo;
	}
}

Representacion_agrupada::~Representacion_agrupada()
{
	vaciar_grupo();
}

void Representacion_agrupada::vaciar_grupo()
{
	if(this->posee_las_representaciones)
	{
		std::vector<Representacion *>::iterator ini=grupo.begin(),
						fin=grupo.end();
		while(ini < fin)
		{
			delete (*ini);
			ini++;
		};
	}

	grupo.clear();
}

void Representacion_agrupada::volcar(Pantalla& p_pantalla, const Camara& p_camara, int ex, int ey)
{
	if(es_visible() && es_en_toma(p_camara.acc_caja_foco(), ex, ey))
	{
		volcado_interno(p_pantalla, &p_camara, ex, ey);
	}
}

void Representacion_agrupada::volcar(Pantalla& p_pantalla, int ex, int ey)
{
	if(es_visible() && es_en_toma(p_pantalla.acc_simulacro_caja(), ex, ey)) 
	{
		volcado_interno(p_pantalla, nullptr, ey, ey);
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

void Representacion_agrupada::volcado_interno(Pantalla& p_pantalla, Camara const * p_camara, int ex, int ey)
{
	unsigned int alpha_p=acc_alpha();
	auto modo_blend_p=acc_modo_blend();
//	unsigned int mod_color_r=acc_mod_color_r();
//	unsigned int mod_color_g=acc_mod_color_g();
//	unsigned int mod_color_b=acc_mod_color_b();

	unsigned int alpha_a=0;
	auto modo_blend_a=Representacion::blends::BLEND_NADA;

	//Al asignar se reinician las matrices. Si la asignación ocurre durante 
	//la iteración podríamos tenemos malos resultados ya que antes ha habido
	//un translate que no te ha tenido en cuenta.
	if(p_camara!=nullptr) p_pantalla.asignar_camara(*p_camara);

	for(auto &r : grupo)
	{
		if(impone_alpha)
		{
			alpha_a=r->acc_alpha();
			r->establecer_alpha(alpha_p);
		}

		if(impone_modo_blend)
		{
			modo_blend_a=r->acc_modo_blend();
			r->establecer_modo_blend(modo_blend_p);
		}

//		unsigned int mod_color_r_a=acc_mod_color_r();
//		unsigned int mod_color_g_a=acc_mod_color_g();
//		unsigned int mod_color_b_a=acc_mod_color_b();

//		r->establecer_mod_color(mod_color_r, mod_color_g, mod_color_b);

		glMatrixMode(GL_MODELVIEW);

//TODO: This is not accounting for all transformations...

		//Zoom, desplazamiento y rotación...
/*
		auto iv=p_camara != nullptr ? p_camara->acc_info_volcado() : p_pantalla.acc_info_volcado();
		glTranslatef(x*iv.zoom, y*iv.zoom, 0.f);

		//Desplazamiento cámara...
///		if(p_camara!=nullptr)
//		{
//			int 	cx=iv.pos_x+x-iv.rel_x, 
//				cy=iv.pos_y+y-iv.rel_y;
//			glTranslatef(cx, cy, 0.f);
//		}

		const auto& transformacion=acc_transformacion_rotacion();
		if(transformacion.angulo_rotacion != 0.f)
		{
//			glTranslatef(transformacion.centro_rotacion.x, transformacion.centro_rotacion.y, 0.f);
			glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
//			glTranslatef(-transformacion.centro_rotacion.x, -transformacion.centro_rotacion.y, 0.f);
		}
		*/

/*
		auto iv=p_camara != nullptr ? p_camara->acc_info_volcado() : p_pantalla.acc_info_volcado();
		auto pos=Punto{x, y};
		int 	tx=iv.pos_x+pos.x-iv.rel_x, 
			ty=iv.pos_y+pos.y-iv.rel_y;

		//En caso de que haya zoom compensamos el movimiento que causa la escala.
		//Básicamente estamos desplazando la mitad, dos tercios, tres cuartos...
		if(iv.zoom!=1.0)
		{
			glScaled(iv.zoom, iv.zoom, 1.0);
			//Puro empirismo.
			glTranslatef((iv.pos_x / iv.zoom) - iv.pos_x, (iv.pos_y / iv.zoom) - iv.pos_y, 0.f);
		}

		glTranslatef(tx, ty, 0.f);

		const auto& transformacion=acc_transformacion_rotacion();
		if(transformacion.angulo_rotacion != 0.f)
		{
			glTranslatef(transformacion.centro_rotacion.x, transformacion.centro_rotacion.y, 0.f);
			glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
			glTranslatef(-transformacion.centro_rotacion.x, -transformacion.centro_rotacion.y, 0.f);
		}
*/
		if(p_camara!=nullptr) 
		{
			r->volcar(p_pantalla, *p_camara, x+ex, y+ey);
		}
		else 
		{
			r->volcar(p_pantalla, x+ex, y+ey);
		}

		if(impone_alpha) r->establecer_alpha(alpha_a);
		if(impone_modo_blend) r->establecer_modo_blend(modo_blend_a);
//		r->establecer_mod_color(mod_color_r_a, mod_color_g_a, mod_color_b_a);
	}
}

void Representacion_agrupada::volcado()
{
	//Lulz circus. Nunca llegaríamos aquí.
}

void Representacion_agrupada::insertar_representacion(Representacion * p_rep)
{
	this->grupo.push_back(p_rep);
	actualizar_posicion_vista_rotacion();
}

void  Representacion_agrupada::ir_a(int px, int py)
{
	x=px;
	y=py;
	actualizar_posicion_vista_rotacion();
}

Punto Representacion_agrupada::obtener_posicion() const
{
	return Punto{x, y};
}

Rect Representacion_agrupada::obtener_base_posicion_vista() const
{
	if(!grupo.size())
	{
		return Rect{0,0,0,0};
	}
	else
	{
		Rect res=grupo[0]->obtener_base_posicion_vista();

		res.x+=x;
		res.y+=y;

		int 	fx=res.x+res.w,
			fy=res.y+res.h;

		for(const auto& r : grupo)
		{
			Rect pr=r->obtener_base_posicion_vista();

			pr.x+=x;
			pr.y+=y;

			if(pr.x < res.x) res.x=pr.y;
			if(pr.y < res.y) res.y=pr.y;

			int 	prfx=pr.x+pr.w,
				prfy=pr.y+pr.h;

			if(prfx > fx) fx=prfx;
			if(prfy > fy) fy=prfy;
		}

		res.w=fx-res.x;
		res.h=fy-res.y;

		return res;
	}
}

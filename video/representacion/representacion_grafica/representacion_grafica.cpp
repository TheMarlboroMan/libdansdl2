#include "representacion_grafica.h"
#include <algorithm>
#include "../../../herramientas/poligono_2d/poligono_2d.h"

using namespace DLibV;

extern DLibH::Log_base LOG;


Representacion_grafica::Representacion_grafica()
	:Representacion(), textura(nullptr), pincel{0,0}
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
	actualizar_caja_rotacion();
}

Representacion_grafica::Representacion_grafica(ColorRGBA color)
	:Representacion(color), textura(nullptr),pincel{0,0}
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
	actualizar_caja_rotacion();
}

Representacion_grafica::Representacion_grafica(const Representacion_grafica& o)
	:Representacion(o) ,textura(o.textura),
	posicion_rotada(o.posicion_rotada), pincel(o.pincel), puntos(o.puntos), 
	final_ptex(o.final_ptex)
{

}

Representacion_grafica& Representacion_grafica::operator=(const Representacion_grafica& o)
{
	Representacion::operator=(o);
	textura=o.textura;
	posicion_rotada=o.posicion_rotada;
	pincel=o.pincel;
	puntos=o.puntos;
	final_ptex=o.final_ptex;

	return *this;
}

Representacion_grafica::~Representacion_grafica()
{
	//OJO: No se borra el recurso gráfico porque se asume que lo hemos
	//obtenido de un gestor de recursos. Esta clase base NO gestiona los
	//recursos asignados. Una superior, en cambio, si podría.
}

void Representacion_grafica::recorte_a_medidas_textura()
{
	establecer_recorte(0,0, textura->acc_w(), textura->acc_h());
}

void Representacion_grafica::volcado()
{
	const Rect& pos=acc_posicion();

	glMatrixMode(GL_MODELVIEW);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glBindTexture(GL_TEXTURE_2D, textura->acc_indice());
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//Alpha...
	const auto c=acc_rgba();
	switch(acc_modo_blend())
	{
		case Representacion::blends::BLEND_NADA:
			glDisable(GL_BLEND);
			glColor3f(c.r, c.g, c.b);
		break;
		case Representacion::blends::BLEND_ALPHA:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(c.r, c.g, c.b, c.a);
		break;
	}
		
	glTranslatef(pos.x, pos.y, 0.f);

	if(!puntos.size() || final_ptex.size())
	{
		calcular_puntos();
	}

	if(transformacion.angulo_rotacion != 0.f)
	{
		//Translación adicional para poder colocar el centro de rotación.
		glTranslatef(transformacion.x_centro_rotacion, transformacion.y_centro_rotacion, 0.f);

		//Actualización de los puntos para colocar el centro.
		for(auto& p : puntos){
			p.x-=transformacion.x_centro_rotacion;
			p.y-=transformacion.y_centro_rotacion;
		}

		//Rotación...
		glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	glVertexPointer(2, GL_INT, 0, puntos.data());
	glTexCoordPointer(2, GL_FLOAT, 0, final_ptex.data());
	glDrawArrays(GL_QUADS, 0, puntos.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glDisable(GL_TEXTURE_2D);

	//Restaurar el estado de opengl.
	if(transformacion.angulo_rotacion!=0.f)
	{
		glRotatef(-transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
		glTranslatef(-transformacion.x_centro_rotacion, -transformacion.y_centro_rotacion, 0.f);
	}
	glTranslatef(-pos.x, -pos.y, 0.f);
}

//Calcula las posiciones de los vértices y los puntos de las texturas.
//Dado que las posiciones se calculan a partir de 0.0 no debería haber problemas
//al cambiar la posición.
//Los valores calculados se almacenan.

void Representacion_grafica::calcular_puntos()
{
	const Rect& pos=acc_posicion();
	const Rect& recor=acc_recorte();
	
	if(!pincel.w) pincel.w=pos.w;
	if(!pincel.h) pincel.h=pos.h;

	float w_tex=textura->acc_w(), h_tex=textura->acc_h();

	puntos.clear();
	final_ptex.clear();

	int itx=0;

//TODO: What the hell is dancing????.

	for(int x=0; x < (int)pos.w; x+=pincel.w)
	{
		int ity=0;
		const int dif_x=x+pincel.w > (int)pos.w ? pos.w - (itx * pincel.w)  : pincel.w;

		for(int y=0; y < (int)pos.h; y+=pincel.h)
		{
			const int dif_y=y+pincel.h > (int)pos.h ? pos.w - (ity * pincel.h) : pincel.h;

			punto pts[]={{x, y}, {x+dif_x, y}, {x+dif_x, y+dif_y}, {x, y+dif_y}};

			//Samplear los puntos centrales de las cuatro esquinas. De momento no buscamos el centro del texel. 
			//El cálculo de ptex_fx y ptex_fy está sacando la proporción entre el pincel y la cantidad de espacio
			//que queda por dibujar (una simple regla de tres). La finalidad es mapear sólo el trozo de textura
			//necesario.
 
			GLfloat ptex_x=(GLfloat)recor.x,
				ptex_y=(GLfloat)recor.y, 
				ptex_fx=ptex_x+( ( (GLfloat)dif_x * (GLfloat)recor.w) / (GLfloat)pincel.w), 
				ptex_fy=ptex_y+( ( (GLfloat)dif_y * (GLfloat)recor.h) / (GLfloat)pincel.h);

			puntotex ptex[]={
				{ptex_x,	ptex_y},
				{ptex_fx,	ptex_y},
				{ptex_fx,	ptex_fy},
				{ptex_x,	ptex_fy}};

			//Invertir y resamplear puntos...
			if(transformacion.invertir_horizontal)
			{
				std::swap(ptex[0].x, ptex[1].x);
				std::swap(ptex[2].x, ptex[3].x);
				for(auto &p: ptex) p.x-=0.5f;
			}

			if(transformacion.invertir_vertical)
			{
				std::swap(ptex[0].y, ptex[2].y);
				std::swap(ptex[1].y, ptex[3].y); 
				for(auto &p: ptex) p.y-=0.5f;
			}

			for(auto &p : ptex)
			{
				p.x/=w_tex; 
				p.y/=h_tex;
			}

			final_ptex.insert(std::end(final_ptex), ptex, ptex+4);
			puntos.insert(std::end(puntos), pts, pts+4);
			++ity;
		}
		++itx;
	}
}

//Eso sólo deberíamos llamarlo en aquellas para las cuales hemos creado una
//textura que poseen. Si es parte de un recurso que no es de su propiedad
//te vas a encontrar con un problema.

void Representacion_grafica::liberar_textura()
{
	if(this->textura)
	{
		delete this->textura;
		this->textura=nullptr;
	}
}

void Representacion_grafica::transformar_rotar(float v) 
{
	transformacion.angulo_rotacion=v;
	actualizar_caja_rotacion();
}

void Representacion_grafica::transformar_cancelar_rotar() 
{
	transformacion.angulo_rotacion=0.f;
	actualizar_caja_rotacion();
}

void Representacion_grafica::transformar_centro_rotacion(float x, float y) 
{
	transformacion.centro_rotacion(x, y);
	actualizar_caja_rotacion();
}

void Representacion_grafica::transformar_centro_rotacion_cancelar() 
{
	transformacion.cancelar_centro_rotacion();
	actualizar_caja_rotacion();
}

void Representacion_grafica::actualizar_caja_rotacion()
{
	const auto& p=acc_posicion();

	if(!transformacion.es_transformacion())
	{
		posicion_rotada=p;
	}
	else
	{
		//TODO: Restore... We deleted obtener_centro_rotacion(),

/*		auto c=transformacion.acc_centro_rotacion();
		DLibH::Poligono_2d_vertices<double> polig(
			{ 
				{(double)p.x, (double)p.y},
				{(double)(p.x+p.w), (double)p.y},
				{(double)(p.x+p.w), (double)(p.y+p.h)},
				{(double)p.x, (double)(p.y+p.h)},        
			}, {(double)c.x+p.x, (double)c.y+p.y});

		//Las rotaciones de SDL son "clockwise"... Las reales son "counter-clockwise"...
		float a=transformacion.acc_angulo_rotacion();
		polig.rotar(a);

		//Sacar las medidas para la nueva caja...
		std::vector<double> xs={polig.vertice(0).x, polig.vertice(1).x, polig.vertice(2).x, polig.vertice(3).x};
		std::vector<double> ys={polig.vertice(0).y, polig.vertice(1).y, polig.vertice(2).y, polig.vertice(3).y};

		posicion_rotada.x=*std::min_element(std::begin(xs), std::end(xs));
		posicion_rotada.y=*std::min_element(std::begin(ys), std::end(ys));
		posicion_rotada.w=*std::max_element(std::begin(xs), std::end(xs))-posicion_rotada.x;
		posicion_rotada.h=*std::max_element(std::begin(ys), std::end(ys))-posicion_rotada.y;
*/
	}
}

void Representacion_grafica::establecer_posicion(int x, int y, int w, int h, int f)
{
	Representacion::establecer_posicion(x, y, w, h, f);
	actualizar_caja_rotacion();
}

void Representacion_grafica::establecer_posicion(Rect c)
{
	Representacion::establecer_posicion(c);
	actualizar_caja_rotacion();
}

Rect Representacion_grafica::copia_posicion_rotada() const
{
	return Rect{posicion_rotada.x, posicion_rotada.y, posicion_rotada.w, posicion_rotada.h};
}

//TODO: Does this affect us???
/*
void Representacion_grafica::preparar(const SDL_Renderer * renderer)
{
	actualizar_caja_rotacion();
	marcar_como_preparada();
}
*/

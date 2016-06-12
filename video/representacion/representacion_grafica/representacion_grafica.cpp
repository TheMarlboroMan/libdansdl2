#include "representacion_grafica.h"
#include <algorithm>
#include "../../../herramientas/poligono_2d/poligono_2d.h"

using namespace DLibV;

extern DLibH::Log_base LOG;

Representacion_grafica::Representacion_grafica()
	:Representacion(), textura(nullptr), preparada(false)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
	actualizar_caja_rotacion();
}

Representacion_grafica::Representacion_grafica(const Representacion_grafica& o)
	:Representacion(o) ,textura(o.textura), preparada(o.preparada),
	posicion_rotada(o.posicion_rotada)
{

}

Representacion_grafica& Representacion_grafica::operator=(const Representacion_grafica& o)
{
	Representacion::operator=(o);
	textura=o.textura;
	preparada=o.preparada;
	posicion_rotada=o.posicion_rotada;

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

//TODO: Fuck the renderer parameter.
void Representacion_grafica::volcado(SDL_Renderer * p_renderer)
{
	const SDL_Rect& pos=acc_posicion();
	const SDL_Rect& recor=acc_recorte();

	//TODO: This should be stored when set.
	struct punto{int x, y;};
	punto puntos[]={{0, 0},
		{pos.w, 0}, 
		{pos.w, pos.h}, 
		{0, pos.h} };


	//TODO: This is a fucking disaster. Store it somewhere when it's set.
	float w_tex=textura->acc_w(), h_tex=textura->acc_h();
	struct puntotex {GLfloat x, y;};

	//Samplear los puntos centrales. De momento no buscamos el centro del texel.
	puntotex ptex[]={
		{(GLfloat)recor.x, 			(GLfloat)recor.y},
		{(GLfloat)recor.x+(GLfloat)recor.w, 	(GLfloat)recor.y},
		{(GLfloat)recor.x+(GLfloat)recor.w, 	(GLfloat)recor.y+(GLfloat)recor.h},
		{(GLfloat)recor.x, 			(GLfloat)recor.y+(GLfloat)recor.h}};

	//Invertir y resamplear puntos...
	if(transformacion.invertir_horizontal)
	{
		std::swap(ptex[0].x, ptex[1].x);
		std::swap(ptex[2].x, ptex[3].x);
		for(auto &p: ptex) p.x-=0.5f;		
	}

	//Invertir y resamplear puntos...
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

	glMatrixMode(GL_MODELVIEW);
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, textura->acc_indice());
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

	glTranslatef(pos.x, pos.y, 0.f);

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

	glVertexPointer(2, GL_INT, 0, puntos);
	glTexCoordPointer(2, GL_FLOAT, 0, ptex);
	glDrawArrays(GL_QUADS, 0, 4);

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

void Representacion_grafica::volcado(SDL_Renderer * p_renderer, const SDL_Rect& p_foco, const SDL_Rect& p_pos, double zoom)
{
	//TODO...
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

void Representacion_grafica::establecer_posicion(SDL_Rect c)
{
	Representacion::establecer_posicion(c);
	actualizar_caja_rotacion();
}

SDL_Rect Representacion_grafica::copia_posicion_rotada() const
{
	return SDL_Rect{posicion_rotada.x, posicion_rotada.y, posicion_rotada.w, posicion_rotada.h};
}

void Representacion_grafica::preparar(const SDL_Renderer * renderer)
{
	actualizar_caja_rotacion();
	marcar_como_preparada();
}

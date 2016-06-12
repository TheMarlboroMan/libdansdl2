#include "camara.h"

using namespace DLibV;

Camara::Camara(int p_x, int p_y, unsigned int p_w, unsigned int p_h, unsigned int p_px, unsigned int p_py):
	pos_x(p_px), pos_y(p_py),
	/*con_clip(true),*/ limitada(false), limite_min_x(0), limite_min_y(0),
	limite_max_x(0), limite_max_y(0), zoom(1.0)
{
	this->caja_foco.x=p_x;
	this->caja_foco.y=p_y;
	this->caja_foco.w=p_w;
	this->caja_foco.h=p_h;

	this->sincronizar_cajas();
	this->caja_pos.w=this->caja_foco.w;
	this->caja_pos.h=this->caja_foco.h;
}

void Camara::sincronizar_cajas()
{
	this->caja_pos.x=this->pos_x;
	this->caja_pos.y=this->pos_y;
}

/*Mueve la posición a la que apunta la cámara en la pantalla. Se usan las
coordenadas provistas.*/

void Camara::enfocar_a(int p_x, int p_y)
{
	if(this->limitada)
	{
		auto procesar=[](int pos, int dimension, int limite_min, int limite_max, int &blanco)
		{
			int fin=pos + dimension;
			if(pos >= limite_min && fin  <= limite_max) blanco=pos;
			else if(pos < limite_min) blanco=limite_min;
			else if(fin > limite_max) blanco=limite_max-dimension;
		};

		if((int)caja_foco.w <= limite_max_x - limite_min_x) procesar(p_x, caja_foco.w, limite_min_x, limite_max_x, caja_foco.x);
		if((int)caja_foco.h <= limite_max_y - limite_min_y) procesar(p_y, caja_foco.h, limite_min_y, limite_max_y, caja_foco.y);
	}
	else
	{
		this->caja_foco.x=p_x;
		this->caja_foco.y=p_y;
	}

	this->sincronizar_cajas();
}

/*Mueve la posición a la que apunta la cámara sumando las cantidades provistas
en los parámetros.*/

void Camara::movimiento_relativo(int p_x, int p_y)
{
	this->enfocar_a(this->caja_foco.x+p_x, this->caja_foco.y+p_y);
}

/*Establece los límites a los que la cámara puede apuntar: es decir, los límites
máximos para la cámara dentro del "mundo".*/

void Camara::establecer_limites(int p_min_limite_x, int p_min_limite_y, int p_max_limite_x, int p_max_limite_y)
{
	this->limitada=true;
//	this->limite_max_x=p_max_limite_x-this->caja_foco.w;
//	this->limite_max_y=p_max_limite_y-this->caja_foco.h;
	this->limite_max_x=p_max_limite_x; //-this->caja_pos.w;
	this->limite_max_y=p_max_limite_y; //-this->caja_pos.h;
	this->limite_min_x=p_min_limite_x;
	this->limite_min_y=p_min_limite_y;
}

void Camara::limpiar_limite()
{
	this->limitada=false;
	this->limite_max_x=0;
	this->limite_max_y=0;
	this->limite_min_x=0;
	this->limite_min_y=0;
}

void Camara::transformar_posicion_raton(int& x, int& y)
{
	x=caja_foco.x + (x * (caja_foco.w / (float) caja_pos.w));
	y=caja_foco.y + (y * (caja_foco.h / (float) caja_pos.h));
}

void Camara::mut_zoom(double v)
{
	zoom=v;
	caja_foco.w=caja_pos.w*v;
	caja_foco.h=caja_pos.h*v;
}

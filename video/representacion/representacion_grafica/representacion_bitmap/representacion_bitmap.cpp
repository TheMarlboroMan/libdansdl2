#include "representacion_bitmap.h"

using namespace DLibV;
Representacion_bitmap::Representacion_bitmap():Representacion_grafica()
{

}

Representacion_bitmap::Representacion_bitmap(const Textura * p_rec):Representacion_grafica()
{
	this->establecer_textura(p_rec);
	this->establecer_dimensiones_posicion_por_recorte();
}

Representacion_bitmap::Representacion_bitmap(const Representacion_bitmap& o)
	:Representacion_grafica(o)
{

}

Representacion_bitmap::~Representacion_bitmap()
{

}

Representacion_bitmap& Representacion_bitmap::operator=(const Representacion_bitmap & p_otra)
{
	Representacion_grafica::operator=(p_otra);
	return *this;
}

void Representacion_bitmap::establecer_textura(Textura const * p_recurso)
{
	Representacion_grafica::establecer_textura(p_recurso);
	this->recorte_a_medidas_textura();
}

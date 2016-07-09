#include "representacion_primitiva_caja.h"

using namespace DLibV;

Representacion_primitiva_caja::Representacion_primitiva_caja(Representacion_primitiva_poligono::tipo t,const Rect& p_pos, const ColorRGBA& c)
	:Representacion_primitiva_poligono(t, { 
		{p_pos.x, p_pos.y}, 
		{p_pos.x+(int)p_pos.w, p_pos.y}, 
		{p_pos.x+(int)p_pos.w, p_pos.y+(int)p_pos.h}, 
		{p_pos.x, p_pos.y+(int)p_pos.h}}, 
		c)
{

}

Representacion_primitiva_caja& Representacion_primitiva_caja::operator=(const Representacion_primitiva_caja& p_otro)
{
	Representacion_primitiva_poligono::operator=(p_otro);
	return *this;
}

Representacion_primitiva_caja::Representacion_primitiva_caja(const Representacion_primitiva_caja& p_otra)
	:Representacion_primitiva_poligono(p_otra) 
{

}

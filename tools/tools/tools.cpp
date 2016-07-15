#include "tools.h"

using namespace ldt;

float ldt::angle_from_points(float x1, float y1, float x2, float y2)
{
	float theta = atan2(y2 - y1, x2 - x1);
	theta += M_PI/2.0; //0 apunta ahora hacia el norte...
	float angle=theta * 180 / M_PI; //Convertir a grados.
	if(angle < 0) angle+=360;
	return angle;
}

float ldt::percent(float p_part, float p_total)
{
	if(p_total==0) return 0;
	else return (p_part * 100) / p_total;
}

float ldt::degree_360(float a)
{
	if(a < 0.f)
	{
		a=180.f + 180.f + a;
	}

	return a;
}

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

bool ldt::point_in_box(int cx, int cy, unsigned int cw, unsigned int ch, int px, int py)
{
        return  (cx <= px && (int)(cx+cw) >= px)
        &&
        (cy <= py && (int)(cy+ch) >= py);
}

bool ldt::box_in_box(int pqx, int pqy, unsigned int pqw, unsigned int pqh, int grx, int gry, unsigned int grw, unsigned int grh)
{
	int big_xf=grx+grw,
	small_xf=pqx+pqw,
	big_yf=gry+grh,
	small_yf=pqy+pqh;

	return (grx <= pqx
	&& big_xf >= small_xf
	&& gry <= pqy
	&& big_yf >= small_yf);
}

float ldt::degree_360(float a)
{
	if(a < 0.f)
	{
		a=180.f + 180.f + a;
	}

	return a;
}

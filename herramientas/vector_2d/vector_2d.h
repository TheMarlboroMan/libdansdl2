#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <cmath>

namespace DLibH
{

/**
* Un vector usable directamente, agnóstico de su tipo. Simplemente indica una
* dirección y una velocidad.
*/

struct Vector_2d_cartesiano;
struct Vector_2d_pantalla;

struct Vector_2d
{
	float x;
	float y;

	Vector_2d(): x(0.f), y(0.f) {}
	Vector_2d(const Vector_2d& o): x(o.x), y(o.y) {}
	Vector_2d(float p_x, float p_y):x(p_x), y(p_y) {}

	Vector_2d operator+(const Vector_2d &otro);
	Vector_2d operator-(const Vector_2d &otro);
	Vector_2d operator*(const Vector_2d &otro);
	Vector_2d operator/(const Vector_2d &otro);

	Vector_2d& operator=(const Vector_2d &otro);

	Vector_2d& operator+=(const Vector_2d &otro);
	Vector_2d& operator-=(const Vector_2d &otro);
	Vector_2d& operator*=(const Vector_2d &otro);
	Vector_2d& operator/=(const Vector_2d &otro);

	Vector_2d& operator*(const float p_multiplicador);
	Vector_2d& operator/(const float p_divisor);
	Vector_2d& operator*=(const float p_multiplicador);
	Vector_2d& operator/=(const float p_divisor);

	void normalizar();
	float longitud() const;
	float angulo_radianes() const;
	float angulo_grados() const;

	static float obtener_angulo_para_vector_unidad_radianes(const Vector_2d&);
	static float obtener_angulo_para_vector_unidad_grados(const Vector_2d&);

	static Vector_2d vector_unidad_para_angulo(float);

	static Vector_2d_cartesiano obtener_para_puntos_cartesiano(float p_xa, float p_ya, float p_xb, float p_yb);
	static Vector_2d_pantalla obtener_para_puntos_pantalla(float p_xa, float p_ya, float p_xb, float p_yb);
};

/**
* Este otro vector responde a las coordenadas de pantalla. Y negativo es arriba.
* Y positivo es abajo. Simplemente existe para ser un tipo separado y que no
* haya confusiones luego en el código cliente.
*/

struct Vector_2d_pantalla:
	public Vector_2d
{
						Vector_2d_pantalla(float px, float py):Vector_2d(px, py) {}
						Vector_2d_pantalla(const Vector_2d& v);
						Vector_2d_pantalla& operator=(const Vector_2d& v);
	Vector_2d_cartesiano			a_cartesiano() const;
	Vector_2d_pantalla 			operator+(const Vector_2d &otro);
	Vector_2d_pantalla 			operator-(const Vector_2d &otro);
	Vector_2d_pantalla 			operator*(const Vector_2d &otro);
	Vector_2d_pantalla 			operator/(const Vector_2d &otro);
	Vector_2d_pantalla& 			operator+=(const Vector_2d &otro);
	Vector_2d_pantalla& 			operator-=(const Vector_2d &otro);
	Vector_2d_pantalla& 			operator*=(const Vector_2d &otro);
	Vector_2d_pantalla& 			operator/=(const Vector_2d &otro);
	Vector_2d_pantalla&			operator*(const float p_multiplicador);
	Vector_2d_pantalla& 			operator/(const float p_divisor);
	Vector_2d_pantalla& 			operator*=(const float p_multiplicador);
	Vector_2d_pantalla& 			operator/=(const float p_divisor);	
};

/**
* Este vector es cartesiano: y positivo es arriba, y negativo es abajo.
*/

struct Vector_2d_cartesiano:
	public Vector_2d
{
						Vector_2d_cartesiano(float px, float py):Vector_2d(px, py) {}
						Vector_2d_cartesiano(const Vector_2d& v);
						Vector_2d_cartesiano& operator=(const Vector_2d& v);
	Vector_2d_pantalla			a_pantalla() const;
	Vector_2d_cartesiano 			operator+(const Vector_2d &otro);
	Vector_2d_cartesiano 			operator-(const Vector_2d &otro);
	Vector_2d_cartesiano 			operator*(const Vector_2d &otro);
	Vector_2d_cartesiano 			operator/(const Vector_2d &otro);
	Vector_2d_cartesiano& 			operator+=(const Vector_2d &otro);
	Vector_2d_cartesiano& 			operator-=(const Vector_2d &otro);
	Vector_2d_cartesiano& 			operator*=(const Vector_2d &otro);
	Vector_2d_cartesiano& 			operator/=(const Vector_2d &otro);
	Vector_2d_cartesiano&			operator*(const float p_multiplicador);
	Vector_2d_cartesiano& 			operator/(const float p_divisor);
	Vector_2d_cartesiano& 			operator*=(const float p_multiplicador);
	Vector_2d_cartesiano& 			operator/=(const float p_divisor);
	
};

} //Fin namespace DLibH
#endif

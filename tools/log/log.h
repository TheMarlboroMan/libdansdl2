#ifndef LIBDANSDL2_LOG_H
#define LIBDANSDL2_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstring>
#include <mutex>

/*Esto es para sustituir y al guardián. Es más de lo mismo pero está un poco
más actualizado. Básicamente va guardando lo que se meta con el operador <<
pero adicionalmente permite recibir por medio de ese operador los dos structs
log_base_n y log_base_m, que van cambiando el nivel de "error".

Por defecto está en nivel de error 10. Todo aquello que esté por encima del 
nivel de error no sólo se guardará en un archivo, sino que se almacenará en una
cadena propia de la clase. Todo lo que esté por debajo no se almacenará en la
cadena.

La cadena la podemos usar para mostrarla por cout o por medio de alguna 
representación de SDL.

Se incluye además una clase log_motor, que es una aplicación singleton de esta.
Se usará dentro del motor para notificar los posibles errores.


*/

namespace ldt
{

enum class lin{error, warning, info};
enum class lcut{error, warning, info, all};
enum class lop{lock, unlock};
enum class ltime{date, time, datetime};

class log
{
	public:

	//This enum just bitwises the levels.
	enum levels{all=0, info=1, warning=2, error=3};

	lcut	int_to_lcut(int v);
	log();
	log(const char * filename);
	~log();

	void activate();
	void deactivate();
	void init(const char * filename);

	template <class X> log& operator<<(const X &input)
	{
		if(is_usable() && check_levels())
		{
			s<<input;
			s.flush();
		}
		return *this;
	}

	log& operator<<(lop);
	log& operator<<(lin);
	log& operator<<(lcut);
	log& operator<<(ltime);
	log& operator<<(std::ostream& ( *pf )(std::ostream&));
	log& operator<<(std::ios& ( *pf )(std::ios&));
	log& operator<<(std::ios_base& ( *pf )(std::ios_base&));

	private:

	bool					check_levels() {return entry_level >= min_level;}
	bool 					is_usable() {return active && s.is_open();}
	std::string				date() const;
	std::string				time() const;

	std::mutex				mtx;
	std::ofstream 				s;	
	int 					entry_level,
						min_level;
	bool 					active;
};

/*Incluimos un singleton aquí, para uso del motor. Sería algo así
como DLibH::log_motor::L()<<"aquí tu log";

Un detalle: cuando se inicia está inactive. Hay que habilitarlo manualmente 
haciendo log_motor::L().arrancar();
*/


class log_lsdl
{
	public:

	static log& get()
	{
		if(!l) 
		{
			l=new log();
		}
		
		return *l;
	}

	static void init(const std::string& path)
	{
		if(!l) 
		{
			l=new log();
		}

		l->init(path.c_str());
		l->activate();
	}

	static void end()
	{
		if(l)
		{
			delete l;
			l=nullptr;
		}
	}

	private:

	static log * l;

};

}
#endif

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

enum class lin{error, warning, operation};
enum class lcut{error, warning, operation, all};
enum class lop{lock, unlock};
enum class ltime{now};

class log
{
	public:

	//This enum just bitwises the levels.
	enum levels{all=0, operation=1, warning=2, error=3};

	lcut	int_to_lcut(int v)
	{
		if(v < 0 || v > 3)
		{
			throw std::runtime_error("Invalid cut level "+std::to_string(v)+" specified for int_to_lcut");
		}
	
		switch(v)
		{
			case error: 	return lcut::error; break;
			case warning: 	return lcut::warning; break;
			case operation: return lcut::operation; break;
			case all: 	return lcut::all; break;
		}	

		//Compiler: just shut up.
		return lcut::all;
	}

	log()
		:s(), entry_level(levels::all), min_level(levels::all), 
		active(false)
	{

	}

	log(const char * filename)
		:s(), entry_level(levels::all), min_level(levels::all), 
		active(true)
	{
		init(filename);
	}

	~log()
	{
		if(is_usable())
		{
			(*this)<<"Session ends "<<ltime::now<<std::endl;
			s.close();
		}
	}

	void activate() 
	{
		active=true;
		if(is_usable())
		{
			(*this)<<"Session starts "<<ltime::now<<std::endl;
		}
	}
	void deactivate() 
	{
		if(is_usable())
		{
			(*this)<<"Session ends "<<ltime::now<<std::endl;
		}

		active=false;
	}

	void init(const char * filename)
	{
		s.open(filename);

		if(is_usable())
		{
			(*this)<<"Session starts "<<ltime::now<<std::endl;
		}
	}

	//Esta es la entrada para todo lo que hay... Si el nivel actual es 
	//mayor o igual que el nivel mínimo se logueará también en la cadena.
	template <class X> log& operator<<(const X &input)
	{
		if(is_usable() && check_levels())
		{
			s<<input;
			s.flush();
		}
		return *this;
	}
	
	log& operator<<(lop op)
	{
		switch(op)
		{
			case lop::lock: 	mtx.lock(); break;
			case lop::unlock: 	mtx.unlock(); break;
		}
		return *this;
	}

	//Establece el nivel de los mensajes entrantes.
	log& operator<<(lin lvl)
	{
		switch(lvl)
		{
			case lin::error:
				s<<"[ERROR] ";
				entry_level=error; 
			break;
			case lin::warning:
				s<<"[WARNING] ";
				entry_level=warning; 
			break;
			case lin::operation:
				s<<"[INFO] ";
				entry_level=operation; 
			break;
		}
		return *this;
	}

	log& operator<<(lcut lvl)
	{
		switch(lvl)
		{
			case lcut::error: 	min_level=error; break;
			case lcut::warning:	min_level=warning; break;
			case lcut::operation:	min_level=operation; break;
			case lcut::all:		min_level=all; break;
		}
		return *this;
	}

	//Coloca la hora.
	log& operator<<(ltime v)
	{
		if(is_usable())
	 	{
			switch(v)
			{
				case ltime::now:
				{
					char * t=new char[14];
					memset(t, '\0', 14);
					time_t tiempo=time(NULL);
					strftime(t, 14, "%H:%M:%S >> ", localtime(&tiempo));
					s<<"["<<t<<"]";
					delete [] t ;
				}
				break;
			}
		}
		
		return *this;
	}

	log& operator<<(std::ostream& ( *pf )(std::ostream&))
	{
		if(is_usable() && check_levels())
		{
			s<<pf;
		}
		return *this;
	}

	log& operator<<(std::ios& ( *pf )(std::ios&))
	{
		if(is_usable() && check_levels())
		{
			s<<pf;
		}
		return *this;
	}

	log& operator<<(std::ios_base& ( *pf )(std::ios_base&))
	{
		if(is_usable() && check_levels())
		{
			s<<pf;
		}
		return *this;
	}

	private:

	bool					check_levels() {return entry_level >= min_level;}
	bool 					is_usable() {return active && s.is_open();}

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

	static log& L()
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

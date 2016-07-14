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
Log_base_n y Log_base_m, que van cambiando el nivel de "error".

Por defecto está en nivel de error 10. Todo aquello que esté por encima del 
nivel de error no sólo se guardará en un archivo, sino que se almacenará en una
cadena propia de la clase. Todo lo que esté por debajo no se almacenará en la
cadena.

La cadena la podemos usar para mostrarla por cout o por medio de alguna 
representación de SDL.

Se incluye además una clase Log_motor, que es una aplicación singleton de esta.
Se usará dentro del motor para notificar los posibles errores.


*/

namespace ldt
{

enum class Lin{error, warning, operation};
enum class Lcut{error, warning, operation, all};
enum class Lop{lock, unlock};
enum class Ltime{now};

class Log
{
	public:

	//This enum just bitwises the levels.
	enum Levels{all=0, operation=1, warning=2, error=3};

	Lcut	int_to_lcut(int v)
	{
		if(v < 0 || v > 3)
		{
			throw std::runtime_error("Invalid cut level "+std::to_string(v)+" specified for int_to_lcut");
		}
	
		switch(v)
		{
			case error: 	return Lcut::error; break;
			case warning: 	return Lcut::warning; break;
			case operation: return Lcut::operation; break;
			case all: 	return Lcut::all; break;
		}	

		//Compiler: just shut up.
		return Lcut::all;
	}

	Log()
		:s(), entry_level(Levels::all), min_level(Levels::all), 
		active(false)
	{

	}

	Log(const char * filename)
		:s(), entry_level(Levels::all), min_level(Levels::all), 
		active(true)
	{
		init(filename);
	}

	~Log()
	{
		if(is_usable())
		{
			(*this)<<"Session ends "<<Ltime::now<<std::endl;
			s.close();
		}
	}

	void activate() 
	{
		active=true;
		if(is_usable())
		{
			(*this)<<"Session starts "<<Ltime::now<<std::endl;
		}
	}
	void deactivate() 
	{
		if(is_usable())
		{
			(*this)<<"Session ends "<<Ltime::now<<std::endl;
		}

		active=false;
	}

	void init(const char * filename)
	{
		s.open(filename);

		if(is_usable())
		{
			(*this)<<"Session starts "<<Ltime::now<<std::endl;
		}
	}

	//Esta es la entrada para todo lo que hay... Si el nivel actual es 
	//mayor o igual que el nivel mínimo se logueará también en la cadena.
	template <class X> Log& operator<<(const X &input)
	{
		if(is_usable() && check_levels())
		{
			s<<input;
			s.flush();
		}
		return *this;
	}
	
	Log& operator<<(Lop op)
	{
		switch(op)
		{
			case Lop::lock: 	mtx.lock(); break;
			case Lop::unlock: 	mtx.unlock(); break;
		}
		return *this;
	}

	//Establece el nivel de los mensajes entrantes.
	Log& operator<<(Lin lvl)
	{
		switch(lvl)
		{
			case Lin::error:
				s<<"[ERROR] ";
				entry_level=error; 
			break;
			case Lin::warning:
				s<<"[WARNING] ";
				entry_level=warning; 
			break;
			case Lin::operation:
				s<<"[INFO] ";
				entry_level=operation; 
			break;
		}
		return *this;
	}

	Log& operator<<(Lcut lvl)
	{
		switch(lvl)
		{
			case Lcut::error: 	min_level=error; break;
			case Lcut::warning:	min_level=warning; break;
			case Lcut::operation:	min_level=operation; break;
			case Lcut::all:		min_level=all; break;
		}
		return *this;
	}

	//Coloca la hora.
	Log& operator<<(Ltime v)
	{
		if(is_usable())
	 	{
			switch(v)
			{
				case Ltime::now:
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

	Log& operator<<(std::ostream& ( *pf )(std::ostream&))
	{
		if(is_usable() && check_levels())
		{
			s<<pf;
		}
		return *this;
	}

	Log& operator<<(std::ios& ( *pf )(std::ios&))
	{
		if(is_usable() && check_levels())
		{
			s<<pf;
		}
		return *this;
	}

	Log& operator<<(std::ios_base& ( *pf )(std::ios_base&))
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
como DLibH::Log_motor::L()<<"aquí tu log";

Un detalle: cuando se inicia está inactive. Hay que habilitarlo manualmente 
haciendo Log_motor::L().arrancar();
*/


class Log_lsdl
{
	private:

	static Log * l;
	
	public:

	static Log& L()
	{
		if(!l) 
		{
			l=new Log();
		}
		
		return *l;
	}

	static void init(const std::string& path)
	{
		if(!l) 
		{
			l=new Log();
		}

		l->init(path.c_str());
		l->activate();
	}

	static void end()
	{
		if(l)
		{
			delete l;
			l=NULL;
		}
	}
};

}
#endif

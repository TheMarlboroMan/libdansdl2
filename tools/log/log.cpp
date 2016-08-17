#include "log.h"

using namespace ldt;

log * log_lsdl::l=nullptr;

lcut	log::int_to_lcut(int v)
{
	if(v < 0 || v > 3)
	{
		throw std::runtime_error("Invalid cut level "+std::to_string(v)+" specified for int_to_lcut");
	}

	switch(v)
	{
		case error: 	return lcut::error; break;
		case warning: 	return lcut::warning; break;
		case info: return lcut::info; break;
		case all: 	return lcut::all; break;
	}	

	//Compiler: just shut up.
	return lcut::all;
}

log::log()
	:s(), entry_level(levels::all), min_level(levels::all), 
	active(false)
{

}

log::log(const char * filename)
	:s(), entry_level(levels::all), min_level(levels::all), 
	active(true)
{
	init(filename);
}

log::~log()
{
	if(is_usable())
	{
		(*this)<<"Session ends "<<ltime::datetime<<std::endl;
		s.close();
	}
}

void log::activate() 
{
	active=true;
	if(is_usable())
	{
		(*this)<<"Session starts "<<ltime::datetime<<std::endl;
	}
}

void log::deactivate() 
{
	if(is_usable())
	{
		(*this)<<"Session ends "<<ltime::datetime<<std::endl;
	}

	active=false;
}

void log::init(const char * filename)
{
	s.open(filename);

	if(is_usable())
	{
		(*this)<<"Session starts "<<ltime::datetime<<std::endl;
	}
}

log& log::operator<<(lop op)
{
	switch(op)
	{
		case lop::lock: 	mtx.lock(); break;
		case lop::unlock: 	mtx.unlock(); break;
	}
	return *this;
}

//Establece el nivel de los mensajes entrantes.
log& log::operator<<(lin lvl)
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
		case lin::info:
			s<<"[INFO] ";
			entry_level=info; 
		break;
	}
	return *this;
}

log& log::operator<<(lcut lvl)
{
	switch(lvl)
	{
		case lcut::error: 	min_level=error; break;
		case lcut::warning:	min_level=warning; break;
		case lcut::info:	min_level=info; break;
		case lcut::all:		min_level=all; break;
	}
	return *this;
}

//Coloca la hora.
log& log::operator<<(ltime v)
{
	if(is_usable())
 	{
		switch(v)
		{
			case ltime::time:
				operator<<("[")<<time()<<"] ";
			break;
			case ltime::date:
				operator<<("[")<<date()<<"] ";
			break;
			case ltime::datetime:
				operator<<("[")<<date()<<" "<<time()<<"] ";
			break;
		}
	}
	
	return *this;
}

std::string log::time() const
{
	char * t=new char[14];
	memset(t, '\0', 14);
	time_t tiempo=std::time(nullptr);
	strftime(t, 14, "%H:%M:%S", localtime(&tiempo));
	std::string res(t);
	delete [] t ;
	return res;
}

std::string log::date() const
{
	char * t=new char[14];
	memset(t, '\0', 14);
	time_t tiempo=std::time(nullptr);
	strftime(t, 14, "%F", localtime(&tiempo));
	std::string res(t);
	delete [] t ;
	return res;
}

log& log::operator<<(std::ostream& ( *pf )(std::ostream&))
{
	if(is_usable() && check_levels())
	{
		s<<pf;
	}
	return *this;
}

log& log::operator<<(std::ios& ( *pf )(std::ios&))
{
	if(is_usable() && check_levels())
	{
		s<<pf;
	}
	return *this;
}

log& log::operator<<(std::ios_base& ( *pf )(std::ios_base&))
{
	if(is_usable() && check_levels())
	{
		s<<pf;
	}
	return *this;
}

#ifndef LIBDANSDL2_LOG_H
#define LIBDANSDL2_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstring>
#include <mutex>
#include <log.h>

namespace ldt
{

//!Singleton instance for internal use of the library.

class log_lsdl
{
	public:

	//!Gets the log instance.

	static tools::log& get()
	{
		if(!l) {
			l=new tools::log();
		}
		
		return *l;
	}

	//!Inits the log with the output set to path.

	static void init(const std::string& path) {
		if(!l) {
			l=new tools::log();
		}

		l->init(path.c_str());
		l->activate();
	}

	//!Deletes the log, closing the file.
	static void end() {
		if(l) {
			delete l;
			l=nullptr;
		}
	}

	private:

	static tools::log * l;
};

}
#endif

#pragma once

#include <sstream>
#include <stdexcept>
#include <memory>

#include <src/file_logger.h>

namespace ldt
{

//!Singleton instance for internal use of the library, implemented in terms of
//!log::log. The call sequence should be set_type, set_filename (if needed)
//!and finally, get.
class log_lsdl
{
	public:
	
	//!Specifies the kind of logger that will be used.
	enum class types{file, out, null};

	//!Specifies where the library log will live. Throws if the log has
	//!been already instanced. Throws if the log type is not a file.
	static void set_filename(const std::string& _str) {

		if(nullptr!=l) {
			throw std::runtime_error("cannot set location on already instanced log_lsdl");
		}

		if(types::file!=type) {
			throw std::runtime_error("cannot set location of non-file log_lsdl");
		}

		log_location=_str;
	}

	//!Allows the log type to be set. Throws when the log has been already
	//!instanced.
	static void set_type(types _t) {

		if(nullptr!=l) {
			throw std::runtime_error("cannot set type on already instanced log_lsdl");
		}
		type=_t;
	}

	//!Gets the log instance, building the log if it wasn't built already.
	//!Might throw if a file_logger is being built and the file location
	//!was not specified.
	static tools::logger& get()
	{
		if(nullptr!=l) {
			switch(type) {
				case types::file:
					if(!filename.size()) {
						throw std::runtime_error("lsdl_log file needs a filename");
					}
					l.reset(new log::file_logger(filename));
				break;
				case types::out:
					l.reset(new log::ostream_logger(std::cout));
				break;
				case types::null:
					l.reset(new log::void_logger());
				break;
		}
		
		return *(l.get());
	}
	
	private:

	static types type;
	static std::string filename;
	static std::unique_ptr<tools::logger> l;
};

}

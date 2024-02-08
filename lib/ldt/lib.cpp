#include <ldt/lib.h>
#include <sstream>

using namespace ldt;

std::string ldt::get_lib_version() {

	std::stringstream ss;
	ss<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"-"<<LIB_VERSION<<"-"<<RELEASE_VERSION;

	return ss.str();
}

#include "log.h"

using namespace ldt;

std::unique_ptr<tools::logger> log_lsdl::l;
ldt::lsdl_log::types log_lsdl::type=ldt::lsdl_log:types::file;
std::string ldt::log_lsdl::filename;

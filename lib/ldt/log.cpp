#include <ldt/log.h>

using namespace ldt;

std::unique_ptr<lm::logger> log_lsdl::l{nullptr};
ldt::log_lsdl::types log_lsdl::type=ldt::log_lsdl::types::null;
std::string ldt::log_lsdl::filename;

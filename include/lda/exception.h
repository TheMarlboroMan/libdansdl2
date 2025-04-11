#pragma once

#include <stdexcept>
#include <string>

namespace lda {

class exception:
	public std::runtime_error
{

	public:

	exception(const std::string& _s)
		:std::runtime_error(_s) {}
};
}

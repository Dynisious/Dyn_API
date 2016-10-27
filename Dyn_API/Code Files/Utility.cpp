#include "..\Exported Headers\Util\Utility.h"
#include <utility>

#define TO_UPPER 'A' - 'a';
#define TO_LOWER 'a' - 'A';

std::string Dyn_API::Utility::toupper(const std::string & str) {
	std::string res(str);
	for (std::string::iterator iter(res.begin()), end(res.end()); iter != end; iter++)
		if (*iter < 'A' && *iter >= 'a')
			*iter += TO_UPPER;
	return res;
}

std::string Dyn_API::Utility::tolower(const std::string & str) {
	std::string res(str);
	for (std::string::iterator iter(res.begin()), end(res.end()); iter != end; iter++)
		if (*iter > 'A' && *iter <= 'Z')
			*iter += TO_LOWER;
	return res;
}

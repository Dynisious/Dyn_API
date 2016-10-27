#pragma once

#include "Utility.h"
#include <string>

namespace Dyn_API {

	namespace Utility {

		/*
		A StringConverter converts instances of a type to a string and vice versa.*/
		template<class _T> struct StringConverter;

		template<> struct StringConverter<void>;

		template<>
		struct StringConverter<std::string> {
			std::string operator()(const std::string &str) const {
				return str;
			}

		};

		template<>
		struct StringConverter<char> {
			char operator()(const std::string &str) const {
				return *str.data();
			}
			std::string operator()(const char val) const {
				return std::string({ val });
			}

		};

		template<>
		struct StringConverter<__int16> {
			__int16 operator()(const std::string &str) const {
				return std::stoi(str);
			}
			std::string operator()(const __int16 val) const {
				return std::to_string(val);
			}

		};

		template<>
		struct StringConverter<__int32> {
			__int32 operator()(const std::string &str) const {
				return std::stoi(str);
			}
			std::string operator()(const __int32 val) const {
				return std::to_string(val);
			}

		};

		template<>
		struct StringConverter<__int64> {
			__int64 operator()(const std::string &str) const {
				return std::stol(str);
			}
			std::string operator()(const __int64 val) const {
				return std::to_string(val);
			}

		};

		template<>
		struct StringConverter<float> {
			float operator()(const std::string &str) const {
				return std::stof(str);
			}
			std::string operator()(const float val) const {
				return std::to_string(val);
			}

		};

		template<>
		struct StringConverter<double> {
			double operator()(const std::string &str) const {
				return std::stod(str);
			}
			std::string operator()(const double val) const {
				return std::to_string(val);
			}

		};

	}

}

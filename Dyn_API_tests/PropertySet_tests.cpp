#include "stdafx.h"
#include "CppUnitTest.h"
#include <Util\PropertySet.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Dyn_API::Utility;

#define STRING "STRING"
#define INT "10"
#define DOUBLE "10.01"

namespace Dyn_API_tests {
	TEST_CLASS(PropertySet_tests) {
public:

	TEST_METHOD(PropertySet_test) {
		PropertySet props({ {STRING,STRING}, {INT,INT}, {DOUBLE,DOUBLE} }, 10);

		Assert::IsTrue(props[STRING] == STRING
					   && props[INT] == INT
					   && props[DOUBLE] == DOUBLE, L"\"[] op\" failed.");

		Assert::IsTrue(props.to_type<std::string>(STRING) == STRING, L"\"to_type string\" failed.");

		Assert::IsTrue(props.to_type<double>(DOUBLE) == 10.01, L"\"to_type double\" failed.");

		Assert::IsTrue(props.to_type<__int32>(INT) == 10, L"\"to_type int\" failed.");

		props.assign<std::string>(STRING, "HELLO WORLD");
		Assert::IsTrue(props.to_type<std::string>(STRING) == "HELLO WORLD", L"\"assign double\" failed.");

		props.assign<double>(DOUBLE, 1.1);
		Assert::IsTrue(props.to_type<double>(DOUBLE) == 1.1, L"\"assign double\" failed.");

		props.assign<__int32>(INT, 20);
		Assert::IsTrue(props.to_type<__int32>(INT) == 20, L"\"assign int\" failed.");

	}

	TEST_METHOD(PropertySet_text_test) {
		PropertySet props(10);

		std::string str("STRING= STRING \n 10 = 10\n10.01=10.01");
		std::stringstream in(str);

		props.loadFromText(in);
		Assert::IsTrue(props[STRING] == STRING
					   && props[INT] == INT
					   && props[DOUBLE] == DOUBLE, L"\"load_text\" failed.");

		str = "temp";
		std::stringstream out;
		props.convToText(out);
		Assert::IsTrue(out.str() == "STRING=STRING\n10=10\n10.01=10.01\n", L"\"to_text\" failed.");

	}

	};
}
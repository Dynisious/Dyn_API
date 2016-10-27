#include "stdafx.h"
#include "CppUnitTest.h"
#include <Util\delegate.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Dyn_API::Utility;

struct foo {

	int bar() {
		return 1;
	}

	int far() {
		return 2;
	}

};

namespace Dyn_API_tests {
	TEST_CLASS(delegate_tests) {
public:

	TEST_METHOD(delegate_equivilance_test) {
		foo f1, f2;
		delegate<int, foo> d1(&f1, &foo::bar), d2(&f2, &foo::far);
		delegate<int, foo, true> b1({ &f1 }, &foo::bar), b2({ &f2 }, &foo::far);

		Assert::IsTrue(d1 == d1, L"\"== op1\" failed.");
		Assert::IsFalse(d1 == d2, L"\"== op2\" failed.");
		Assert::IsTrue(b1 == b1, L"\"== op3\" failed.");
		Assert::IsFalse(b1 == b2, L"\"== op4\" failed.");

		Assert::IsFalse(d1 != d1, L"\"!= op1\" failed.");
		Assert::IsTrue(d1 != d2, L"\"!= op2\" failed.");
		Assert::IsFalse(b1 != b1, L"\"!= op3\" failed.");
		Assert::IsTrue(b1 != b2, L"\"!= op4\" failed.");

	}

	TEST_METHOD(delegate_modification_test) {
		foo f1, f2;
		delegate<int, foo> d1(&f1, &foo::bar), d2(&f2, &foo::far);
		delegate<int, foo, true> b1({ &f1 }, &foo::bar), b2({ &f2 }, &foo::far);

		d1.rebind(f2, &foo::far);
		Assert::IsTrue(d1 == d2, L"\"rebind op1\" failed.");

		b1.rebind({ &f2 }, &foo::far);
		Assert::IsTrue(b1 == b2, L"\"rebind op2\" failed.");

		try {
			d1 + f1;
		} catch (...) {
			Assert::Fail(L"expanding delegate1 failed.");
		}

		try {
			d1 + delegate<void, foo, true>::BoundList({ &f1, &f2 });
		} catch (...) {
			Assert::Fail(L"expanding delegate2 failed.");
		}

		Assert::IsTrue((d1 = (delegate<int, foo>) b2) == d2, L"\"convert op\" failed.");

	}

	TEST_METHOD(delegate_exectution_test) {
		foo f1, f2;
		delegate<int, foo> d1(&f1, &foo::bar), d2(&f2, &foo::far);
		delegate<int, foo, true> b1({ &f1, &f2 }, &foo::bar), b2({ &f2, &f1 }, &foo::far);

		Assert::IsTrue(d1() == foo().bar(), L"\"() op1\" failed.");
		Assert::IsTrue(d2() == foo().far(), L"\"() op2\" failed.");

		try {
			b1();
		} catch (...) {
			Assert::IsFalse(L"\"() op3\" failed.");
		}

		try {
			int i, e;
			b1(delegate<int, foo, true>::ResultList({ &i,&e }));
		} catch (...) {
			Assert::IsFalse(L"\"() op4\" failed.");
		}

	}

	};
}
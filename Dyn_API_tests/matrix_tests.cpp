#include "stdafx.h"
#include "CppUnitTest.h"
#include <Maths\matrix.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Dyn_API::Maths;

namespace Dyn_API_tests {
	TEST_CLASS(matrix_tests) {
public:

	TEST_METHOD(matrix_creation_test) {
		matrix<int, 2, 2> mat1(1),
			mat2(mat1),
			mat3({ 1,2,3,4 }),
			mat4({ 1.1,2.1,3.1,4.1 }),
			mat5(matrix<double, 2, 2>({ 1.1,2.1,3.1,4.1 }));

		Assert::IsTrue(&mat1[0] == mat1.data(), L"\"[] operator\" failed.");

		Assert::IsTrue(mat1[0] == mat1[1] && mat1[1] == mat1[2] && mat1[2] == mat1[3], L"\"init val\" failed.");

		Assert::IsTrue(mat2[0] == mat2[1] && mat2[1] == mat2[2] && mat2[2] == mat2[3], L"\"init orig-1\" failed.");

		Assert::IsTrue(mat3[0] == 1 && mat3[1] == 2 && mat3[2] == 3 && mat3[3] == 4, L"\"init list-1\" failed.");

		Assert::IsTrue(mat4[0] == 1 && mat4[1] == 2 && mat4[2] == 3 && mat4[3] == 4, L"\"init list-2\" failed.");

		Assert::IsTrue(mat5[0] == 1 && mat5[1] == 2 && mat5[2] == 3 && mat5[3] == 4, L"\"init orig-2\" failed.");

		mat1 = mat3;
		Assert::IsTrue(mat1[0] == 1 && mat1[1] == 2 && mat1[2] == 3 && mat1[3] == 4, L"\"assign mat\" failed.");

		mat1.fill(0);
		Assert::IsTrue(mat1[0] == 0 && mat1[1] == 0 && mat1[2] == 0 && mat1[3] == 0, L"\"fill\" failed.");

	}

	TEST_METHOD(matrix_equals_test) {
		matrix<int, 2, 2> mat1(0), mat2(0), mat3(1);

		Assert::IsTrue(mat1 == mat2, L"\"mat == mat\" failed.");

		Assert::IsFalse(mat1 == mat3, L"\"mat == not mat\" failed.");

		Assert::IsFalse(mat1 != mat2, L"\"mat != mat\" failed.");

		Assert::IsTrue(mat1 != mat3, L"\"mat != not mat\" failed.");

	}

	TEST_METHOD(matrix_increment_test) {
		matrix<int, 2, 2> mat1(1), mat2(2);

		Assert::IsTrue((mat2 -= mat1) == matrix<int, 2, 2>(1), L"\"mat -= mat\" failed.");

		Assert::IsTrue((mat2 += mat1) == matrix<int, 2, 2>(2), L"\"mat += mat\" failed.");

		Assert::IsTrue((mat2 - mat1) == mat1, L"\"mat - mat\" failed.");

		Assert::IsTrue((mat2 + -mat1) == mat1, L"\"-mat\" failed.");

		Assert::IsTrue((mat1 + mat1) == mat2, L"\"mat + mat\" failed.");

		Assert::IsTrue(++mat1 == mat2--, L"\"++mat == mat--\" failed.");

		Assert::IsTrue(mat1-- == ++mat2, L"\"mat-- == ++mat\" failed.");

	}

	TEST_METHOD(matrix_linear_test) {
		matrix<int, 2, 2> matI({ 1,0
							   , 0,1 }),
			mat1(1), mat2(2);

		Assert::IsTrue((mat1 *= 2) == mat2, L"\"mat *= int\" failed.");

		Assert::IsTrue((mat1 /= 2) == matrix<int, 2, 2>(1), L"\"mat /= int\" failed.");

		Assert::IsTrue(mat1 * 2 == mat2, L"\"mat * 2\" failed.");

		Assert::IsTrue(mat2 / 2 == mat1, L"\"mat / 1\" failed.");

		Assert::IsTrue(matI.mul(mat2) == mat2, L"\"Identity * mat\" failed.");

	}

	TEST_METHOD(matrix_bitwise_test) {
		matrix<int, 2, 2> mat1(-1), mat0(0);

		Assert::IsTrue(~mat1 == mat0, L"\"mat ~ mat\" failed.");

		Assert::IsTrue((mat1 &= mat1) == mat1, L"\"mat &= mat\" failed.");

		Assert::IsTrue((mat1 & mat0) == mat0, L"\"mat & mat\" failed.");

		Assert::IsTrue((mat1 |= mat0) == mat1, L"\"mat |= mat\" failed.");

		Assert::IsTrue((mat1 | mat1) == mat1, L"\"mat | mat\" failed.");

		Assert::IsTrue((mat1 ^= mat0) == mat1, L"\"mat ^= mat\" failed.");

		Assert::IsTrue((mat1 ^ mat1) == mat0, L"\"mat ^ mat\" failed.");

	}

	TEST_METHOD(matrix_arithmetic_test) {
		matrix<int, 2, 1> mat1({ 1, 0 }), mat2({ 0, 1 }), mat3(1);

		Assert::IsTrue(1 == Matrix_Math::dot<int>(mat1, mat1), L"\"mat dot mat\" failed.");

		Assert::IsTrue(1 == Matrix_Math::magnituid<int>(mat1), L"\"magnituid mat\" failed.");

		Assert::IsTrue(mat1 == Matrix_Math::unit<int>(mat1), L"\"unit mat\" failed.");

		Assert::IsTrue(std::abs(std::acos(-1.0)) / 2 == Matrix_Math::angle(mat1, mat2), L"\"mat angle1 mat\" failed.");

		Assert::IsTrue(Matrix_Math::angle(mat1, mat1) == 0, L"\"mat angle2 mat\" failed.");

		Assert::IsTrue(Matrix_Math::inDir<double>(mat3, mat1) == mat1, L"\"mat inDir1 mat\" failed.");

		Assert::IsTrue(Matrix_Math::inDir<double>(mat3, mat2) == mat2, L"\"mat inDir2 mat\" failed.");

	}

	};
}
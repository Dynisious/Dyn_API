#pragma once

#include "Maths.h"
#include <xhash>
#include <type_traits>
#include <cmath>
#include <iterator>

namespace Dyn_API {

	namespace Maths {

		/*
		A matrix is a fixed size array organised in rows and columns. Performs
		just like mathematical 2D arrays.*/
		template<class _T, size_t _Rows, size_t _Columns>
		class matrix {
			static_assert(_Rows != 0 && _Columns != 0, "A matrix cannot have 0 values.");
			static_assert(!std::is_void<_T>::value
						  && !std::is_const<_T>::value
						  && !std::is_volatile<_T>::value, "A matrix cannot be defined for types with these modifiers.");
		public:
			typedef matrix _MyType;
			typedef _T value_type;
			typedef size_t index_type;
			typedef const value_type const_type;
			typedef value_type * pointer_type;
			typedef const_type * const_pointer_type;
			typedef value_type & reference_type;
			typedef const_type & const_reference_type;
			struct const_iterator : public std::random_access_iterator_tag {
				const_iterator()
					: ptr(nullptr), mat(nullptr) {}
				const_iterator(const const_iterator &orig)
					: const_iterator(orig.ptr, orig.mat) {}
				const_iterator(const_pointer_type _ptr, const _MyType *_mat)
					: ptr(( pointer_type ) _ptr), mat(( _MyType * ) _mat) {
					check();
				}

				/*
				True if this iterator is differencable, meaning it points to an
				actual value.*/
				bool differencable() const {
					return ptr <= (mat->values + Length)
						|| ptr >= mat->values;
				}
				/*
				Limits the iterator to point within [begin, end].*/
				void check() {
					if (ptr > (mat->values + Length))
						ptr = mat->values + Length;
					else if (ptr < mat->values)
						ptr = mat->values;
				}

				const_iterator & operator=(const const_iterator &right) {
					ptr = right.ptr;
					mat = right.mat;
					return *this;
				}
				bool operator==(const const_iterator &right) const {
					return ptr == right.ptr
						&& mat == right.mat;
				}
				bool operator!=(const const_iterator &right) const {
					return ptr != right.ptr
						|| mat != right.mat;
				}
				const_reference_type operator*() const {
					return *ptr;
				}
				const_reference_type operator->() const {
					return operator*();
				}
				const_iterator & operator+=(const __int32 off) {
					ptr += off;
					check();
					return *this;
				}
				const_iterator operator+(const __int32 off) const {
					return const_iterator(*this) += off;
				}
				const_iterator operator++(int) {
					const_iterator res(*this);
					operator+=(1);
					return res;
				}
				const_iterator & operator++() {
					return operator+=(1);
				}
				const_iterator & operator-=(const __int32 off) {
					return operator+=(-off);
				}
				const_iterator operator-(const __int32 off) const {
					return const_iterator(*this) -= off;
				}
				const_iterator operator--(int) {
					const_iterator res(*this);
					operator-=(-1);
					return res;
				}
				const_iterator & operator--() {
					return operator-=(-1);
				}
				const_reference_type operator[](const __int32 off) const {
					return *operator+(off);
				}

				pointer_type ptr;
				_MyType *mat;

			};
			struct iterator : public const_iterator {
				iterator()
					: const_iterator() {}
				iterator(const const_iterator &orig)
					: const_iterator(orig) {}
				iterator(const_pointer_type _ptr, const _MyType *_mat)
					: const_iterator(_ptr, _mat) {}

				reference_type operator*() const {
					return ( reference_type ) const_iterator::operator*();
				}
				reference_type operator->() const {
					return const_iterator::operator*();
				}
				reference_type operator[](const __int32 off) const {
					return *operator+(off);
				}

			};

			static const size_t Rows = _Rows, Columns = _Columns, Length = _Rows * _Columns;

			matrix()
				: matrix(value_type()) {}
			template<class _V>
			matrix(const matrix<_V, _Rows, _Columns> &orig) {
				for (size_t i(0); i < Length; i++)
					values[i] = orig.data()[i];
			}
			matrix(const_reference_type val) {
				fill(val);
			}
			template<class _V>
			matrix(const std::initializer_list<_V> &init) {
				for (size_t i(0), lim(init.size() < Length ? init.size() : Length); i < lim; i++)
					values[i] = init.begin()[i];
			}

			iterator begin() {
				return iterator(values, this);
			}
			iterator end() {
				return iterator(values + Length, this);
			}
			const_iterator begin() const {
				return const_iterator(values, this);
			}
			const_iterator end() const {
				return const_iterator(values + Length, this);
			}
			void fill(const_reference_type val) {
				for (iterator iter(begin()), end(end()); iter != end; iter++)
					*iter = val;
			}
			/*
			Returns the index of the first occourance of "val" between
			("off", Length).*/
			index_type indexOf(const_reference_type val, const index_type off = 0) const {
				for (pointer_type iter(values + off), end(values + Length); iter != end; iter++)
					if (*iter == val)
						return iter - values;
				return Length;
			}
			/*
			Returns the number of occourances of "val" within this matrix.*/
			index_type count(const_reference_type val) const {
				index_type res(0);
				for (index_type off(-1); (off = indexOf(val, off + 1)) != Length; res++);
				return res;
			}
			pointer_type data() {
				return values;
			}
			const_pointer_type data() const {
				return values;
			}
			/*
			Matrix multiplication.*/
			template<class _V, size_t _C>
			matrix<value_type, _Rows, _C> mul(const matrix<_V, _Columns, _C> &right) {
				matrix<value_type, _Rows, _C> res;
				for (index_type row(0), col, i; row < Rows; row++)
					for (col = 0; col < _C; col++)
						for (i = 0; i < Columns; i++)
							res(row, col) += operator()(row, i) * right(i, col);
				return res;
			}

			template<class _V>
			matrix & operator=(const matrix<_V, _Rows, _Columns> &right) {
				for (index_type i(0); i < Length; i++)
					values[i] = right.values[i];
				return *this;
			}
			template<class _V>
			bool operator==(const matrix<_V, _Rows, _Columns> &right) const {
				if (this == ( void* ) &right)
					return true;
				for (index_type i(0); i < Length; i++)
					if (values[i] != right[i])
						return false;
				return true;
			}
			template<class _V>
			bool operator!=(const matrix<_V, _Rows, _Columns> &right) const {
				if (this == ( void* ) &right)
					return false;
				for (index_type i(0); i < Length; i++)
					if (values[i] != right[i])
						return true;
				return false;
			}
			template<class _V>
			matrix & operator+=(const matrix<_V, _Rows, _Columns> &right) {
				for (index_type i(0); i < Length; i++)
					values[i] += right[i];
				return *this;
			}
			template<class _V>
			matrix operator+(const matrix<_V, _Rows, _Columns> &right) const {
				return matrix(*this) += right;
			}
			matrix operator++(int) {
				matrix res(*this);
				++*this;
				return res;
			}
			matrix & operator++() {
				for (pointer_type iter(values), end(values + Length); iter != end; iter++)
					++*iter;
				return *this;
			}
			matrix operator-() const {
				matrix res;
				for (index_type i(0); i < Length; i++)
					res[i] = -values[i];
				return res;
			}
			template<class _V>
			matrix & operator-=(const matrix<_V, _Rows, _Columns> &right) {
				for (index_type i(0); i < Length; i++)
					values[i] -= right[i];
				return *this;
			}
			template<class _V>
			matrix operator-(const matrix<_V, _Rows, _Columns> &right) const {
				return matrix(*this) -= right;
			}
			matrix operator--(int) {
				matrix res(*this);
				--*this;
				return res;
			}
			matrix & operator--() {
				for (pointer_type iter(values), end(values + Length); iter < end; iter++)
					--*iter;
				return *this;
			}
			template<class _V>
			matrix & operator*=(const _V &right) {
				for (pointer_type iter(values), end(values + Length); iter != end; iter++)
					*iter *= right;
				return *this;
			}
			template<class _V>
			matrix operator*(const _V &right) const {
				return matrix(*this) *= right;
			}
			template<class _V>
			matrix & operator/=(const _V &right) {
				for (pointer_type iter(values), end(values + Length); iter != end; iter++)
					*iter /= right;
				return *this;
			}
			template<class _V>
			matrix operator/(const _V &right) const {
				return matrix(*this) /= right;
			}
			template<class _V>
			matrix & operator&=(const matrix<_V, _Rows, _Columns> &right) {
				for (index_type i(0); i < Length; i++)
					values[i] &= right[i];
				return *this;
			}
			template<class _V>
			matrix operator&(const matrix<_V, _Rows, _Columns> &right) const {
				return matrix(*this) &= right;
			}
			template<class _V>
			matrix & operator|=(const matrix<_V, _Rows, _Columns> &right) {
				for (index_type i(0); i < Length; i++)
					values[i] |= right[i];
				return *this;
			}
			template<class _V>
			matrix operator|(const matrix<_V, _Rows, _Columns> &right) const {
				return matrix(*this) |= right;
			}
			template<class _V>
			matrix & operator^=(const matrix<_V, _Rows, _Columns> &right) {
				for (index_type i(0); i < Length; i++)
					values[i] ^= right[i];
				return *this;
			}
			template<class _V>
			matrix operator^(const matrix<_V, _Rows, _Columns> &right) const {
				return matrix(*this) ^= right;
			}
			matrix operator~() const {
				matrix res(*this);
				for (pointer_type iter(res.values), end(res.values + Length); iter != end; iter++)
					*iter = ~*iter;
				return res;
			}
			reference_type operator[](const index_type index) {
				if (index > Length)
					throw std::out_of_range("\"index\" was greater than \"Length\".");
				return values[index];
			}
			const_reference_type operator[](const index_type index) const {
				if (index > Length)
					throw std::out_of_range("\"index\" was greater than \"Length\".");
				return values[index];
			}
			reference_type operator()(const index_type row, const index_type col) {
				return operator[](row * Columns + col);
			}
			const_reference_type operator()(const index_type row, const index_type col) const {
				return operator[](row * Columns + col);
			}

		private:
			value_type values[Length];

		};

		namespace Matrix_Math {

			/*
			Returns the dot product of the two passed matrixes.*/
			template<class _R, class _A, class _B, size_t _Rows, size_t _Columns>
			static _R dot(const matrix<_A, _Rows, _Columns> &left, const matrix<_B, _Rows, _Columns> &right) {
				static_assert(_Rows == 1 || _Columns == 1, "\"dot\" can only be calculated on liner matrixes.");
				_R res(left[0] * right[0]);
				for (size_t i(1); i < left.Length; i++)
					res += left[i] * right[i];
				return res;
			}

			/*
			Returns the magnituid of the passed matrix.*/
			template<class _R, class _V, size_t _Rows, size_t _Columns>
			static _R magnituid(const matrix<_V, _Rows, _Columns> &mat) {
				static_assert(_Rows == 1 || _Columns == 1, "\"magnituid\" can only be calculated on liner matrixes.");
				return std::sqrt(dot<_R>(mat, mat));
			}

			/*
			Returns the unit matrix of the passed matrix.*/
			template<class _R, class _V, size_t _Rows, size_t _Columns>
			static matrix<_R, _Rows, _Columns> unit(const matrix<_V, _Rows, _Columns> &mat) {
				static_assert(_Rows == 1 || _Columns == 1, "\"unit\" can only be calculated on liner matrixes.");
				return mat / magnituid<_R>(mat);
			}

			/*
			Returns the angle between the two passed matrixes in radians.*/
			template<class _A, class _B, size_t _Rows, size_t _Columns>
			static double angle(const matrix<_A, _Rows, _Columns> &left, const matrix<_B, _Rows, _Columns> &right) {
				static_assert(_Rows == 1 || _Columns == 1, "\"angle\" can only be calculated on liner matrixes.");
				return std::acos((
					dot<double>(left, right)
					/ magnituid<double>(left)
				) / magnituid<double>(right)
				);
			}

			/*
			Returns the segment of "mat" pointed in the direction of "dir".*/
			template<class _R, class _A, class _B, size_t _Rows, size_t _Columns>
			static matrix<_R, _Rows, _Columns> inDir(const matrix<_A, _Rows, _Columns> &mat, const matrix<_B, _Rows, _Columns> &dir) {
				static_assert(_Rows == 1 || _Columns == 1, "\"inDir\" can only be calculated on liner matrixes.");
				return (
					dir * dot<_R>(mat, dir)
					/ magnituid<_R>(dir)
					) / magnituid<_R>(dir);
			}

		}

	}

}

namespace std {

	template<class _T, __int32 _Rows, __int32 _Columns>
	struct hash<Dyn_API::Maths::matrix<_T, _Rows, _Columns>> {
	public:
		typedef Dyn_API::Maths::matrix<_T, _Rows, _Columns> Key_t;

		size_t operator()(const Key_t& key, const hash<_T> hasher = hash<_T>()) const {
			size_t res(hasher(key[0]));
			for (size_t i(1); i < key.Length; i++)
				res *= hasher(key[i]);
			return res;
		}

	};

}

#pragma once

#include "Utility.h"
#include <vector>
#include <list>

namespace Dyn_API {

	namespace Utility {

		/*
		A delegate binds a method pointer and 1 or more object pointers together
		and allows the execution of the method to be forwarded to the objects.*/
		template<class _Result_t, class _Obj_t, bool = false, class... _Args_t>
		class delegate {
		public:
			typedef delegate _MyType;
			typedef _Result_t result_t;
			typedef _Obj_t object_t;
			typedef object_t *object_ptr;
			typedef object_t &object_ref;
			typedef const result_t const_result_t;
			typedef const object_t const_object_t;
			typedef const object_ptr const_object_ptr;
			typedef const object_ref const_object_ref;
			typedef result_t(_Obj_t::*func_ptr)(_Args_t...);

			static const bool Chainable = false;

			delegate(const delegate &orig)
				: boundObj(orig.boundObj), boundFunc(orig.boundFunc) {}
			delegate(void* const bndObj, func_ptr const bndFnc)
				: boundObj(( object_ptr ) bndObj), boundFunc(bndFnc) {}

			void rebind(object_ref const bndObj, func_ptr const bndFnc) {
				boundObj = &bndObj;
				boundFunc = bndFnc;
			}

			object_ptr boundObj;
			func_ptr boundFunc;

			delegate operator=(const delegate right) {
				boundObj = right.boundObj;
				boundFunc = right.boundFunc;
				return *this;
			}
			bool operator==(const delegate right) const {
				return boundObj == right.boundObj
					&& boundFunc == right.boundFunc;
			}
			bool operator!=(const delegate right) const {
				return boundObj != right.boundObj
					|| boundFunc != right.boundFunc;
			}
			template<class _Param>
			delegate<result_t, object_t, true, _Args_t...> operator+(_Param &right) const {
				return delegate<result_t, object_t, true, _Args_t...>(*this) += right;
			}
			/*
			Executes the bound method on the bound pointer and returns the result.*/
			result_t operator()(_Args_t... args) {
				return (boundObj->*boundFunc)(args...);
			}

		};

		template<class _Result_t, class _Obj_t, class... _Args_t>
		class delegate<_Result_t, _Obj_t, true, _Args_t...> {
		public:
			typedef delegate _MyType;
			typedef _Result_t result_t;
			typedef _Obj_t object_t;
			typedef result_t *result_ptr;
			typedef object_t *object_ptr;
			typedef object_t &object_ref;
			typedef const result_t const_result_t;
			typedef const object_t const_object_t;
			typedef const object_ptr const_object_ptr;
			typedef const object_ref const_object_ref;
			typedef result_t(_Obj_t::*func_ptr)(_Args_t...);
			typedef std::list<object_ptr> BoundList;
			typedef std::list<result_ptr> ResultList;

			static const bool Chainable = true;

			delegate(const delegate &orig)
				: boundObjs(orig.boundObjs), boundFunc(orig.boundFunc) {}
			delegate(const delegate<result_t, object_t, false, _Args_t...> &orig)
				: boundObjs(), boundFunc(orig.boundFunc) {
				boundObjs.push_front(orig.boundObj);
			}
			delegate(void* const bndObjs, const size_t bndObjs_c, func_ptr const bndFnc)
				: boundObjs(bndObjs, bndObjs_c), boundFunc(bndFnc) {}
			delegate(const std::initializer_list<void*> &bndObjs, func_ptr const bndFnc)
				: boundObjs(), boundFunc(bndFnc) {
				for (std::initializer_list<void*>::const_iterator iter(bndObjs.begin()), end(bndObjs.end()); iter != end; iter++)
					boundObjs.push_back(( object_ptr ) *iter);
			}

			void rebind(object_ptr const bndObjs, const size_t bndObjs_c, func_ptr const bndFnc) {
				boundObjs = BoundList(bndObjs, bndObjs_c);
				boundFunc = bndFnc;
			}
			void rebind(const std::initializer_list<object_ptr> &bndObjs, func_ptr const bndFnc) {
				boundObjs = bndObjs;
				boundFunc = bndFnc;
			}

			BoundList boundObjs;
			func_ptr boundFunc;

			delegate operator=(const delegate right) {
				boundObjs = right.boundObjs;
				boundFunc = right.boundFunc;
				return *this;
			}
			bool operator==(const delegate right) const {
				return (this == &right)
					|| (boundObjs == right.boundObjs
						&& boundFunc == right.boundFunc);
			}
			bool operator!=(const delegate right) const {
				return (this == &right)
					|| (boundObjs != right.boundObjs
						|| boundFunc != right.boundFunc);
			}
			delegate & operator+=(const std::initializer_list<object_t> &right) {
				boundObjs.push_back(right.begin(), right.end());
				return *this;
			}
			delegate & operator+=(const BoundList &right) {
				boundObjs.insert(boundObjs.end(), right.begin(), right.end());
				return *this;
			}
			delegate & operator+=(const_object_ref right) {
				boundObjs.push_back(&right);
				return *this;
			}
			delegate operator+(const std::initializer_list<object_t> &right) {
				return delegate(*this) += right;
			}
			delegate operator+(const_object_ref right) {
				return delegate(*this) += right;
			}
			delegate & operator-=(const std::initializer_list<object_t> &right) {
				boundObjs.erase(right.begin(), right.end());
				if (boundObjs.empty())
					throw std::runtime_error("A delegate must point to something.");
				return *this;
			}
			delegate & operator-=(const BoundList &right) {
				boundObjs.erase(right.begin(), right.end());
				if (boundObjs.empty())
					throw std::runtime_error("A delegate must point to something.");
				return *this;
			}
			delegate & operator-=(const_object_ref right) {
				boundObjs.erase(right);
				if (boundObjs.empty())
					throw std::runtime_error("A delegate must point to something.");
				return *this;
			}
			delegate operator-(const std::initializer_list<object_t> &right) {
				return delegate(*this) -= right;
			}
			delegate operator-(const_object_ref right) {
				return delegate(*this) -= right;
			}
			/*
			Executes the bound method on all bound pointers.*/
			void operator()(_Args_t... args) {
				for (BoundList::iterator iter(boundObjs.begin()), end(boundObjs.end()); iter != end; iter++)
					((*iter)->*boundFunc)(args...);
			}
			/*
			Executes the bound method on all bound pointers and returns the results.*/
			void operator()(ResultList &results, _Args_t... args) {
				if (results.size() < boundObjs.size())
					throw std::invalid_argument("The passed list for results is smaller than the number of objects being delegated to.");
				ResultList::iterator res(results.begin());
				for (BoundList::iterator iter(boundObjs.begin()), end(boundObjs.end()); iter != end; iter++, res++)
					**res = ((*iter)->*boundFunc)(args...);
			}

			operator delegate<result_t, object_t, false, _Args_t...>() {
				return delegate<result_t, object_t, false, _Args_t...>(*boundObjs.begin(), boundFunc);
			}

		};

	}

}

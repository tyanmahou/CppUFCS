#pragma once
#include<type_traits>
#include<tuple>

/*
* Unified Function Call Syntax on C++
*/
namespace ufcs
{
	namespace detail
	{
		template<class AlwaysVoid, template<class...>class Op, class ...Args>
		struct is_detected_impl :std::false_type
		{};
		template<template<class...>class Op, class ...Args>
		struct is_detected_impl<std::void_t<Op<Args...>>, Op, Args...> :std::true_type
		{};

		template<class Func, class... TupleArgs, class... Args>
		auto param_expand_impl(Func func, [[maybe_unused]] const std::tuple<TupleArgs...>& tuple, Args&&... args)->
			std::enable_if_t<sizeof...(Args) == sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...>>
		{
			return func(args...);
		}
		template<class Func, class... TupleArgs, class... Args>
		auto param_expand_impl(Func func, [[maybe_unused]] const std::tuple<TupleArgs...>& tuple, Args&&... args)->
			std::enable_if_t < sizeof...(Args) < sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...> >
		{
			return param_expand_impl(func, tuple, args..., std::get<sizeof...(Args)>(tuple));
		}
	}
	template<template<class...>class Op, class ...Args>
	using is_detected = detail::is_detected_impl<void, Op, Args...>;

	template<class Func, class Arg>
	decltype(auto) param_expand(Func func, Arg&& arg)
	{
		return detail::param_expand_impl(func, std::forward<Arg>(arg));
	}

	template<class Base, class... Args>
	struct TupleParam {
		std::tuple<Args...> m_param;
	};
}

///<summary>
///method chain
///<summary>
template<class T, class Base, class... Args>
decltype(auto) operator | (T&& v, ufcs::TupleParam<Base, Args...> tuple)
{
	auto f = [&](auto&&... args) {
		return Base::free(v, std::forward<decltype(args)>(args)...);
	};
	return ufcs::param_expand(f, tuple.m_param);
}

#define USE_UFCS(funcName,...) \
constexpr struct funcName##_op\
{\
	template<class T,class... Args> \
	using check = decltype(std::declval<T&>().funcName(std::declval<Args&>()...)); \
	template<class T,class... Args>\
	static decltype(auto) free(T&& v,Args&&... args)\
	{\
		if constexpr(ufcs::is_detected<check, T,Args...>::value)\
		{ \
			return v.funcName(std::forward<Args>(args)...);\
		}\
		else {\
			return __VA_ARGS__ funcName(std::forward<T>(v),std::forward<Args>(args)...);\
		}\
	}\
	template<class... Args>\
	ufcs::TupleParam<funcName##_op,Args...> operator()(Args&&... args)const\
	{\
		return { std::make_tuple(std::forward<Args>(args)...) };\
	}\
} funcName##_

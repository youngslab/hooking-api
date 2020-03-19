

#pragma once

#include <dlfcn.h>
#include <string>
#include <sstream>
#include <cstring>
// invoke
#include <chrono>
#include <tuple>
#include <functional> // std::invoke

#define _str(a) #a
#define xstr(a) _str(a)

#define hook_load(x) hook::load<typeof(x)*>(xstr(x))
#define hook_format(f, ...) hook::format(xstr(f), ## __VA_ARGS__)
#define hook_invoke(...) std::invoke(__VA_ARGS__)
#define hook_invoke_d(...) hook::invoke_d(__VA_ARGS__)

namespace hook {
template<typename Func>
inline auto load(std::string const &x)-> Func{
  auto ptr = dlsym(RTLD_NEXT, x.data());
  return reinterpret_cast<Func>(ptr);
}

namespace detail {
template<typename Stream, typename... Args>
auto format(Stream &stream, std::string const &func, Args... args){
	constexpr auto sep= ", ";
  stream << func << "(";
  ((stream << args << sep),...);
  if(sizeof...(args) > 0){
    stream.seekp(-strlen(sep), std::ios_base::end);
  }
  stream << ")";
}
} // namespace detail

template <typename... Args>
auto format(std::string const &func, Args&&... args) -> std::string {
  std::stringstream ss;
  detail::format(ss, func, std::forward<Args>(args)...);
  return ss.str();
}


template<typename Func , typename... Args>
auto invoke_d(Func const &f, Args&&... args) -> 
  std::pair<decltype(f(args...)), std::chrono::milliseconds> {
	auto start = std::chrono::system_clock::now();
  auto res = std::invoke(f, std::forward<Args>(args)...);
  auto dur = std::chrono::system_clock::now() - start;
  return {res, std::chrono::duration_cast<std::chrono::milliseconds>(dur)}; 
}

	
} // namesapce hook



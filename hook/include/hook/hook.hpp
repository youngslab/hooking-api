

#pragma once

#include <dlfcn.h>
#include <string>
#include <sstream>
#include <cstring>

#define _str(a) #a
#define xstr(a) _str(a)

#define hook_load(x) hook::load<typeof(x)*>(xstr(x))
#define hook_format(f, ...) hook::format(xstr(f), ## __VA_ARGS__)

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

} // namesapce hook



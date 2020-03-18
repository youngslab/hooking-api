

#pragma once

#include <dlfcn.h>
#include <string>

#define _str(a) #a
#define xstr(a) _str(a)

#define hook_load(x) hook::load<typeof(x)*>(xstr(x))

namespace hook {
template<typename Func>
auto load(std::string const &x)-> Func{
  auto ptr = dlsym(RTLD_NEXT, x.data());
  return reinterpret_cast<Func>(ptr);
}
} // namesapce hook




#include <version.h>
#include <iostream>
#include <hook/hook.hpp>

std::string xxx(){}

std::string version() {
  // sstatic auto f = hook::load<typeof(version)*>("version");
  static auto f = hook_load(version);
  std::cout << "["<< hook_format(version) << "]";
  return hook_invoke(f) + " hacked!";
}

int add(int x, int y) {
  static auto f = hook_load(add);
  auto [res, dur] = hook_invoke_d(f,x,y);
  std::cout << "[" << dur.count() <<"ms][" << hook_format(add,x,y) << "]";
  return res;
}

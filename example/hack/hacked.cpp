
#include <version.h>
#include <iostream>
#include <hook/hook.hpp>

std::string version() {
	// sstatic auto f = hook::load<typeof(version)*>("version");
  static auto f = hook_load(version);
	std::cout << "["<< hook_format(version) << "]";
	return f() + " hacked!";
}

int add(int x, int y) {
  static auto f = hook_load(add);
  std::cout << "["<< hook_format(add, x, y) << "]";
  return f(x, y);
}

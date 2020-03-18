
#include <version.h>
#include <hook/hook.hpp>

std::string version() {
	// sstatic auto f = hook::load<typeof(version)*>("version");
  static auto f = hook_load(version);
	return f() + " hacked!";
}

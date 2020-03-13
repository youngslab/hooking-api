
#include <version.h>

#include <functional>
#include <dlfcn.h>

std::string version() {
	using signiture = std::string();

	std::string (*func)(); 
	auto ptr = dlsym(RTLD_NEXT, "version");
	if(!ptr) {
		return "no function found";
	}

	func = reinterpret_cast<std::string(*)()>(ptr);
	if(!func) {
	  return "failed to convert";
	}

	return "successfully hacked with " + func();
}

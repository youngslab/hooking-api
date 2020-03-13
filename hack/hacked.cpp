
#include <version.h>

#include <functional>
#include <dlfcn.h>

#define xstr(a) str(a)
#define str(a) #a
#define apply(f, args...) reinterpret_cast<typeof(x)*>(dlsym(RTLD_NEXT, str(x)))(args...)


template<typename T, typename... Args>
auto log(std::string const&api, Args... args){
▒░// log with time.. process
}

extern "C" EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface) {
▒░// can i return ?
  return apply(eglSwapBuffers, display, surface);
}



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

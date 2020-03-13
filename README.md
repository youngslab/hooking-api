


# API Hooking Exercise

Application이 어떤 함수를 호출 하고자 할때 LD_PRELOAD를 통해 Loading된 library내에서 함수의 symbol을 검색하여 가장 적합한 함수를 사용한다. 이를 이용하여 App은 삽입된 library의 함수를 사용하고, hooking library내에서는 필요하다면 기존 함수의 symbol을 찾아내어 사용한다. 이 예제에서는 `version`이라는 함수를 hooking하여 새로운 string으로 변환하며 결과는 아래와 같다. 

```
# At build directory
$./bin/app  
Version: v1.0.0 !

$ LD_PRELOAD=./lib/libhack.so ./app/app
Version: successfully hacked with v1.0.0 !
```


## Hooking function. 
치환은 같은 signiture함수를 갖는 library를 제공해 주는 것만으로 충분하지만, 원래함수를 호출하기 위해서는 자신의 함수와 똑같은 symbol을 loading 해 주어야 한다. 이를 위해 `dlsym`함수와 `RTLD_NEXT` enum 값을 사용하여 symbol의 우선 순위가 2번째인 함수를 찾는다. 

```cpp
#include <dlfcn.h>
// get a pointer to the "next" symbol.
std::string version() {
    auto ptr = dlsym(RTLD_NEXT, "version");
    auto func = reinterpret_cast<std::string*()>(ptr);
    return func();
}
```

## C++ 에서 사용 
C++를 사용하게 되면 함수 이름이 mangling되기 때문에 extern "C"를 통해 Mangling 안되도록 한다. 
```
extern "C" std::string version();
```

## References 
- [Function Hooking Part I: Hooking Shared Library Function Calls in Linux](https://blog.netspi.com/function-hooking-part-i-hooking-shared-library-function-calls-in-linux/)


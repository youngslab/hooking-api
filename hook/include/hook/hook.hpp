

#pragma once

#include <dlfcn.h>
#include <chrono>
#include <cstring>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>

#define _str(a) #a
#define xstr(a) _str(a)

// helper interface function-like macro

/**
 * @brief  Load the original function
 */
#define hook_load(x) hook::load<typeof(x) *>(xstr(x))

/**
 * @brief Formatting a function with its argmuemnts.
 */
#define hook_format(f, ...) hook::format(xstr(f), ##__VA_ARGS__)

/**
 * @brief Invoke a function with its argument. It just wrap this std function
 * for its consistent interface
 */
#define hook_invoke(...) std::invoke(__VA_ARGS__)

/**
 * @brief Invoke a function and return its result with the time how much it
 * takes.
 */
#define hook_invoke_d(...) hook::invoke_d(__VA_ARGS__)

namespace hook {
template <typename Func>
inline auto load(std::string const &x) -> Func {
  auto ptr = dlsym(RTLD_NEXT, x.data());
  if (!ptr) {
    throw std::runtime_error(x + " is not found.");
  }
  return reinterpret_cast<Func>(ptr);
}

namespace detail {
template <typename Stream, typename... Args>
auto format(Stream &stream, std::string const &func, Args... args) {
  constexpr auto sep = ", ";
  stream << func << "(";
  ((stream << args << sep), ...);
  if (sizeof...(args) > 0) {
    stream.seekp(-strlen(sep), std::ios_base::end);
  }
  stream << ")";
}
}  // namespace detail

template <typename Func, typename... Args>
struct has_result {
  constexpr static bool value =
      !std::is_same_v<std::invoke_result_t<Func, Args...>, void>;
};

template <typename Func, typename... Args>
constexpr static bool has_result_v = has_result<Func, Args...>::value;

template <typename... Args>
auto format(std::string const &func, Args &&... args) -> std::string {
  std::stringstream ss;
  detail::format(ss, func, std::forward<Args>(args)...);
  return ss.str();
}

template <typename Func, typename... Args,
	  typename Enabled = std::enable_if_t<has_result_v<Func, Args...>>>
auto invoke_d(Func const &f, Args &&... args)
    -> std::pair<decltype(f(args...)), std::chrono::nanoseconds> {
  auto start = std::chrono::high_resolution_clock::now();
  auto res = std::invoke(f, std::forward<Args>(args)...);
  auto dur = std::chrono::high_resolution_clock::now() - start;
  return {res, std::chrono::duration_cast<std::chrono::nanoseconds>(dur)};
}

template <typename Func, typename... Args,
	  typename Enabled = std::enable_if_t<!has_result_v<Func, Args...>>>
auto invoke_d(Func const &f, Args &&... args) -> std::chrono::nanoseconds {
  auto start = std::chrono::high_resolution_clock::now();
  std::invoke(f, std::forward<Args>(args)...);
  auto dur = std::chrono::high_resolution_clock::now() - start;
  return std::chrono::duration_cast<std::chrono::nanoseconds>(dur);
}

}  // namesapce hook


#ifndef LAMINAR_LOG_H_
#define LAMINAR_LOG_H_

#include <kj/debug.h>
#include <utility>

namespace _ {
    constexpr const char* static_basename_impl(const char* b, const char* t) {
        return *t == '\0' ? b : static_basename_impl(*t == '/' ? t+1 : b, t+1);
    }
    constexpr const char* static_basename(const char* p) {
        return static_basename_impl(p, p);
    }
    constexpr int static_strlen(const char* s) {
        return *s == '\0' ? 0 : static_strlen(s + 1) + 1;
    }
    template<int N, int...I>
    static constexpr decltype(auto) static_alloc_str_impl(const char* str, std::integer_sequence<int, I...>) {
        typedef struct {char buf[N+1];} static_null_terminated;
        return (static_null_terminated) {str[I]..., '\0'};
    }
    template<int N>
    static constexpr decltype(auto) static_alloc_str(const char* str) {
        return static_alloc_str_impl<N>(str, std::make_integer_sequence<int, N>());
    }
}
#define __FILE_BASE__ (::_::static_alloc_str<::_::static_strlen(::_::static_basename(__FILE__))>\
                        (::_::static_basename(__FILE__)).buf)

#define LLOG(severity, ...) \
  if (!::kj::_::Debug::shouldLog(::kj::_::Debug::Severity::severity)) {} else \
    ::kj::_::Debug::log(__FILE_BASE__, __LINE__, \
      ::kj::_::Debug::Severity::severity, #__VA_ARGS__, __VA_ARGS__)

#define LASSERT(cond, ...) \
  if (KJ_LIKELY(cond)) {} else \
    for (::kj::_::Debug::Fault f(__FILE_BASE__, __LINE__, \
      ::kj::Exception::Type::FAILED, #cond, #__VA_ARGS__, ##__VA_ARGS__);; f.fatal())

#define LSYSCALL(call, ...) \
  if (auto _kjSyscallResult = ::kj::_::Debug::syscall([&](){return (call);}, false)) {} else \
    for (::kj::_::Debug::Fault f(__FILE_BASE__, __LINE__, \
      _kjSyscallResult.getErrorNumber(), #call, #__VA_ARGS__, ##__VA_ARGS__);; f.fatal())

#endif // LAMINAR_LOG_H_


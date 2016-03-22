#ifndef _MALIKANIA_COMMON_H_
#define _MALIKANIA_COMMON_H_

#if defined(_WIN32)
#  if defined(MALIKANIA_COMMON_BUILD)
#    define MALIKANIA_COMMON_EXPORT __declspec(dllexport)
#  else
#    define MALIKANIA_COMMON_EXPORT __declspec(dllimport)
#  endif
#else
#  define MALIKANIA_COMMON_EXPORT
#endif

#endif // !_MALIKANIA_COMMON_H_

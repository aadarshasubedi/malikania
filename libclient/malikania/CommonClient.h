#ifndef _MALIKANIA_COMMON_CLIENT_H_
#define _MALIKANIA_COMMON_CLIENT_H_

#if defined(_WIN32)
#  if defined(MALIKANIA_CLIENT_BUILD)
#    define MALIKANIA_CLIENT_EXPORT __declspec(dllexport)
#  else
#    define MALIKANIA_CLIENT_EXPORT __declspec(dllimport)
#  endif
#else
#  define MALIKANIA_CLIENT_EXPORT
#endif

#endif // !_MALIKANIA_COMMON_CLIENT_H_

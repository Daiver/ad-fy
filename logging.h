#ifndef __LOGGING_H__
#define __LOGGING_H__

#define devel
#ifdef devel
#define LOG(where, what) printf("\n=====[%s] %s\n", where, what);
#else
#define LOG(where, what)
#endif

#endif

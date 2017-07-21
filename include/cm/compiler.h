#ifndef CM_COMPILER_H
#define CM_COMPILER_H  1

#ifdef __GNUC__
#define sentinel  __attribute__ ((sentinel))
#else
#define sentinel
#endif

#endif  /* CM_COMPILER_H */

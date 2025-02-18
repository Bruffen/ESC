/*
 * Generated by dtrace(1M).
 */

#ifndef	_PROBES_H
#define	_PROBES_H

#include <unistd.h>
#include <inttypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define _DTRACE_VERSION 1

#if _DTRACE_VERSION

#define	DT_DISTTRANS_ITEND() \
	__dtrace_dt___disttrans__itend()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_ITEND_ENABLED() \
	__dtraceenabled_dt___disttrans__itend()
#else
#define	DT_DISTTRANS_ITEND_ENABLED() \
	__dtraceenabled_dt___disttrans__itend(0)
#endif
#define	DT_DISTTRANS_ITSTART() \
	__dtrace_dt___disttrans__itstart()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_ITSTART_ENABLED() \
	__dtraceenabled_dt___disttrans__itstart()
#else
#define	DT_DISTTRANS_ITSTART_ENABLED() \
	__dtraceenabled_dt___disttrans__itstart(0)
#endif
#define	DT_DISTTRANS_LOADEND() \
	__dtrace_dt___disttrans__loadend()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_LOADEND_ENABLED() \
	__dtraceenabled_dt___disttrans__loadend()
#else
#define	DT_DISTTRANS_LOADEND_ENABLED() \
	__dtraceenabled_dt___disttrans__loadend(0)
#endif
#define	DT_DISTTRANS_LOADSTART() \
	__dtrace_dt___disttrans__loadstart()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_LOADSTART_ENABLED() \
	__dtraceenabled_dt___disttrans__loadstart()
#else
#define	DT_DISTTRANS_LOADSTART_ENABLED() \
	__dtraceenabled_dt___disttrans__loadstart(0)
#endif
#define	DT_DISTTRANS_PROGRAMEND() \
	__dtrace_dt___disttrans__programend()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_PROGRAMEND_ENABLED() \
	__dtraceenabled_dt___disttrans__programend()
#else
#define	DT_DISTTRANS_PROGRAMEND_ENABLED() \
	__dtraceenabled_dt___disttrans__programend(0)
#endif
#define	DT_DISTTRANS_PROGRAMSTART() \
	__dtrace_dt___disttrans__programstart()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_PROGRAMSTART_ENABLED() \
	__dtraceenabled_dt___disttrans__programstart()
#else
#define	DT_DISTTRANS_PROGRAMSTART_ENABLED() \
	__dtraceenabled_dt___disttrans__programstart(0)
#endif
#define	DT_DISTTRANS_SAVEEND() \
	__dtrace_dt___disttrans__saveend()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_SAVEEND_ENABLED() \
	__dtraceenabled_dt___disttrans__saveend()
#else
#define	DT_DISTTRANS_SAVEEND_ENABLED() \
	__dtraceenabled_dt___disttrans__saveend(0)
#endif
#define	DT_DISTTRANS_SAVESTART() \
	__dtrace_dt___disttrans__savestart()
#if !defined(__aarch64__) && !defined(__sparc)
#define	DT_DISTTRANS_SAVESTART_ENABLED() \
	__dtraceenabled_dt___disttrans__savestart()
#else
#define	DT_DISTTRANS_SAVESTART_ENABLED() \
	__dtraceenabled_dt___disttrans__savestart(0)
#endif


extern void __dtrace_dt___disttrans__itend(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__itend(void);
#else
extern int __dtraceenabled_dt___disttrans__itend(long);
#endif
extern void __dtrace_dt___disttrans__itstart(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__itstart(void);
#else
extern int __dtraceenabled_dt___disttrans__itstart(long);
#endif
extern void __dtrace_dt___disttrans__loadend(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__loadend(void);
#else
extern int __dtraceenabled_dt___disttrans__loadend(long);
#endif
extern void __dtrace_dt___disttrans__loadstart(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__loadstart(void);
#else
extern int __dtraceenabled_dt___disttrans__loadstart(long);
#endif
extern void __dtrace_dt___disttrans__programend(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__programend(void);
#else
extern int __dtraceenabled_dt___disttrans__programend(long);
#endif
extern void __dtrace_dt___disttrans__programstart(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__programstart(void);
#else
extern int __dtraceenabled_dt___disttrans__programstart(long);
#endif
extern void __dtrace_dt___disttrans__saveend(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__saveend(void);
#else
extern int __dtraceenabled_dt___disttrans__saveend(long);
#endif
extern void __dtrace_dt___disttrans__savestart(void);
#if !defined(__aarch64__) && !defined(__sparc)
extern int __dtraceenabled_dt___disttrans__savestart(void);
#else
extern int __dtraceenabled_dt___disttrans__savestart(long);
#endif

#else

#define	DT_DISTTRANS_ITEND()
#define	DT_DISTTRANS_ITEND_ENABLED() (0)
#define	DT_DISTTRANS_ITSTART()
#define	DT_DISTTRANS_ITSTART_ENABLED() (0)
#define	DT_DISTTRANS_LOADEND()
#define	DT_DISTTRANS_LOADEND_ENABLED() (0)
#define	DT_DISTTRANS_LOADSTART()
#define	DT_DISTTRANS_LOADSTART_ENABLED() (0)
#define	DT_DISTTRANS_PROGRAMEND()
#define	DT_DISTTRANS_PROGRAMEND_ENABLED() (0)
#define	DT_DISTTRANS_PROGRAMSTART()
#define	DT_DISTTRANS_PROGRAMSTART_ENABLED() (0)
#define	DT_DISTTRANS_SAVEEND()
#define	DT_DISTTRANS_SAVEEND_ENABLED() (0)
#define	DT_DISTTRANS_SAVESTART()
#define	DT_DISTTRANS_SAVESTART_ENABLED() (0)

#endif


#ifdef	__cplusplus
}
#endif

#endif	/* _PROBES_H */

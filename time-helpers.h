#ifndef TIME_HELPERS_H
#define TIME_HELPERS_H

/*
 * comment or use #undef to disable profiling
 * where the macros are used.
 */
#define PROFILE_ENABLED

#include <time.h>
#include <stdint.h>

/* profiling macros */
#ifdef PROFILE_ENABLED

#define NANO 1000000000

#define CTX(brace) brace

/* context macros can be used multiple times within the same scope */
#define PROFILE_CTX_START CTX({) struct timespec __prof_start_time__ = now();
#define PROFILE_CTX_END elapsed(&__prof_start_time__); CTX(})
#define PEOFILE_CTX_END_PRNT(stream) stream << elapsed(&__prof_start_time__); CTX(})

/* those macros will cause compilation errors when used within the same scope */
#define PROFILE_START struct timespec __prof_start_time__ = now();
#define PROFILE_END elapsed(&__prof_start_time__)
#define PROFILE_END_SC elapsed(&__prof_start_time__);
#define PEOFILE_END_PRNT(stream) stream << elapsed(&__prof_start_time__);

#else

#define PROFILE_START
#define PROFILE_END 0
#define PROFILE_END_SC 0;
#define PEOFILE_END_PRNT(stream) ;

#define PROFILE_CTX_START
#define PROFILE_CTX_END ;
#define PEOFILE_CTX_END_PRNT(stream) ;

#endif

/* functions delcarations */
inline struct timespec now () __attribute__((always_inline));
inline uint64_t timediff (const struct timespec *t1,
                          const struct timespec *t2) __attribute__((always_inline));
inline uint64_t elapsed (const struct timespec *t1) __attribute__((always_inline));
inline void get_time (struct timespec *time) __attribute__((always_inline));

/* functions definitions */
inline struct timespec now ()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t;
}

inline void get_time (struct timespec *time)
{
    clock_gettime(CLOCK_MONOTONIC, time);
}

inline uint64_t timediff (const struct timespec *t1,
                          const struct timespec *t2)
{
    return (NANO * (t2->tv_sec - t1->tv_sec)) +
            (t2->tv_nsec - t1->tv_nsec);
}

inline uint64_t elapsed (const timespec *t1)
{
    struct timespec t2;
    get_time(&t2);
    return (NANO * (t2.tv_sec - t1->tv_sec)) +
            (t2.tv_nsec - t1->tv_nsec);
}

inline uint64_t nanosecs (uint64_t ns)
{
    return ns;
}

inline uint64_t microsecs (uint64_t ns)
{
    return ns / 1000;
}

inline uint64_t millisecs (uint64_t ns)
{
    return ns / 1000000;
}

#endif // TIME_HELPERS_H

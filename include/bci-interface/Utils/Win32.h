#ifdef WIN32

#include <stdint.h>
#include <iostream>

# if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
# else
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
# endif

void gettimeofday(struct timeval *tv, void *);
#endif
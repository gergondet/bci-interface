#ifdef WIN32

#include <stdint.h>
#include <iostream>
#include <Windows.h>

# if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
# else
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
# endif
namespace bciinterface
{
void gettimeofday(struct timeval *tv, void *);

void usleep(int t);
}
#endif
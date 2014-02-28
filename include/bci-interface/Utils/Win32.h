#ifdef WIN32

#include <bci-interface/BCIAPI.h>

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
BCIINTERFACE_API void gettimeofday(struct timeval *tv, void *);

BCIINTERFACE_API void usleep(int t);
}
#endif
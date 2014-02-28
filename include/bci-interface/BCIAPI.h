#ifndef BCIINTERFACE_API
#ifdef WIN32
#define BCIINTERFACE_API __declspec(dllexport)
#else
#define BCIINTERFACE_API
#endif
#endif
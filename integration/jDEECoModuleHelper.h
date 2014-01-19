#ifndef _JDEECMODULEOHELPER_H_
#define _JDEECMODULEOHELPER_H_

#ifdef __cplusplus
extern "C" {
#endif
#ifdef MAKEDLL
#  define DLLEXPORT __declspec(dllexport)
#else
#  define DLLEXPORT __declspec(dllimport)
#endif

DLLEXPORT void packetReceived(unsigned char * data, void * module);
DLLEXPORT void registerModule(void * module);

#ifdef __cplusplus
}
#endif
#endif

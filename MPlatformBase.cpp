#include "MPlatformBase.h"


#if (MPLATFORM == MPLATFORM_WINDOWS)
#   pragma comment(lib, "ws2_32.lib")
#   pragma comment(lib,"mswsock.lib") 
#	pragma comment(lib,"Shlwapi.lib") 
#endif
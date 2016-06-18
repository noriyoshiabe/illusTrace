#pragma once

#include <cstdio>

#ifdef DEBUG
#define __Trace__ __IsTrace__ ? printf("-- %s - %d - %s\n", __FILE__, __LINE__, __FUNCTION__) : 0;
extern bool __IsTrace__;
#else
#define __Trace__
#endif

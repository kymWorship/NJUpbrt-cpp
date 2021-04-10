#include "settings.h"

#include <memory>
using std::shared_ptr;
using std::make_shared; 

#include <cassert>
#if ISDEBUGGING
#define DASSERT(a, b) assert(a&&b)
#else
#define DASSERT(a, b) 
#endif

#define Fmax(a, b, c) fmax(fmax(a, b), c)
#define Fmin(a, b, c) fmin(fmin(a, b), c)

#include <float.h>
#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif
 
#ifndef MINFLOAT
#define MINFLOAT FLT_MIN
#endif
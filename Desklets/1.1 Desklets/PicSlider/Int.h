#pragma once

#include <math.h>

// I may be missing something, but the runtime lib seems to lack
// a function to round of a REAL (float, double) to an int.
inline int Int(REAL r)	{ return (int) floor(r + 0.5f); }

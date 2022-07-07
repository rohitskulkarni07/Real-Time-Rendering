#include <stdio.h>
#include <stdlib.h>

#include "vmath.h"

using namespace vmath;

struct CTM {
	mat4 data;
	struct CTM* link;
};

void pushMatrix(mat4 data);
void popMatrix(mat4 *data);
void deleteStack();

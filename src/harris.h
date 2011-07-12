#ifndef HARRIS_H
#define HARRIS_H

#include "array.h"

void gradients(image* img, array * dx, array * dy);
array * harris(array * img);

#endif

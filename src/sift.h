#ifndef SIFT_H
#define SIFT_H

#include "array.h"

typedef struct {
	int x;
	int y;
	double scale;
	double ang;
	double strength;
} frame;

typedef double descriptor[128];

void frames(frame *fr);
void descriptors(array *img, frames *fr, descriptor *desc);

#endif

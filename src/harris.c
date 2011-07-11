/*
 * Multiscale Harris operator
 */ 

#include <maths.h>

#include "array.h"
#include "utilities.h"

// utilities
void convolution1d(float* a, float* kernel);
void convolution(image img, array kernel);

void smooth(image img, float sig);

void gradients(image img) {
	array* kernel;
	construct(kernel, 3, 3);
	kernel->px = {
		-1.0, 0.0, 1.0,
		-2.0, 0.0, 2.0,
		-1.0, 0.0, 1.0
	};

	convolution(img, kernel, dx);
	transpose(kernel);
	convolution(img, kernel, dy);

	destruct(kernel);
}


// Harris operator
void harris(pixels img, float thres) {
	array dx, dy, dx2, dy2, dxy, strengths, corners;
	int i,j,n;

	smooth(img);

	gradients(img, dx, dy);

	multiply(dx, dx, dx2);
	multiply(dy, dy, dy2);
	multiply(dx, dy, dxy);

	destruct(dx);
	destruct(dy);

	smooth(dx2, 2.0);
	smooth(dy2, 2.0);
	smooth(dxy, 2.0);

	construct_same(strengths, img);
	
	// strengths
	for(i=0; i < strengths->rows; ++i) {
		for (j=0; j < strengths->cols; ++j) {
			n = i*strengths->rows + j;
			
			strengths->px[n] = 2 * (dx2->px[n] * dy2->px[n] - dxy->px[n]*dxy->px[n]) / (dx2->px[n] + dy2->px[n]);
		}
	}

	destruct(dx2);
	destruct(dy2);
	destruct(dxy);

	local_extrema(strengths, corners);
}

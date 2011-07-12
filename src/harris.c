/*
 * Multiscale Harris operator
 */ 

#include <stdlib.h>
#include <math.h>

#include "harris.h"

// utilities
array * gaussian(float sig) {
	int i,j,n,s;
	float a,b,x;
	float c = 1/(2*M_PI*sig*sig);

	int max = (int)(5.0*sig) + 1; // maximum radius of kernel (mean)
	s = max + max - 1; // total size

	array *kernel = construct(s, s);

	for (i=0; i<s; ++i) {
		for (j=0; j<s; ++j) {
			n = i*s + j;
			a = (float)(i-max);
			b = (float)(j-max);
			
			x = 0.0 - (a*a + b*b)/(2.0*sig*sig);
			kernel->px[n] = c * expf(x);
		}
	}

	return kernel;
}

void gradients(image *img, array *dx, array *dy) {
	array *kernel = construct(3, 3);
	dx = construct_same(img);
	dy = construct_same(img);

	float tab[] = {
		-1.0, 0.0, 1.0,
		-2.0, 0.0, 2.0,
		-1.0, 0.0, 1.0
	};

	kernel->px = tab;

	dx = convolution(img, kernel);
	array *kernelT = transpose(kernel);
	dy = convolution(img, kernelT);

	destruct(kernel);
	destruct(kernelT);
}

void polar(array *dx, array *dy, array *mag, array *ang) {
	int i,j,n;
	mag = construct_same(dx);
	ang = construct_same(dx);

	for (i=0; i < dx->rows; ++i) {
		for (j=0; j < dx->cols; ++j) {
			n = i*dx->rows + j;
			
			mag->px[n] = sqrt(pow(dx->px[n], 2) + pow(dx->py[n], 2));
			ang->px[n] = atan2(dx->px[n], dy->px[n]);
		}
	}
}

// Harris operator
array * harris(array * img) {
	array *dx, *dy, *dx2, *dy2, *dxy, *strengths, *corners, *kernel;
	int i,j,n;

	kernel = gaussian(1.0);
	img = convolution(img, kernel);
	gradients(img, dx, dy);

	dx2 = multiply(dx, dx);
	dy2 = multiply(dy, dy);
	dxy = multiply(dx, dy);

	destruct(dx);
	destruct(dy);
	
	kernel = gaussian(2.0);
	dx2 = convolution(dx2, kernel);
	dy2 = convolution(dy2, kernel);
	dxy = convolution(dxy, kernel);

	destruct(kernel);

	strengths = construct_same(img);
	
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

	corners = local_extrema(strengths);
	destruct(strengths);

	return corners;
}


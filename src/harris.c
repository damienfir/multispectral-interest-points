/*
 * Multiscale Harris operator
 */ 

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "harris.h"

// utilities
array * gaussian(pixel sig) {
	int i,j,n,s;
	pixel a,b,x;
	pixel c = 1/(2*M_PI*sig*sig);

	int max = (int)(5.0*sig) + 1; // maximum radius of kernel (mean)
	s = max + max - 1; // total size

	array *kernel = construct(s, s);

	for (i=0; i<s; ++i) {
		for (j=0; j<s; ++j) {
			n = i*s + j;
			a = (pixel)(i-max);
			b = (pixel)(j-max);
			
			x = 0.0 - (a*a + b*b)/(2.0*sig*sig);
			kernel->px[n] = c * exp(x);
		}
	}

	return kernel;
}

void gradients(image *img, array *dx, array *dy) {
	array *kernel, *kernelT;
	kernel = construct(3, 3);

	double tab[] = {
		-1.0, 0.0, 1.0,
		-2.0, 0.0, 2.0,
		-1.0, 0.0, 1.0
	};

	kernel->px = tab;

	dx = convolution(img, kernel);
	kernelT = transpose(kernel);
	dy = convolution(img, kernelT);
	info(dy);

//	destruct(kernel);
//	destruct(kernelT);
}

void polar(array *dx, array *dy, array *mag, array *ang) {
	int i;
	mag = construct_same(dx);
	ang = construct_same(dx);

	for (i=0; i < dx->n; ++i) {
		mag->px[i] = sqrt(pow(dx->px[i], 2) + pow(dx->px[i], 2));
		ang->px[i] = atan2(dx->px[i], dy->px[i]);
	}
}


// Harris operator
void harris(array * img, pixels result) {
	array *dx, *dy, *dx2, *dy2, *dxy, *strengths, *kernel, *extr, *tmp;
	int i;

	// initial smoothing to avoid noise
	kernel = gaussian(1.0);
	img = convolution(img, kernel);
	gradients(img, dx, dy);

	// autocorrelation matrix
	info(dx);
	info(dy);
	dx2 = multiply(dx, dx);
	dy2 = multiply(dy, dy);
	dxy = multiply(dx, dy);
	return;

	destruct(dx);
	destruct(dy);
	
	// smoothing (average over region)
	kernel = gaussian(2.0);
	dx2 = convolution(dx2, kernel);
	dy2 = convolution(dy2, kernel);
	dxy = convolution(dxy, kernel);

	destruct(kernel);

	// calculate strength at each coordinate
	strengths = construct_same(img);
	for(i=0; i < strengths->n; ++i) {
		strengths->px[i] = 2 * (dx2->px[i] * dy2->px[i] - dxy->px[i]*dxy->px[i]) / (dx2->px[i] + dy2->px[i]);
	}

	destruct(dx2);
	destruct(dy2);
	destruct(dxy);

	// get the extremas of the strengths
	extr = local_extrema(strengths);
	copy_px(extr, result);
	destruct(strengths);

	return;
}

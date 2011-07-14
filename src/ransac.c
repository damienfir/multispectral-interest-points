#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "ransac.h"

#define N_ITERATIONS 100
#define N_POINTS 3
#define EPS 3

#define length(array) (sizeof(array)/sizeof(*(array)))

static vec2 * getsamples(vec2 *in, int n) {
	int i;
	vec2 *sampled = calloc(n, sizeof(vec2));
	int size = length(in);

	for (i=0; i < n; ++i) {
		sampled[i] = in[rand() % size];
	}

	return sampled;
}

static array * getaffinematrix(vec2 *in, vec2 *out) {
	array * mat = construct(2,3);
	return mat;
}

static vec2 * affine(array * mat, vec2 * in) {
	vec2 * out = malloc(sizeof(vec2));
	return out;
}

array * ransac(vec2 *in1, vec2 *in2) {
	int i,j,maxinliers;
	float d;
	array *best_tform;

	int size = length(in1);

	for (i=0; i<N_ITERATIONS; ++i) {
		// get sample
		vec2 *samples1 = getsamples(in1, N_POINTS);
		vec2 *samples2 = getsamples(in2, N_POINTS);
		
		// get matrix
		array *matrix = getaffinematrix(samples1, samples2);

		// count consistent
		int ninliers;
		vec2 *p1, *p2;
		for (j=0; j<size; ++j) {
			p1 = affine(matrix, &in1[j]);
			p2 = &in2[j];
			d = sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
			if (d <= EPS) {
				ninliers += 1;
			}
		}

		// get best transform 
		if (ninliers > maxinliers) {
			maxinliers = ninliers;
			best_tform = matrix;
		}
	}

	return best_tform;
}

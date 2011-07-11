#include "array.h"

void construct(array* a, int rows, int cols) {
	a = malloc(sizeof(array));
	a->cols = rows;
	a->rows = cols;
	a->px = calloc(w*h, sizeof(float));
}

void construct_same(array* a, array* o) {
	construct(a, o->rows, o->cols);
}

void destruct(array* a) {
	free(a->px);
	free(a);
}

void transpose(array* a) {
	int i,j,n;
	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			n_old = i*a->cols + j;
			n_new = i*a->rows + j;
			
			// swap
			float tmp = a->px[n_old];
			a->px[n_old] = a->px[n_new];
			a->px[n_new] = tmp;
		}
	}
}


void multiply(array* a, array* b, array* out) {
	int i,j,n;

	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			n = i*a->rows + j;

			out[n] = a->px[n] * b->px[n];
		}
	}
}

void local_extrema(array* a, array* out) {
	int up, down, left, right;
	int i,j,u,v;
	int n1, n2;
	int localmax, localmin = 1;

	construct_same(out, a);

	for (i=0; i < a->rows; ++i) {
		up = (i > 0) ? -1 : 0;
		down = (i < a->rows -1) ? 1 : 0;

		for (j=0; j < a->cols; ++j) {
			left = (j > 0) ? -1 : 0;
			right = (j < a->cols - 1) ? 1 : 0;
			
			n1 = i*a->rows + j;
			for (u=up; u <= down; ++u) {
				for (v=left; v <= right; ++v) {
					if (u == 0 && v == 0) continue;
					
					n2 = (i+u)*a->rows + (j+v);
					localmax &= a->px[n1] > a->px[n2];
					localmin &= a->px[n1] < a->px[n2];
				}
			}
		out->px[n1] = localmax || localmin;
	}
}

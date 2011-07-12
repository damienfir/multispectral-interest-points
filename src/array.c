#include <stdlib.h>

#include "array.h"

array * construct(int rows, int cols) {
	array* a = malloc(sizeof(array));
	a->cols = rows;
	a->rows = cols;
	a->px = calloc(rows*cols, sizeof(float));
	return a;
}

array * construct_same(array* o) {
	return construct(o->rows, o->cols);
}

void destruct(array* a) {
	free(a->px);
	free(a);
}

array * transpose(array* a) {
	int i,j,n_old,n_new;
	array * out = construct_same(a);

	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			n_old = i*a->cols + j;
			n_new = i*a->rows + j;
			
			out->px[n_new] = a->px[n_old];
		}
	}
	return out;
}


array * multiply(array * a, array * b) {
	int i,j,n;
	array * out = construct_same(a);

	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			n = i*a->rows + j;

			out->px[n] = a->px[n] * b->px[n];
		}
	}
	return a;
}

array * local_extrema(array* a) {
	int up, down, left, right;
	int i,j,u,v;
	int n1, n2;
	int localmax, localmin = 1;

	array * out = construct_same(a);

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
		}
		out->px[n1] = localmax || localmin;
	}
	return out;
}


array * convolution(array* a, array* kernel) {
	int i,j,k,l,m,n,o,ii,jj;
	float val;

	array * new = construct_same(a);

	int sx = (int)(kernel->rows / 2);
	int sy = (int)(kernel->cols / 2);

	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			n = i*a->rows + j; // index on array
			val = 0.0;

			for (k=0; k < kernel->rows; ++k) {
				for (l=0; l < kernel->cols; ++l) {
					m = k*kernel->rows + l; // index on kernel

					ii = i+k-sy;
					jj = j+l-sx;
					o = ii*a->rows + jj; // conv index on array

					if (ii >= 0 && ii < a->rows && jj > 0 && jj < a->cols)
						val += a->px[o] * kernel->px[m];
				}
			}
			new->px[n] = val;
		}
	}

	return new;
}


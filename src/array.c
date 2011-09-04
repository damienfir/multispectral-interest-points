#include <stdlib.h>
#include <stdio.h>

#include "array.h"

array * construct(int rows, int cols) {
	int i;
	array* a = malloc(sizeof(array));
	a->rows = rows;
	a->cols = cols;
	a->n = rows*cols;
	a->px = calloc(a->n, sizeof(pixel));
	//for (i = 0; i < a->n; i++) a->px[i] = 0.0;
	return a;
}

array * construct_same(array* o) {
	return construct(o->rows, o->cols);
}

void destruct(array* a) {
	free(a->px);
	free(a);
}

void info(array *a) {
	int i, j, ind = 0;
	int n = 5;

	printf("[array] r:%d c:%d -- %d\n", a->rows, a->cols, a->n);
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->cols; j++) {
			if (ind >= n) break;
			printf("\t[%d,%d] %.3f\n", i, j, a->px[ind]);

			ind++;
		}
		if (ind >= n) break;
	}
}

void copy(pixels from, pixels to, int n) {
	int i;
	for (i = 0; i < n; i++) {
		to[i] = from[i];
	}
}

void copy_px(array * from, pixels to) {
	copy(from->px, to, from->rows*from->cols);
}

array * transpose(array* a) {
	int i, j, n_old, n_new = 0;
	array * out = construct(a->cols, a->rows);

	n_new = 0;
	n_old = 0;
	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			n_new = j*a->rows + i;
			
			out->px[n_new] = a->px[n_old];
			n_old++;
		}
	}
	return out;
}


array * multiply(array * a, array * b) {
	int i,j,n;
	array * out = construct_same(a);

	for (i=0; i < a->n; ++i) {
		out->px[i] = a->px[i] * b->px[i];
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


array * convolution(array *a, array *kernel) {
	unsigned int i,j,k,l,m,n,o,ii,jj;
	pixel val;

	array *new = construct_same(a);

	int sx = (int)(kernel->rows / 2);
	int sy = (int)(kernel->cols / 2);

	n = 0;
	for (i=0; i < a->rows; ++i) {
		for (j=0; j < a->cols; ++j) {
			val = 0.0;
			m = 0;
			for (k=0; k < kernel->rows; ++k) {
				for (l=0; l < kernel->cols; ++l) {
					ii = i+k-sy;
					jj = j+l-sx;
					o = ii*a->rows + jj; // conv index on array

					if (ii >= 0 && ii < a->rows && jj > 0 && jj < a->cols) {
						val += a->px[o] * kernel->px[m];
					}
					m++;
				}
			}
			new->px[n] = val;
			n++;
		}
	}

	return new;
}


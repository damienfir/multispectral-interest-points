#ifndef ARRAY_H
#define ARRAY_H

typedef float* pixels;

typedef struct {
	int rows;
	int cols;
	pixels px;
} array;

typedef struct {
	int x;
	int y;
} point;

typedef array image;

array * construct(int w, int h);
array * construct_same(array * o);
void destruct(array * a);

array * transpose(array * a);
array * multiply(array * a, array * b);

array * local_extrema(array * a);
array * convolution(array* a, array* kernel);

#endif

#ifndef ARRAY_H
#define ARRAY_H

typedef double pixel;
typedef pixel* pixels;

typedef struct {
	int rows;
	int cols;
	pixels px;
} array;

typedef struct {
	double x;
	double y;
} vec2;

typedef array image;

array * construct(int w, int h);
array * construct_same(array * o);
void destruct(array * a);

void copy(pixels from, pixels to, int n);
void copy_px(array * from, pixels to);

array * transpose(array * a);
array * multiply(array * a, array * b);

array * local_extrema(array * a);
array * convolution(array* a, array* kernel);

#endif

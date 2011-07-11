
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

void construct(array* a, int w, int h);
void construct_same(array* a, array* o);
void destruct(array* a);

void transpose(array* a);
void multiply(array* a, array* b, array* out);

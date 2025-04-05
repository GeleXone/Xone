#pragma once

typedef struct matrix_struct {
	double** matrix;
	int rows;
	int columns;
} matrix_t;

typedef struct vec_struct {
	double* vec;
	int length;
} vec_t;

void vec_print(const vec_t* vec);
void matrix_print(const matrix_t* matrix);
void matrix_input(const matrix_t* matrix);

matrix_t* matrix_create(const int rows, const int columns);
matrix_t* matrix_create_transpose(matrix_t* matrix);
matrix_t* matrix_create_rotation_quat(vec_t* quat);
void matrix_transpose(matrix_t* matrix);
void matrix_set_identity(matrix_t* matrix);
void matrix_set_translate4(matrix_t* matrix, double x, double y, double z);
void matrix_set_scale4(matrix_t* matrix, double x, double y, double z);

vec_t* matrix_mult_vec(matrix_t* matrix, vec_t* vec);
vec_t* vec_mult_matrix(vec_t* vec, matrix_t* matrix);
matrix_t* matrix_mult_matrix(matrix_t* A, matrix_t* B);

vec_t* vec_create(const int length);
void vec3_set(vec_t* vec, double x, double y, double z);
void vec4_set(vec_t* vec, double x, double y, double z, double w);

void quat_set(vec_t* quat, vec_t* axis, double angle);
void quat_conjugate(vec_t* quat);

void normalize(vec_t* vec);
void cross(vec_t* out, const vec_t* a, const vec_t* b);
double dot(const vec_t* a, const vec_t* b);

void look_at(matrix_t* out, vec_t* eye, vec_t* center, vec_t* up);
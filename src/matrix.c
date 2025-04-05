#include <Xone/matrix.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>

void vec_print(const vec_t* vec) {
	for (int i = 0; i < vec->length; i++) {
		printf("%.f\n", vec->vec[i]);
	}
}
void matrix_print(const matrix_t* matrix) {
	for (int i = 0; i < matrix->rows; i++) {
		for (int j = 0; j < matrix->columns; j++)
			printf("%.f\t", matrix->matrix[i][j]);
		printf("\n");
	}
}
void matrix_input(const matrix_t* matrix) {
	printf("Matrix input %dx%d:\n", matrix->rows, matrix->columns);
	for (int i = 0; i < matrix->rows; i++) {
		for (int j = 0; j < matrix->columns; j++)
			if (scanf("%lf", &matrix->matrix[i][j]));
	}
}

matrix_t* matrix_create(const int rows, const int columns) {
	matrix_t* result;

	if (rows <= 0 || columns <= 0) {
		return NULL;
	}

	result = malloc(sizeof(matrix_t));
	if (!result)
		return NULL;
	result->rows = rows;
	result->columns = columns;
	result->matrix = calloc(rows, sizeof(double*));
	if (!result->matrix) {
		free(result);
		return NULL;
	}

	for (int i = 0; i < rows; i++)
	{
		result->matrix[i] = calloc(columns, sizeof(double));
		if (!result->matrix[i])
		{
			/* clean-up before exit */
			for (int j = 0; j < i; j++)
				free(result->matrix[j]);
			free(result->matrix);
			free(result);
			return NULL;
		}
	}
	return (result);
}
matrix_t* matrix_create_transpose(matrix_t* matrix) {
	matrix_t* result = matrix_create(matrix->rows, matrix->columns);

	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->columns; j++)
			result->matrix[i][j] = matrix->matrix[j][i];
	}

	return (result);
}
matrix_t* matrix_create_rotation_quat(vec_t* quat) {
	matrix_t* result = matrix_create(quat->length, quat->length);
	double	x = quat->vec[0], 
			y = quat->vec[1], 
			z = quat->vec[2], 
			w = quat->vec[3];

	double xx = x * x, yy = y * y, zz = z * z;
	double xy = x * y, xz = x * z, yz = y * z;
	double wx = w * x, wy = w * y, wz = w * z;

	result->matrix[0][0] = 1 - 2 * (yy + zz);
	result->matrix[0][1] = 2 * (xy + wz);
	result->matrix[0][2] = 2 * (xz - wy);

	result->matrix[1][0] = 2 * (xy - wz);
	result->matrix[1][1] = 1 - 2 * (xx + zz);
	result->matrix[1][2] = 2 * (yz + wx);

	result->matrix[2][0] = 2 * (xz + wy);
	result->matrix[2][1] = 2 * (yz - wx);
	result->matrix[2][2] = 1 - 2 * (xx + yy);
	
	result->matrix[3][3] = 1;

	return (result);
}
void matrix_transpose(matrix_t* matrix) {
	double temp;
	for (int i = 0; i < matrix->rows; i++) {
		for (int j = 0; j < matrix->columns; j++) {
			temp = matrix->matrix[i][j];
			matrix->matrix[i][j] = matrix->matrix[j][i];
			matrix->matrix[j][i] = temp;
		}
	}
}
void matrix_set_identity(matrix_t* matrix) {
	if (matrix->rows != matrix->columns) {
		printf("matrix_set_identity: Матрица не квадратная.\n"); //добавить вывод ошибки
		return -1;
	}

	for (int i = 0; i < matrix->rows; i++) {
		for (int j = 0; j < matrix->columns; j++) {
			if (i == j)
				matrix->matrix[i][j] = 1;
			else
				matrix->matrix[i][j] = 0;
		}
	}
	
	return 0;
}
void matrix_set_translate4(matrix_t* matrix, double x, double y, double z) {
	if (matrix->rows != 4 || matrix->columns != 4) {
		printf("matrix_set_translate4: Матрица не 4 на 4.\n"); //добавить вывод ошибки
		return -1;
	}

	matrix_set_identity(matrix);

	matrix->matrix[0][3] = x;
	matrix->matrix[1][3] = y;
	matrix->matrix[2][3] = z;

	return 0;
}
void matrix_set_scale4(matrix_t* matrix, double x, double y, double z) {
	if (matrix->rows != 4 || matrix->columns != 4) {
		printf("matrix_set_scale4: Матрица не 4 на 4.\n"); //добавить вывод ошибки
		return -1;
	}

	matrix_set_identity(matrix);

	matrix->matrix[0][0] = x;
	matrix->matrix[1][1] = y;
	matrix->matrix[2][2] = z;

	return 0;
}

vec_t* matrix_mult_vec(matrix_t* matrix, vec_t* vec) {
	if (matrix->columns != vec->length) {
		printf("matrix_mult_vec: умножение невозможно, количество строк не совпадает с длиной вектора.\n"); //добавить вывод ошибки
		return NULL;
	}

	vec_t* result = vec_create(vec->length); //забьётся ли так память?
	for (int i = 0; i < matrix->rows; i++)
		for (int j = 0; j < matrix->columns; j++)
			result->vec[i] += matrix->matrix[i][j] * vec->vec[j];

	return result;
}
vec_t* vec_mult_matrix(vec_t* vec, matrix_t* matrix) {
	if (vec->length != matrix->rows) {
		printf("vec_mult_matrix: умножение невозможно, количество столбцов не совпадает с длиной вектора.\n"); //добавить вывод ошибки
		return NULL;
	}
	vec_t* result = vec_create(vec->length); //забьётся ли так память?
	for (int i = 0; i < vec->length; i++)
		for (int j = 0; j < matrix->rows; j++)
			result->vec[i] += matrix->matrix[j][i] * vec->vec[j];

	return result;
}
matrix_t* matrix_mult_matrix(matrix_t* A, matrix_t* B) {
	if (A->columns != B->rows) {
		printf("vec_mult_matrix: умножение невозможно, количество столбцов не совпадает с длиной вектора.\n"); //добавить вывод ошибки
		return NULL;
	}
	matrix_t* result = matrix_create(A->rows, B->columns); //забьётся ли так память?
	for (int i = 0; i < result->rows; i++)
		for (int j = 0; j < result->columns; j++)
			for (int k = 0; k < A->columns; k++)
				result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];

	return result;
}

vec_t* vec_create(const int length) {
	vec_t* result;

	if (length <= 0) {
		return NULL;
	}
	result = malloc(sizeof(vec_t));
	if (!result)
		return NULL;
	result->length = length;
	result->vec = calloc(length, sizeof(double));
	if (!result->vec) {
		free(result);
		return NULL;
	}

	return (result);
}
void vec3_set(vec_t* vec, double x, double y, double z) {
	if (vec->length != 3) {
		printf("vec3_set: Длина вектора не 3.\n"); //добавить вывод ошибки
		return -1;
	}

	vec->vec[0] = x;
	vec->vec[1] = y;
	vec->vec[2] = z;

	return 0;
}
void vec4_set(vec_t* vec, double x, double y, double z, double w) {
	if (vec->length != 4) {
		printf("vec4_set: Длина вектора не 4.\n"); //добавить вывод ошибки
		return -1;
	}

	vec->vec[0] = x;
	vec->vec[1] = y;
	vec->vec[2] = z;
	vec->vec[3] = w;

	return 0;
}

void quat_set(vec_t* quat, vec_t* axis, double angle) {
	if (quat->length != 4) {
		printf("quat_set: Длина вектора не 4.\n"); //добавить вывод ошибки
		return -1;
	}

	double a = sin(angle / 2);
	quat->vec[0] = axis->vec[0] * a;
	quat->vec[1] = axis->vec[1] * a;
	quat->vec[2] = axis->vec[2] * a;
	quat->vec[3] = cos(angle / 2);

	return 0;
}
void quat_conjugate(vec_t* quat) {
	quat->vec[0] = -quat->vec[0];
	quat->vec[1] = -quat->vec[1];
	quat->vec[2] = -quat->vec[2];

	return 0;
}

void normalize(vec_t* vec) {
	double length = sqrt(vec->vec[0] * vec->vec[0] + vec->vec[1] * vec->vec[1] + vec->vec[2] * vec->vec[2]);
	if (length > 0) {
		vec->vec[0] /= length;
		vec->vec[1] /= length;
		vec->vec[2] /= length;
	}
}
void cross(vec_t* out, const vec_t* a, const vec_t* b) {
	out->vec[0] = a->vec[1] * b->vec[2] - a->vec[2] * b->vec[1];
	out->vec[1] = a->vec[2] * b->vec[0] - a->vec[0] * b->vec[2];
	out->vec[2] = a->vec[0] * b->vec[1] - a->vec[1] * b->vec[0];
}
double dot(const vec_t* a, const vec_t* b) {
	return a->vec[0] * b->vec[0] + a->vec[1] * b->vec[1] + a->vec[2] * b->vec[2];
}

void look_at(matrix_t* out, vec_t* eye, vec_t* center, vec_t* up) {
	vec_t* forward = vec_create(3);
	vec3_set(forward, center->vec[0] - eye->vec[0], center->vec[1] - eye->vec[1], center->vec[2] - eye->vec[2]);
	normalize(forward);

	vec_t* right = vec_create(3);
	cross(right, forward, up);

	vec_t* new_up = vec_create(3);
	cross(new_up, right, forward);

	for (int i = 0; i < 3; i++) {
		out->matrix[i][0] = right->vec[i];
		out->matrix[i][1] = new_up->vec[i];
		out->matrix[i][2] = -forward->vec[i];
		out->matrix[i][3] = 0;
	}
	out->matrix[3][0] = -dot(right, eye);
	out->matrix[3][1] = -dot(new_up, eye);
	out->matrix[3][2] = dot(forward, eye);
	out->matrix[3][3] = 1;

	return 0;
}
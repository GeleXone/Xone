#include <Xone/matrix.h>
#include <stdio.h>

int main() {
	matrix_t* A = matrix_create(2, 3);
	matrix_input(A);
	matrix_t* B = matrix_create(3, 2);
	matrix_input(B);

	matrix_print(matrix_mult_matrix(A, B));

	return 0;
}
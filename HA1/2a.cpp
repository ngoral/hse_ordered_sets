/* For all five-element binary relations find the number of:
1) Assymetric and transitive
2) Anti-symmetric and anti-reflecsive
*/

#include <iostream>
#include <array>
#include <cmath>

const size_t N = 5;

void fillMatrix(std::array<std::array<bool, N>, N> &matrix, size_t matrix_bitted)
{
	for(int i = N - 1; i >= 0; --i) {
		for(int j = N - 1; j >= 0; --j) {
			matrix[i][j] = (bool)(matrix_bitted & 1);
			matrix_bitted >>= 1;
		}
	}
}

bool isTransitive(size_t i, size_t j, std::array<std::array<bool, N>, N> &matrix)
{
	for(size_t k = 0; k < N; ++k) { //  brootforce :-(
		if(matrix[j][k]) { // if (b,c)
			if(!matrix[i][k]) { // not transitive
				return false;
			}
		}
	}

	return true;
}

bool checkPartA(std::array<std::array<bool, N>, N> &matrix)
{
	for(size_t i = 0; i < N; ++i) {
		for(size_t j = i; j < N; ++j) { // iterate over upper-triangle part
			if(matrix[i][j]){
				if(matrix[j][i]) { // not assymetric
					return false;
				}
			}

			if(i != j) {
				if(matrix[i][j]) { // if (a,b)
					if(!isTransitive(i, j, matrix)) {
						return false;
					}
				}
				if(matrix[j][i]){
					if(!isTransitive(j, i, matrix)) { // check lower part of matrix
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool checkPartB(std::array<std::array<bool, N>, N> &matrix)
{
	for(size_t i = 0; i < N; ++i) {
		if(matrix[i][i]) { // reflecsive
			return false;
		}
		for(size_t j = i; j < N; ++j) { // iterate over upper-triangle part
			if(matrix[i][j] && matrix[j][i]) { // (a,b) and (b,a)
				if(i != j) { // not anti-symmetric
					return false;
				}
			}
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	std::array<std::array<bool, N>, N> matrix;
	size_t matrix_bitted = 0; // will increment this number and use its bit map to iterate over all matrices
	unsigned int partA = 0, partB = 0;

	for(size_t i = 0; i < pow(2, N*N); ++i) {
		fillMatrix(matrix, matrix_bitted);
		if(checkPartA(matrix)) {
			partA++;
		}
		if(checkPartB(matrix)){
			++partB;
		}

		++matrix_bitted;
	}

	std::cout << "Assymetric and transitive: " << partA << std::endl;
	std::cout << "Anti-symmetric and anti-reflecsive: " << partB << std::endl;

	return 0;
}

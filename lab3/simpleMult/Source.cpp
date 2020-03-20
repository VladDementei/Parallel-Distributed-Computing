#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#include <chrono>
#include <iostream>
#include <time.h>
#define r 10000 // количество строк матрицы
#define c 10000 // количество столбцов матрицы

double A[r][c];
double B[c];
double C[r];

int main() {
	srand(time(NULL));
	std::cout << "A" << std::endl;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			A[i][j] = rand() % 10;
			//std::cout << A[i][j] << " ";
		}
		//std::cout << std::endl;
	}
	std::cout << std::endl << "B" << std::endl;
	for (int j = 0; j < c; j++) {
		B[j] = rand() % 10;
		//std::cout << B[j] << std::endl;
	}
	for (int i = 0; i < r; i++) {
		C[i] = 0;
	}

	auto dwStartTime = std::chrono::steady_clock::now();

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			C[i] += A[i][j] * B[j];
		}
	}

	auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();

	std::cout << std::endl << "C" << std::endl;
	for (int i = 0; i < r; i++) {
		//std::cout << C[i] << std::endl;
	}

	std::cout << std::endl << "Time = " << dwElapsedTime << std::endl;
	system("pause");
	return 0;
}
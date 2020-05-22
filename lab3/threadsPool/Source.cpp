#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#include <chrono>
#include <iostream>
#include <time.h>
#define p 4 // максимальное ко-во потоков
#define r 5000 // количество строк матрицы
#define c 5000 // количество столбцов матрицы

double A[r][c];
double B[c];
double C[r];

LONG queue = r; // очередь событий

DWORD WINAPI ThreadFunction(LPVOID pvParam) {
	long k;
	double sum;
	while ((k = InterlockedDecrement(&queue)) >= 0) {
		sum = 0.0;
		for (int j = 0; j < c; j++) {
			sum += A[k][j] * B[j];
		}
		C[k] = sum;
	}
	return 0;
}

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

	HANDLE hThreads[p];
	for (int i = 0; i < p; i++) {
		hThreads[i] = (HANDLE)CreateThread(NULL, 0, ThreadFunction, (LPVOID)i, 0, NULL);
		if (hThreads[i] == NULL) // обработка ошибки 
		{
			printf("Create Thread %d Error=%d\n", i, GetLastError());
			return -1;
		}
	}

	WaitForMultipleObjects(p, hThreads, true, INFINITE);
	for (int i = 0; i < p; ++i) {
		if (!CloseHandle(hThreads[i])) {
			printf("Close Thread %d Error=%d\n", i, GetLastError());
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
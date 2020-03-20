#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#include <chrono>
#include <iostream>
#include <time.h>
#define p 4 // максимальное ко-во потоков
#define r 10000 // количество строк матрицы
#define c 10000 // количество столбцов матрицы

double A[r][c]; 
double B[c];
double C[r];

HANDLE hSemaphore = CreateSemaphore(NULL, p, p, NULL);

DWORD WINAPI ThreadFunction(LPVOID pvParam) {
	int i = (int)pvParam;
	int start;
	double sum;
	sum = 0.0;
	for (int j = 0; j < c; j++) {
		sum += A[i][j] * B[j];
	}
	C[i] = sum;
	ReleaseSemaphore(hSemaphore, 1, NULL);
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

	HANDLE hThreads[r];
	int queue = 0;
	while (queue < r) {
		DWORD waitRes = WaitForSingleObject(hSemaphore, INFINITE);
		switch (waitRes)
		{
		case WAIT_FAILED:
			printf("Wait failed, Error=%d\n", GetLastError());
			break;

		case WAIT_OBJECT_0:
			hThreads[queue] = (HANDLE)CreateThread(NULL,0,ThreadFunction,(LPVOID)queue, 0, NULL);
			queue++;
			if (hThreads[queue] == NULL) // обработка ошибки 
			{
				printf("Create Thread %d Error=%d\n", queue, GetLastError());
				return -1;
			}
			break;
		}
	}

	WaitForMultipleObjects(r, hThreads, true, INFINITE);
	for (int i = 0; i < r; ++i) {
		if (!CloseHandle(hThreads[i])) {
			printf("Close Thread %d Error=%d\n", i, GetLastError());
		}
	}
	if (!CloseHandle(hSemaphore)) {
		printf("Close semaphore Error=%d\n", GetLastError());
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
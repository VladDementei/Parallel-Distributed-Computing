#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#include <chrono>
#include <iostream>
#define p 4 // количество дочерних потоков 
double pi[p]; int n = 100000000; 

DWORD WINAPI ThreadFunction(LPVOID pvParam) {
	int nParam = (int)pvParam; 
	int start; 
	double h, sum, x; 
	h = 1.0/n; 
	sum = 0.0; 
	start = nParam; 
	for (int i = start; i < n; i += p) {
		x = h * i; 
		sum += 4.0 / (1.0 + x * x);
	}
	pi[nParam] = h * sum; 
	return 0; 
} 

int main() { 
	//DWORD dwStartTime = GetTickCount();
	//auto dwStartTime = std::chrono::steady_clock::now();
	LARGE_INTEGER liFrequency, liStartTime, liFinishTime;
	QueryPerformanceFrequency(&liFrequency);
	// получаем стартовое время
	QueryPerformanceCounter(&liStartTime);

	HANDLE hThreads[p]; 
	int k; 
	double sum; // создание дочерних потоков 
	for (k = 0; k < p; ++k) {
		hThreads[k] = (HANDLE)CreateThread(NULL,
			0,
			ThreadFunction,
			(LPVOID)k,
			0,
			NULL);
		if (hThreads[k] == NULL) // обработка ошибки 
		{
			printf("Create Thread %d Error=%d\n", k, GetLastError());
			return -1;
		}
	} // ожидание завершения дочерних потоков 
	WaitForMultipleObjects(p, hThreads, TRUE,INFINITE); 
	for (k = 0; k < p; ++k) {
		CloseHandle(hThreads[k]);
	}
	sum = 0.0; 
	for (k = 0; k < p; ++k) {
		sum += pi[k];
	}
	//DWORD dwElapsedTime = GetTickCount() - dwStartTime;
	//auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();
	QueryPerformanceCounter(&liFinishTime);

	printf("PI = %.16f\n", sum); 
	std::cout << "Time = " << 1000.*(liFinishTime.QuadPart - liStartTime.QuadPart) / liFrequency.QuadPart;
	system("pause");
	return 0; 
}
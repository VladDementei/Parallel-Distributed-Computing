#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#include <chrono>
#include <iostream>
int n = 1000000;

int main() {
	//DWORD dwStartTime = GetTickCount();
	//auto dwStartTime = std::chrono::steady_clock::now();
	//LARGE_INTEGER liFrequency, liStartTime, liFinishTime;
	//QueryPerformanceFrequency(&liFrequency);
	// получаем стартовое время
	//QueryPerformanceCounter(&liStartTime);
	auto start = std::chrono::high_resolution_clock::now();
	int k;
	double sum = 0.0; // создание дочерних потоков 
	double h, x;
	h = 1.0 / n;
	for (k = 0; k < n; ++k) {
		x = h * k;
		sum += h * (4.0 / (1.0 + x * x));

	} // ожидание завершения дочерних потоков 
	//DWORD dwElapsedTime = GetTickCount() - dwStartTime;
	//auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();
	//QueryPerformanceCounter(&liFinishTime);
	auto stop = std::chrono::high_resolution_clock::now();


	printf("PI = %.16f\n", sum);
	//std::cout << "Time = " << 1000.*(liFinishTime.QuadPart - liStartTime.QuadPart) / liFrequency.QuadPart;
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	system("pause");
	return 0;
}
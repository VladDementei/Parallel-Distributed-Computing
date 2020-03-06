#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#include <chrono>
#include <iostream>
int n = 100000000;

int main() {
	//DWORD dwStartTime = GetTickCount();
	//auto dwStartTime = std::chrono::steady_clock::now();
	LARGE_INTEGER liFrequency, liStartTime, liFinishTime;
	QueryPerformanceFrequency(&liFrequency);
	// �������� ��������� �����
	QueryPerformanceCounter(&liStartTime);

	int k;
	double sum = 0.0; // �������� �������� ������� 
	double h, x;
	h = 1.0 / n;
	for (k = 0; k < n; ++k) {
		x = h * k;
		sum += h * (4.0 / (1.0 + x * x));

	} // �������� ���������� �������� ������� 
	//DWORD dwElapsedTime = GetTickCount() - dwStartTime;
	//auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();
	QueryPerformanceCounter(&liFinishTime);

	printf("PI = %.16f\n", sum);
	std::cout << "Time = " << 1000.*(liFinishTime.QuadPart - liStartTime.QuadPart) / liFrequency.QuadPart;
	system("pause");
	return 0;
}
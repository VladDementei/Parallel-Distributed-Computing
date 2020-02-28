#include <windows.h> 
#include <process.h> 
#include <stdio.h> 
#define p 2 // ���������� �������� ������� 
double pi[p]; int n = 1000000; 

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
	HANDLE hThreads[p]; 
	int k; 
	double sum; // �������� �������� ������� 
	for (k = 0; k < p; ++k) {
		hThreads[k] = (HANDLE)CreateThread(NULL,
			0,
			ThreadFunction,
			(LPVOID)k,
			0,
			NULL);
		if (hThreads[k] == NULL) // ��������� ������ 
		{
			printf("Create Thread %d Error=%d\n", k, GetLastError());
			return -1;
		}
	} // �������� ���������� �������� ������� 
	WaitForMultipleObjects(p, hThreads, TRUE,INFINITE); 
	for (k = 0; k < p; ++k) {
		CloseHandle(hThreads[k]);
	}
	sum = 0.0; 
	for (k = 0; k < p; ++k) {
		sum += pi[k];
	}
	printf("PI = %.16f\n", sum); 
	system("pause");
	return 0; 
}
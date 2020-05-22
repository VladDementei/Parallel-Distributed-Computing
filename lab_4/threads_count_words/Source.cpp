#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <queue>
#include <chrono>
using namespace std;

#define buf_size 10000000
queue<string> messages;
//HANDLE emptySemaphore = CreateSemaphore(NULL, buf_size, buf_size, NULL);
HANDLE fullSemaphore = CreateSemaphore(NULL, 0, buf_size, NULL);
HANDLE access = CreateMutex(NULL, FALSE, "Mutex");
LONG answer = 0;
 
DWORD WINAPI Producer(LPVOID pvParam) {
	ifstream in("input.txt");
	string sentence;

	//HANDLE syncro[1];
	//syncro[0] = emptySemaphore;
	//syncro[1] = access;

	TCHAR szRetRes[100];

	if (in.is_open()) {
		while (getline(in, sentence)) {
			//DWORD waitRes = WaitForMultipleObjects(2, syncro, TRUE, INFINITE);
			DWORD waitRes = WaitForSingleObject(access, INFINITE);
			switch (waitRes) {
			case WAIT_OBJECT_0: 
				messages.push(sentence);
				ReleaseMutex(access);
				ReleaseSemaphore(fullSemaphore, 1, NULL);
				break;
			case WAIT_ABANDONED_0:
				cout << "Wait abandoned" << endl;
				wsprintf(szRetRes, TEXT("WAIT_ABANDONED_0=%d"), GetLastError());
				MessageBox(NULL, szRetRes, "WaitForMultipleObjects", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			case WAIT_FAILED:
				cout << "Wait failed" << endl;
				wsprintf(szRetRes, TEXT("WAIT_FAILED_0=%d"), GetLastError());
				MessageBox(NULL, szRetRes, "WaitForMultipleObjects", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
		}
		in.close();
	}
	else {
		cout << "File not found" << endl;
	}
	ReleaseSemaphore(fullSemaphore, 1, NULL);//
}

DWORD WINAPI Consumer(LPVOID pvParam) {

	HANDLE syncro[2];
	syncro[0] = fullSemaphore;
	syncro[1] = access;
	string sentence;
	string textToSearch = "big";
	TCHAR szRetRes[100];
	int pos = -1;

	while (true) {
		DWORD waitRes = WaitForMultipleObjects(2, syncro, TRUE, INFINITE);
		switch (waitRes) {
		case WAIT_OBJECT_0:
			if (messages.size() == 0) {//
				return 0;//
			}//
			sentence = messages.front();
			messages.pop();
			ReleaseMutex(access);
			//ReleaseSemaphore(emptySemaphore, 1, NULL);
			pos = -1;
			while ((pos = sentence.find(textToSearch, pos + 1)) >= 0) {
				InterlockedIncrement(&answer);
			}
			break;
		case WAIT_ABANDONED_0:
			cout << "Wait abandoned" << endl;
			wsprintf(szRetRes, TEXT("WAIT_ABANDONED_0=%d"), GetLastError());
			MessageBox(NULL, szRetRes, "WaitForMultipleObjects", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		case WAIT_FAILED:
			cout << "Wait failed" << endl;
			wsprintf(szRetRes, TEXT("WAIT_FAILED_0=%d"), GetLastError());
			MessageBox(NULL, szRetRes, "WaitForMultipleObjects", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	}
}


int main() {

	auto dwStartTime = std::chrono::steady_clock::now();
	HANDLE hThreads[2];
	hThreads[0] = (HANDLE)CreateThread(NULL, 0, Producer, (LPVOID)0, 0, NULL);
	if (hThreads[0] == NULL)
	{
		printf("Create Thread %d Error=%d\n",0, GetLastError());
		return -1;
	}
	hThreads[1] = (HANDLE)CreateThread(NULL, 0, Consumer, (LPVOID)0, 0, NULL);
	if (hThreads[1] == NULL)
	{
		printf("Create Thread %d Error=%d\n", 0, GetLastError());
		return -1;
	}

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
	/*for (int i = 0; i < buf_size; i++)
	{
		WaitForSingleObject(emptySemaphore, INFINITE);
	}*/

	//TerminateThread(hThreads[1], 0);


	for (int i = 0; i < 2; ++i) {
		if (!CloseHandle(hThreads[i])) {
			printf("Close Thread %d Error=%d\n", i, GetLastError());
		}
	}
	if (!CloseHandle(access)) {
		printf("Close Thread Error=%d\n", GetLastError());
	}
	if (!CloseHandle(fullSemaphore)) {
		printf("Close Thread Error=%d\n", GetLastError());
	}
	/*if (!CloseHandle(emptySemaphore)) {
		printf("Close Thread Error=%d\n", GetLastError());
	}*/

	auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();

	cout << "ANSWER " << answer << endl;

	std::cout << std::endl << "Time = " << dwElapsedTime << std::endl;

	system("pause");
	return 0;
}
#define HAVE_STRUCT_TIMESPEC
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>
#include <queue>
#include <chrono>
#include <semaphore.h>
#include <thread>
#include <chrono>
using namespace std;

#define buf_size 2
queue<string> messages;
sem_t emptySemaphore;
pthread_mutex_t access;
pthread_cond_t pop_access;

int answer = 0;

void* Producer(void* pvParam) {
	ifstream in("input.txt");
	string sentence;
	int rc;

	if (in.is_open()) {
		while (getline(in, sentence)) {
			rc = sem_wait(&emptySemaphore);
			pthread_mutex_lock(&access);
		
			messages.push(sentence);
			pthread_cond_signal(&pop_access);

			pthread_mutex_unlock(&access);
		}
		in.close();
	}
	else {
		std::cout << "File not found" << endl;
	}
	pthread_exit(NULL);
	return NULL;
}

void* Consumer(void* pvParam) {

	string sentence;
	string textToSearch = "big";
	int pos = -1;

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		pthread_mutex_lock(&access); 
		pthread_cond_wait(&pop_access, &access);

		if (messages.size() > 0) {
			sentence = messages.front();
			messages.pop();
			sem_post(&emptySemaphore);
			pos = -1;
		}
		else {
			pos = 0;
		}

		pthread_mutex_unlock(&access);
		
		if (pos != 0) {
			pos = -1;
			while ((pos = sentence.find(textToSearch, pos + 1)) >= 0) {
				answer++;
			}
		}

	}
	pthread_exit(NULL);
	return NULL;
}


int main() {

	auto dwStartTime = std::chrono::steady_clock::now();
	int rc;
	pthread_attr_t attr;
	pthread_t thread_id[2];
	rc = sem_init(&emptySemaphore, 0, buf_size);
	if (rc)
	{ 
		printf("ERROR: return code from sem_init() emptySemaphore %d\n", rc); 
		return -1; 
	};

	rc = pthread_mutex_init(&access, NULL);
	if (rc)
	{
		printf("ERROR: return code from pthread_mutex_init() access %d\n", rc);
		return -1;
	};

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	rc = pthread_create(&thread_id[0], &attr, Producer, (void*)0); 
	if (rc)
	{ 
		printf("ERROR: return code %d from pthread_create() Producer\n", rc);
		return -1; 
	};
	rc = pthread_create(&thread_id[1], &attr, Consumer, (void*)0);
	if (rc)
	{
		printf("ERROR: return code %d from pthread_create() Consumer\n", rc);
		return -1;
	};
	


	rc = pthread_join(thread_id[0], NULL);
	if (rc)
	{
		printf("ERROR: return code %d from pthread_join() Producer\n", rc);
		return -1;
	};
	for (int i = 0; i < buf_size; i++)
	{
		rc = sem_wait(&emptySemaphore);
		if (rc)
		{
			printf("ERROR: return code %d from sem_wait() emptySemaphore\n", rc);
			return -1;
		};
	}

	rc = pthread_cancel(thread_id[1]);
	if (rc)
	{
		printf("ERROR: return code %d from pthread_cancel() Consumer\n", rc);
		return -1;
	};
	

	pthread_attr_destroy(&attr);
	rc = sem_destroy(&emptySemaphore);
	if (rc) {
		printf("destroy empty semaphore Error=%d\n", rc);
	}

	auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();

	cout << "ANSWER " << answer << endl;

	std::cout << std::endl << "Time = " << dwElapsedTime << std::endl;

	system("pause");
	pthread_exit(NULL);
	return 0;
}

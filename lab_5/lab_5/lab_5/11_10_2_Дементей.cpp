#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>
using namespace std;

struct pcout : public stringstream {
	static inline mutex cout_mutex;
	~pcout() {
		lock_guard<mutex> l{ cout_mutex };
		cout << rdbuf();
		cout.flush();
	}
};

void ThreadFunction(int threadId) {
	pcout{} << "Hello world from thread " + to_string(threadId) + "\n";
	pcout{} << threadId;
	pcout{} << 'a';
}

int main() {
	std::vector<std::thread> threads(5);
	for (int k = 1; k < 5; ++k)
		threads[k] = std::thread(ThreadFunction, k);

	for (int k = 1; k < 5; ++k)
		threads[k].join();
	system("pause");
}
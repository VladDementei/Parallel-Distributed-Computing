//#include <iostream>
//#include <string>
//#include <list>
//#include <vector>
//#include <thread>
//#include <mutex>
//#include <stdexcept>
//
//using namespace std;
//
//std::mutex coutSyncro;
//template<typename T>
//void syncronizedCout(T param) {
//	try {
//		std::lock_guard<std::mutex> lock_guard(coutSyncro);
//		cout << param;
//	}
//	catch (std::logic_error&) {
//		std::cout<<"exception caught \n";
//	}
//}
//
//
//class SyncronizedList {
//
//	list<int> some_list;
//	std::mutex mtx;
//
//public:
//	SyncronizedList() {
//
//	}
//
//	void add_to_list(int new_value) {
//		try {
//			std::lock_guard<std::mutex> lock_guard(mtx);
//			some_list.push_back(new_value);
//		}
//		catch (std::logic_error&) {
//			syncronizedCout("exception caught \n");
//		}
//	
//	}
//	bool list_contains(int value_to_find) {
//		bool answer;
//		try {
//			std::lock_guard<std::mutex> lock_guard(mtx);
//			answer = (std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end());
//		}
//		catch (std::logic_error&) {
//			syncronizedCout("exception caught \n");
//		}
//		return answer;
//	}
//
//};
//
//void ThreadFunction(int threadId, SyncronizedList& list) {
//	syncronizedCout("thread " + to_string(threadId) + " add to list " + to_string(threadId) + "\n");
//	list.add_to_list(threadId);
//	syncronizedCout("thread " + to_string(threadId) + " add to list " + to_string(2* threadId) + "\n");
//	list.add_to_list(threadId * 2);
//	syncronizedCout("thread " + to_string(threadId) + " get " + to_string(threadId) + " from list is " + to_string(list.list_contains(threadId)) + "\n");
//	syncronizedCout("thread " + to_string(threadId) + " get " + to_string(0) + " from list is " + to_string(list.list_contains(0)) + "\n");
//}
//
//int main() {
//	std::vector<std::thread> threads(5);
//	SyncronizedList list;
//	for (int k = 1; k < 5; ++k)
//		threads[k] = std::thread(ThreadFunction, k, std::ref(list));
//						 
//	for (int k = 1; k < 5; ++k)
//		threads[k].join();
//
//	syncronizedCout(5);
//	syncronizedCout(true);
//	system("pause");
//}
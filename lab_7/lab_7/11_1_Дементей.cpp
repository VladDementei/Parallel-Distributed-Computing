#include <iostream>
#include <queue>
#include <tuple>
#include <sstream>
#include <sstream>
#include <condition_variable>
#include <thread>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;
using namespace chrono_literals;

class ReverseWord {
	 queue<string> q;
	 mutex mut;
	 condition_variable cv;
	 bool finished{ false };

	void producer (string file_name) {
		ifstream in(file_name);
		string line;
		if (in.is_open()) {
			while (getline(in, line)) {
				{
					lock_guard<mutex> lk{ mut };
					q.push(line);
				}
				cv.notify_all();
			}
		}
		{
			lock_guard<mutex> lk{ mut };
			finished = true;
		}
		cv.notify_all();
		in.close();
	}

	void consumer(string file_name) {
		ofstream out(file_name, ios::out);
		string line;
		string word;
		while (!finished) {
			unique_lock<mutex> l{ mut };
			cv.wait(l, [this] { return !q.empty() || finished; });
			while (!q.empty()) {
				line = q.front();
				stringstream ss(line + " \n");
				while (getline(ss, word, ' ')) {
					std::reverse(word.begin(), word.end());
					out << word << (word._Equal("\n") ? "" : " ");
				}
				q.pop();
			}
		}
		out.flush();
		out.close();
	}

public:

	void parallel_reverse(string input_file_name, string output_file_name) {
		thread producer{ &ReverseWord::producer, this, input_file_name};
		thread consumer{ &ReverseWord::consumer, this, output_file_name};

		producer.join();
		consumer.join();
	}

	void simple_reverse(string input_file_name, string output_file_name) {
		ifstream in(input_file_name);
		ofstream out(output_file_name, ios::out);

		string line;
		string word;
		if (in.is_open()) {
			while (getline(in, line)) {
				stringstream ss(line + " \n");
				while (getline(ss, word, ' ')) {
					std::reverse(word.begin(), word.end());
					out << word << (word._Equal("\n") ? "" : " ");
				}
			}
		}
		in.close();
		out.flush();
		out.close();
	}

};
int main() {
	auto start = std::chrono::high_resolution_clock::now();
	ReverseWord().parallel_reverse("input.txt", "output.txt");	//ReverseWord().simple_reverse("input.txt", "output.txt");
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	system("pause");
}
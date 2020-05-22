#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
int main() {
	auto dwStartTime = std::chrono::steady_clock::now();

	ifstream in("input.txt");
	string sentence;

	if (in.is_open()) {
		int counter = 0;
		while (getline(in, sentence)) {
			int pos = -1;
			while ((pos = sentence.find("big", pos + 1)) >= 0) {
				counter++;
			}
		}
		in.close();
		auto dwElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - dwStartTime).count();

		cout << "Answer  " << counter << endl;

		std::cout << std::endl << "Time = " << dwElapsedTime << std::endl;
	}
	else {
		cout << "File not found" << endl;
	}
	system("pause");
	return 0;

}
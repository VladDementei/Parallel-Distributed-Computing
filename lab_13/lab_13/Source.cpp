//#include <iostream>
//#include <vector>
//#include <time.h>
//#include <thread>
//#include <algorithm>
//
//using namespace std;
//int main() {
//	int num_rows = 350;
//	int num_columns = 550;
//	std::vector<std::vector<int>> vect = std::vector<std::vector<int>>(num_rows, std::vector<int>(num_columns));
//	srand(time(NULL));
//	for (auto& row : vect) {
//		for (auto& col : row) {
//			col = rand() % 2000000000;
//		}
//	}
//
//	auto start = std::chrono::high_resolution_clock::now();
//	int max = vect[0][0];
//	for (auto& row : vect) {
//		int row_max = *max_element(row.begin(), row.end());
//		if (row_max > max) {
//			max = row_max;
//		}
//	}
//	auto stop = std::chrono::high_resolution_clock::now();
//	std::cout << "Max = " << max << '\n';
//	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
//	system("pause");
//}
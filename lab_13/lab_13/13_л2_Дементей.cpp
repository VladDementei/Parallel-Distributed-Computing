//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <execution>
//#include <cmath>
//using namespace std;
//
//int main() {
//	vector<int> vect;
//	for (int i = -1000000; i < 1000000; i++) {
//		vect.push_back(i);
//	}
//
//	auto start = std::chrono::high_resolution_clock::now();
//	int res = count_if(std::execution::seq, vect.begin(), vect.end(), [](int i) {return i % 2 == 0; });//std::execution::par
//	auto stop = std::chrono::high_resolution_clock::now();
//	std::cout << "Num of even numbers = " << res << endl;
//	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
//
//	start = std::chrono::high_resolution_clock::now();
//	res = *max_element(std::execution::seq, vect.begin(), vect.end(), [](int i, int j) {return abs(i) < abs(j); });//std::execution::par
//	stop = std::chrono::high_resolution_clock::now();
//	std::cout << "Absolute max is = " << res << endl;
//	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
//
//	start = std::chrono::high_resolution_clock::now();
//	std::vector<int>::iterator it = find(std::execution::seq, vect.begin(), vect.end(), 6942);//std::execution::par
//	stop = std::chrono::high_resolution_clock::now();
//	std::cout << "Search elem index is " << distance(vect.begin(), it) << endl;
//	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
//	system("pause");
//}

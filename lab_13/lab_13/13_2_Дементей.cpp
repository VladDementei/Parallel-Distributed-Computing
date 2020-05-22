#include <thread>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <time.h>
#include <future>

template<typename Iterator, typename T>
T parallel_max(Iterator first, Iterator last, std::function<T (const std::vector<T>&)> callback)
{
	ptrdiff_t const range_length = last - first;
	if (!range_length) return (T)0;
	if (range_length == 1) {
		return callback(*first);
	}
	Iterator const mid = first + (range_length / 2);
	std::future<T> bgtask = std::async(&parallel_max<Iterator, T>, first, mid, callback);
	T part_ans;
	try {
		part_ans = parallel_max(mid, last, callback);
	}
	catch (...) {
		bgtask.wait();
		throw;
	}
	return std::max(bgtask.get(), part_ans);
}

int main()
{
	int num_rows = 6;
	int num_columns = 4;
	std::vector<std::vector<int>> vect = std::vector<std::vector<int>>(num_rows, std::vector<int>(num_columns));
	srand(time(NULL));
	for (auto& row : vect) {
		for (auto& col : row) {
			col = rand() % 2000000000;
		}
	}
	std::function<int(const std::vector<int>&)> callback = [](const std::vector<int>& row) -> int {return *max_element(row.begin(), row.end()); };

	auto start = std::chrono::high_resolution_clock::now();
	int max = parallel_max(vect.begin(), vect.end(), callback);
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Max = " << max << '\n';
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	system("pause");
	return 0;
}


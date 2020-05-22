#include <thread>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <time.h>


template <typename Iterator, typename T>
struct find_max_block
{
	void operator()(Iterator first, Iterator last, T& result)
	{
		result = (*first)[0];
		std::for_each(first, last, [&](const std::vector<T>& row)
		{
			T max = *max_element(row.begin(), row.end());
			if (max > result) {
				result = max;
			}
		});
	}
};
template <typename Iterator, typename T>
T parallel_matrix_max(Iterator first_line, Iterator last_line, T init)
{
	unsigned long const length = std::distance(first_line, last_line);
	if (!length)
		return init;
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	num_threads = 2ul;
	std::cout << "Num threads " << num_threads << std::endl;
	unsigned long const block_size = length / num_threads;
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start = first_line;
	for (unsigned long i = 0; i<(num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(find_max_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}
	find_max_block<Iterator, T>()(block_start, last_line, results[num_threads - 1]);
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	return *max_element(results.begin(), results.end());
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


	auto start = std::chrono::high_resolution_clock::now();
	int max = parallel_matrix_max(vect.begin(), vect.end(), 0);
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Max = " << max << '\n';
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	system("pause");
	return 0;
}
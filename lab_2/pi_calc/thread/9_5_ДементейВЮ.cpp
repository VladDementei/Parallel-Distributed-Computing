#include <thread>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>

template <typename Iterator, typename T>
struct mult_block
{
	void operator()(Iterator start_first, Iterator end_first, Iterator start_second, Iterator end_second, T& result)
	{
		result = std::inner_product(start_first, end_first, start_second, result);
	}
};
template <typename Iterator, typename T>
T parallel_vector_mult(Iterator start_first, Iterator end_first, Iterator start_second, Iterator end_second, T init) throw(std::string)
{
	unsigned long const length = std::distance(start_first, end_first);
	if (!length)
		return init;
	if (length != std::distance(start_second, end_second)) {
		throw std::string("error: different vectors length");
	}
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	std::cout << "Num threads " << num_threads << std::endl;
	unsigned long const block_size = length / num_threads;
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start_first = start_first;
	Iterator block_start_second = start_second;
	for (unsigned long i = 0; i<(num_threads - 1); ++i)
	{
		Iterator block_end_first = block_start_first;
		Iterator block_end_second = block_start_second;
		std::advance(block_end_first, block_size);
		std::advance(block_end_second, block_size);
		threads[i] = std::thread(mult_block<Iterator, T>(), block_start_first, block_end_first, block_start_second, block_end_second, std::ref(results[i]));
		block_start_first = block_end_first;
		block_start_second = block_end_second;
	}
	mult_block<Iterator, T>()(block_start_first, end_first, block_start_second, end_second, results[num_threads - 1]);
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	return std::accumulate(results.begin(), results.end(), init);
}
void main()
{
	/*std::list<int> lst1 = { 1,2,3,4,5,6,7,8,9,10 };
	std::list<int> lst2 = { 1,2,3,4,5,6,7,8,9,10 };*/
	std::list<int> lst1(1000000, 2);
	std::list<int> lst2 = lst1;

	try {
		auto start = std::chrono::high_resolution_clock::now();
		int sum = parallel_vector_mult(lst1.begin(), lst1.end(), lst2.begin(), lst2.end(), 0);
		auto stop = std::chrono::high_resolution_clock::now();
		std::cout << "sum = " << sum << '\n';
		std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	}
	catch (std::string error) {
		std::cout << error << std::endl;
	}
	system("pause");
}
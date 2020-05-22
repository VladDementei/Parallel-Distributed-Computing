#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>


class Pi {
	unsigned long num_threads;
	const int n = 100000000;

public:
	void operator()(unsigned long threadId,  double& result) const
	{
		int start;
		double h, sum, x;
		h = 1.0 / n;
		sum = 0.0;
		start = threadId;
		for (int i = start; i < n; i += num_threads) {
			x = h * i;
			sum += 4.0 / (1.0 + x * x);
		}
		result = h * sum;
	}
	
	Pi() {
	}

	Pi(unsigned long num_threads) {
		this->num_threads = num_threads;
	}

	double parallel_pi(unsigned long request_num_threads)
	{
		unsigned long const hardware_threads = std::thread::hardware_concurrency();
		std::cout << "Hardware threads " << hardware_threads << std::endl;
		this->num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, request_num_threads);
		std::vector<double> results(num_threads);
		std::vector<std::thread> threads(num_threads - 1);
		for (unsigned long i = 0; i<(num_threads - 1); ++i)
		{
			threads[i] = std::thread(Pi(num_threads), i, std::ref(results[i]));
		}
		this->operator()(num_threads - 1, results[num_threads - 1]);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
		return std::accumulate(results.begin(), results.end(), 0.0);
	}

};

void main()
{
	auto start = std::chrono::high_resolution_clock::now();
	double sum = Pi().parallel_pi((unsigned long)2);
	auto stop = std::chrono::high_resolution_clock::now();
	printf("PI = %.16f\n", sum);
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	system("pause");
}

#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>

class IntegralCalculator {

	unsigned long num_threads;
	int partitions_num = 1000000;

public:
	void operator()(unsigned long threadId, double a, double b, std::function<double(double)>& callback, double& result) const
	{
		int start;
		double h, sum, x;
		h = (b - a) / partitions_num;
		sum = 0.0;
		start = threadId;
		for (int i = start; i < partitions_num; i += num_threads) {
			x = a + h * i;
			sum += callback(x);
		}
		result = h * sum;
	}


	IntegralCalculator(unsigned long required_num_threads) {
		this->num_threads = required_num_threads;
	}

	void setPartitionsNum(int partitions_num) {
		this->partitions_num = partitions_num;
	}

	void setNumThreads(unsigned long required_num_threads) {
		this->num_threads = required_num_threads;
	}

	double parallel_integration(double a, double b, std::function<double(double)> callback)
	{
		unsigned long const hardware_threads = std::thread::hardware_concurrency();
		std::cout << "Hardware threads " << hardware_threads << std::endl;
		this->num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, this->num_threads);
		std::cout << "Num threads for calculation " << num_threads << std::endl;
		std::vector<double> results(num_threads);
		std::vector<std::thread> threads(num_threads - 1);
		for (unsigned long i = 0; i<(num_threads - 1); ++i)
		{
			threads[i] = std::thread(IntegralCalculator(num_threads), i, a, b, std::ref(callback), std::ref(results[i]));
		}
		this->operator()(num_threads - 1, a, b, callback, results[num_threads - 1]);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
		return std::accumulate(results.begin(), results.end(), 0.0);
	}

};

void main()
{
	std::function<double(double)> callback = [](double x) -> double { return  exp(x) + 1; };
	double a = 0.0;
	double b = 1.0;


	IntegralCalculator calculator((unsigned long)1);
	calculator.setPartitionsNum(1000000);

	auto start = std::chrono::high_resolution_clock::now();
	double sum = calculator.parallel_integration(a, b, callback);
	auto stop = std::chrono::high_resolution_clock::now();

	printf("Answer = %.3f\n", sum);
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";

	calculator.setNumThreads((unsigned long)2);

	start = std::chrono::high_resolution_clock::now();
	sum = calculator.parallel_integration(a, b, callback);
	stop = std::chrono::high_resolution_clock::now();

	printf("Answer = %.3f\n", sum);
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";

	calculator.setNumThreads((unsigned long)4);

	start = std::chrono::high_resolution_clock::now();
	sum = calculator.parallel_integration(a, b, callback);
	stop = std::chrono::high_resolution_clock::now();

	printf("Answer = %.3f\n", sum);
	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
	system("pause");
}

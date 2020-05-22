//#include <thread>
//#include <iostream>
//#include <vector>
//using namespace std;
//
//#define p 2
//double pi[p];
//int n = 100000000;
//
//void ThreadFunction(int threadId) {
//	int start;
//	double h, sum, x;
//	h = 1.0 / n;
//	sum = 0.0;
//	start = threadId;
//	for (int i = start; i < n; i += p) {
//		x = h * i;
//		sum += 4.0 / (1.0 + x * x);
//	}
//	pi[threadId] = h * sum;
//}
//
//void main()
//{
//	auto start = std::chrono::high_resolution_clock::now();
//
//	std::vector<std::thread> threads(p);
//	int k;
//	for (k = 0; k<p; ++k)// создание дочерних потоков
//		threads[k] = std::thread(ThreadFunction, k);
//
//	for (k = 0; k<p; ++k) // ожидание завершения дочерних потоков
//		threads[k].join();
//	double sum = 0.;
//	for (k = 0; k<p; ++k)sum += pi[k];
//
//	auto stop = std::chrono::high_resolution_clock::now();
//	printf("PI = %.16f\n", sum);
//	std::cout << "Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " miliseconds\n";
//	system("pause");
//}
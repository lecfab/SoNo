#include "tools.h"

using namespace std;

void print_time(chrono::steady_clock::time_point begin, chrono::steady_clock::time_point end) {
	long int diff = chrono::duration_cast<chrono::milliseconds>(end - begin).count();  int factor = 1000;
	long int diff_copy = diff;
	long int h = diff / 3600 / factor;	diff -= h * 3600 * factor;
	long int m = diff / 60 / factor;	diff -= m * 60 * factor;
	long int s = diff / factor;			diff -= s * factor;
	printf("\t- Time = %ldh %ldm %lds %ld\t\tms=%ld\n", h, m, s, diff, diff_copy );
}

// https://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html
// template <typename Builder>
// auto makeAndProcessObject (const Builder& builder) -> decltype( builder.makeObject() )
// {
//     auto val = builder.makeObject();
//     // do stuff with val
//     return val;
// }

// template <typename T>
// auto Timer (T &function, int argc, char** argv) -> decltype(function(argc, argv))
// {
// 	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

// 	//time_t t1=time(NULL);
// 	auto output = function(argc, argv);

// 	chrono::steady_clock::time_point end = chrono::steady_clock::now();
// 	print_time(begin, end);
// 	return output;
// }

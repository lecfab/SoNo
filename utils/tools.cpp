#ifndef TOOLS
#define TOOLS

#include <iostream>
#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files
#include <ctime>//to estimate the runing time
#include <string>
#include <chrono>

#include <vector>
#include <tuple>
#include <list>

#ifndef DEBUG
#define Debug(msg) {}
#else
#define Debug(msg) { cout << "\tDebug: " << msg << endl; fflush(stdout); }
#endif
#define Alert(msg) { cout << "\tAlert: " << msg << endl; fflush(stdout); }
#define Info(msg) { cout << "\tInfo: " << msg << endl; fflush(stdout); }

#ifndef NLINKS
#define NLINKS 100000000
#endif

#define TimeBegin() auto time_begin = chrono::steady_clock::now(); auto time_previous = time_begin, time_now = time_begin; auto time_diff = chrono::duration_cast<chrono::milliseconds>(time_now - time_previous).count();
#define TimeStep(m) cout << m; time_now = chrono::steady_clock::now(); print_time(time_previous, time_now); time_previous = time_now;
#define TimeTotal()	cout << "Total"; time_now = chrono::steady_clock::now(); print_time(time_begin, time_now);

// #define TimeRecStep(m) time_now = chrono::steady_clock::now(); cout << chrono::duration_cast<chrono::milliseconds>(time_now - time_previous).count() << "\t"; time_previous = time_now; Debug(m)
// #define TimeRecTotal() time_now = chrono::steady_clock::now(); cout << chrono::duration_cast<chrono::milliseconds>(time_now - time_begin).count() << endl;

#define TimeRecStep(m, out) time_now = chrono::steady_clock::now(); out << m << "\t" << chrono::duration_cast<chrono::milliseconds>(time_now - time_previous).count() << endl; time_previous = time_now;
#define TimeRecTotal(out) time_now = chrono::steady_clock::now(); out << "total\t" << chrono::duration_cast<chrono::milliseconds>(time_now - time_begin).count() << endl;

// #define TimeRecBegin() vector<chrono::steady_clock::time_point> time_rec; time_rec.push_back(chrono::steady_clock::now());
// #define TimeRecStep(m) time_rec.push_back(chrono::steady_clock::now());
// #define TimeRecTotal() { auto t0 = time_rec[0], t1=t0; for(auto &t : time_rec) { cout << chrono::duration_cast<chrono::milliseconds>(t - t1).count() << ","; t1=t; } cout << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() << endl; }


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


typedef unsigned long ul;
typedef pair<ul,ul> edge;

//edge list structure:
typedef struct {
	ul n;//number of nodes
	ul e;//number of edges
	vector<edge> edges;//list of edges
} edgelist;

struct Keyvalue {
	ul key;
	ul val;
	Keyvalue(ul key, ul val) : key(key), val(val) {}
	//inline bool operator<=(const X& lhs, const X& rhs){ return !(lhs > rhs); }
};

#endif
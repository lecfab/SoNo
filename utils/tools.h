#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files
#include <ctime>//to estimate the runing time
#include <string>
#include <chrono>

#include <vector>
#include <utility>
#include <list>

#ifndef DEBUG
#define Debug(msg) {}
#else
#define Debug(msg) { std::cout << "\tDebug: " << msg << endl; fflush(stdout); }
#endif
#define Alert(msg) { std::cout << "\tAlert: " << msg << endl; fflush(stdout); }
#define Info(msg) { std::cout << "\tInfo: " << msg << endl; fflush(stdout); }

#ifndef NLINKS
#define NLINKS 100000000
#define NNODES 1000000
#endif

#define TimeBegin() auto time_begin = std::chrono::steady_clock::now(); auto time_previous = time_begin, timeRec_previous = time_begin, time_now = time_begin; auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_previous).count();
#define TimeStep(m) std::cout << m; time_now = std::chrono::steady_clock::now(); print_time(time_previous, time_now); time_previous = time_now;
#define TimeTotal()	std::cout << "Total"; time_now = std::chrono::steady_clock::now(); print_time(time_begin, time_now);

// #define TimeRecStep(m) time_now = chrono::steady_clock::now(); std::cout << chrono::duration_cast<chrono::milliseconds>(time_now - time_previous).count() << "\t"; time_previous = time_now; Debug(m)
// #define TimeRecTotal() time_now = chrono::steady_clock::now(); std::cout << chrono::duration_cast<chrono::milliseconds>(time_now - time_begin).count() << endl;

#define TimeRecStep(m, out) time_now = std::chrono::steady_clock::now(); out << m << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - timeRec_previous).count() << endl; timeRec_previous = time_now;
#define TimeRecTotal(m, out) time_now = std::chrono::steady_clock::now(); out << m << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_begin).count() << endl;

// #define TimeRecBegin() vector<chrono::steady_clock::time_point> time_rec; time_rec.push_back(chrono::steady_clock::now());
// #define TimeRecStep(m) time_rec.push_back(chrono::steady_clock::now());
// #define TimeRecTotal() { auto t0 = time_rec[0], t1=t0; for(auto &t : time_rec) { std::cout << chrono::duration_cast<chrono::milliseconds>(t - t1).count() << ","; t1=t; } std::cout << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() << endl; }


void print_time(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end);

// https://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html
// template <typename Builder>
// auto makeAndProcessObject (const Builder& builder) -> decltype( builder.makeObject() );
// template <typename T>
// auto Timer (T &function, int argc, char** argv) -> decltype(function(argc, argv));


typedef unsigned long ul;
//compute the maximum of three unsigned long
inline ul max3(ul a, ul b, ul c) { return (a >= b and a >= c) ? a : ((b >= c) ? b : c); }

struct Keyvalue {
	ul key;
	ul val;
	Keyvalue(ul key, ul val) : key(key), val(val) {}
};

std::vector<ul> rank_from_order(const std::vector<ul> &order, const ul &n);

#endif

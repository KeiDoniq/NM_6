#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
#include <functional>
#include <vector>
using std::vector;
using std::string;
using std::map;

#include <utility>
using std::pair;

class diffSweep {
	string data;
	string rez;

	double (*p)(double);
	double (*q)(double);
	double (*f)(double);

	//double(*functions[3])(double,double,double,double);
	//in 
	double f1(double x, double y1, double y2, double y3) {
		return p(x) * y1 + y2;
	}
	double f2(double x, double y1, double y2, double y3) {
		return q(x) * y1;
	}
	double f3(double x, double y1, double y2, double y3) {
		return f(x) * y1;
	}
	//vector<double(*)(double, double, double, double)> functions;

	int Icod = 0;
	int N;
	double alpha_[2], beta_[2], gamma_[2];
	map<int,double> X; 
	
	double u, w, v;
	map<int, double> alpha, beta, gamma;

	void input();
	void solve();
	pair<double, double> solve_linear_system(int nodeNumber);
	string step_to_string(int nodeNumber, double y, double yDerivative) const;

public:
	diffSweep(string data, string rez,
		double (*p)(double), double (*q)(double), double (*f)(double)) :
		data(data), rez(rez), p(p), q(q), f(f) {
		try {
			input();
			solve();
		}
		catch (const std::invalid_argument& e) {
			std::ofstream fout(rez, std::ios_base::app);
			fout << e.what() << '\n';
			fout.close();
		}
	}
};
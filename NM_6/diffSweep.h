#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
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

	int Icod = 0;
	int N;
	double alpha_[2], beta_[2], gamma_[2];
	map<int,double> X; 
	
	double u, w, v;
	double alpha, beta, gamma;

	void input();
	void solve();
	pair<double, double> solve_linear_system(int nodeNumber);
	double calculate_RungeKutta(double (*function)(double, double), int nodeNumber, double y_0);
	double calculate_RungeKutta(double (*function)(double),	int nodeNumber, double y_0);
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
			std::ofstream fout(rez);
			fout << e.what() << '\n';
			fout.close();
		}
	}
};
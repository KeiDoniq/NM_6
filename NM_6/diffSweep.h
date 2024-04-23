#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <map>
using std::string;
using std::map;

#include <utility>
using std::pair;
//typedef std::pair<double, double> point;

class diffSweep {
	string data;
	string rez;

	double (*p)(double);
	double (*q)(double);
	double (*f)(double);

	int Icod = 0;
	int N;
	double A;// начальное значение - это все ранво, что нулевой узел Х??
	double B;// конечное ----------------||--------------, что last узел Х??
	double alpha_[2], beta_[2], gamma_[2];
	map<int,double> X; 
	//can it be vector?? can i say nodes number and index in vector are the same??

	double u, w, v;
	double alpha, beta, gamma;


	void input();
	void solve();
	pair<double, double> solve_systeme(int nodeNumber);
	double calculate_RungeKutta(double (*function)(double, double), int nodeNumber, double y_0);
	double calculate_Coshi(double (*function)(double, double), /*double x_0, */double y_0);//х_0 == А ?= X[0]??
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
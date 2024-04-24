#include "diffSweep.h"

void diffSweep::input()
{
	std::ifstream fin(data);
	double A, B;
	for (int i = 0; i < 2; ++i)
		fin >> alpha_[i] >> beta_[i] >> gamma_[i];
	fin >> A >> B >> N;

	int nodeNumber;
	double xValue;
	X.emplace(0, A);
	for (int i = 1; i < N; ++i) {
		fin >> nodeNumber >> xValue;
		X.emplace(nodeNumber, xValue);
	}
	X.emplace(N, B);

	fin.close();
}

double support_coef;

double linear_comb_2(double x, double y) {
	return support_coef * x + y;
}
double linear_comb_1(double x) {
	return support_coef * x;
}

void diffSweep::solve()
{
	for (int i = 1; i < N; ++i)
	{
		support_coef = p(X.at(i));
		u = calculate_RungeKutta(linear_comb_2, i, alpha_[0]);
		support_coef = q(X.at(i));
		v = calculate_RungeKutta(linear_comb_1, i, -beta_[0]);
		support_coef = f(X.at(i));
		w = calculate_RungeKutta(linear_comb_1, i, gamma_[0]);

		support_coef = p(X.at(i));
		alpha = calculate_RungeKutta(linear_comb_2, i, alpha_[1]);
		support_coef = q(X.at(i));
		beta = calculate_RungeKutta(linear_comb_1, i, -beta_[1]);
		support_coef = f(X.at(i));
		gamma = calculate_RungeKutta(linear_comb_1, i, gamma_[1]);
		/*
		* => на данном этапе получили систему
		 u			-v		=	w
		 alpha	    -beta	=  gamma
		*/

		//check if there is solution
		if (u * beta != (-v) * alpha) {
			pair<double, double> yValues = solve_linear_system(i);
			step_to_string(i, yValues.first, yValues.second);
		}
		else {
			Icod = 2; //или 1?
			throw std::invalid_argument("Система несовместна.");
		}
	}
}

pair<double, double> diffSweep::solve_linear_system(int nodeNumber)
{
	double det = 1 / (beta * u - v * alpha);
	double yD = det*(w * beta - v * gamma);
	double y = -det*(gamma * u - alpha * w);
	return pair<double, double>(y,yD);
}

double diffSweep::calculate_RungeKutta(double (*function)(double,double),
	int nodeNumber, double y_0)
{
	double h = abs(X.at(nodeNumber) - X.at(nodeNumber-1));

	double K1 = h * function(X.at(nodeNumber),y_0);
	double K2 = h * function(X.at(nodeNumber)+h, y_0 + K1);
	return y_0 + 0.5*(K1+K2);
}

double diffSweep::calculate_RungeKutta(double (*function)(double),
	int nodeNumber, double y_0)
{
	double h = abs(X.at(nodeNumber) - X.at(nodeNumber - 1));
	double K1 = function(X.at(nodeNumber));
	double K2 = function(X.at(nodeNumber)+h);
	return y_0 + 0.5 * h * (K1 + K2);
}

string diffSweep::step_to_string(int nodeNumber, double y, double yDerivative) const 
{
	std::stringstream ss;
	ss << std::setiosflags(std::ios::left);
	ss << std::setprecision(5)
		<< std::setw(10) << nodeNumber
		<< std::setw(40) << X.at(nodeNumber)
		<< std::setw(65) << y
		<< std::setw(65) << yDerivative << '\n';
	return ss.str();
}


#include "diffSweep.h"

void diffSweep::input()
{
	std::ifstream fin(data);

	for (int i = 0; i < 2; ++i)
		fin >> alpha_[i] >> beta_[i] >> gamma_[i];
	fin >> A >> B >> N;

	int nodeNumber;
	double xValue;
	for (int i = 0; i < N; ++i) {
		fin >> nodeNumber >> xValue;
		X.emplace(nodeNumber, xValue);
	}

	fin.close();
}

void diffSweep::solve()
{
	//u = calculate_Coshi(////p*u + v////,0, alpha_[0]);
	//v = calculate_Coshi(////q*u////,0, -beta_[0]);
	//w = calculate_Coshi(////f*u////,0, gamma_[0]);
	
	//alpha = calculate_Coshi(////p*alpha + beta////,0, alpha_[1]);
	//beta = calculate_Coshi(////q*alpha////,0, -beta_[1]);
	//gamma = calculate_Coshi(////f*alpha////,0, gamma_[1]);

	/*
	* => на данном этапе получили систему
	  u			-v		=	w
	  alpha	    -beta	=  gamma

	*/

	//check if there is solution
	if(u*beta != (-v)*alpha)
	{
		double y, yDerivative;
		for (int i = 0; i < N; ++i) {
			pair<double, double> yValues = solve_systeme(i);
			step_to_string(i, yValues.first, yValues.second);
		}
	}
	else {
		Icod = 2; //или 1?
		throw std::invalid_argument("Система несовместна.");
	}
}

pair<double, double> diffSweep::solve_systeme(int nodeNumber)
{
	//????
	/*
	  u			-v		=	w
	  alpha	    -beta	=  gamma
	*/

	return pair<double, double>();
}


double diffSweep::calculate_RungeKutta(double (*function)(double,double), int nodeNumber, double y_0)
{
	//who is h ?? откуда взять шаг? он постоянный? но сетка же не грантированно с равноотстоящими узлами.
	double h = 0.00001;//???	!tmp solution

	double K1 = h * function(X.at(nodeNumber),y_0);
	double K2 = h * function(X.at(nodeNumber), y_0 + K1);
	return y_0 + 0.5*(K1+K2);
}

double diffSweep::calculate_Coshi(double (*function)(double, double), /*double x_0, */double y_0)
{
	for (int i = 1; i < N; ++i)
		y_0 = calculate_RungeKutta(function, i, y_0);
	return y_0;
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

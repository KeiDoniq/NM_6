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
	X.emplace(N-1, B);

	fin.close();
}

void diffSweep::solve()
{
	std::ofstream fout("output.txt");
	double curr_u = alpha_[0];
	double curr_v = -beta_[0];
	double curr_w = gamma_[0];
	double curr_a = alpha_[1];
	double curr_b = -beta_[1];
	double curr_g = gamma_[1];
	int sign_h;

	auto Runge_Kutta = [this, &sign_h](int ind_x, double y1_0, double y2_0, double y3_0,
		double& y1, double& y2, double& y3)
	{
			//fyfkjubxyj gjvtyznm b vector<>y, vector<>y_0
		//todo:: если бы функции были в массиве указателей, то можно было бы пропустить циклом
		// и снизить число вычислений

		double K_1[3] = {};
		double K_2[3] = {};
		double x_0 = X.at(ind_x);
		double h = x_0 - X.at(ind_x - sign_h);

		K_1[0] = h * f1(x_0, y1_0, y2_0, y3_0);
		K_1[1] = h * f2(x_0, y1_0, y2_0, y3_0);
		K_1[2] = h * f3(x_0, y1_0, y2_0, y3_0);

		K_2[0] = h * f1(x_0 + h, y1_0 + K_1[0], y2_0 + K_1[1], y3_0 + K_1[2]);
		K_2[1] = h * f2(x_0 + h, y1_0 + K_1[0], y2_0 + K_1[1], y3_0 + K_1[2]);
		K_2[2] = h * f3(x_0 + h, y1_0 + K_1[0], y2_0 + K_1[1], y3_0 + K_1[2]);

		y1 = y1_0 + 0.5 * (K_1[0] + K_2[0]);
		y2 = y2_0 + 0.5 * (K_1[1] + K_2[1]);
		y3 = y3_0 + 0.5 * (K_1[2] + K_2[2]);
	};

	for (int i = 1; i < N; ++i)
	{
		sign_h = 1;
		Runge_Kutta(i, curr_u, curr_v, curr_w, u, v, w);
		sign_h = -1;
		Runge_Kutta((N-i-1), curr_a, curr_b, curr_g, alpha, beta, gamma);
		/*
		* => на данном этапе получили систему
		 u			-v		=	w
		 alpha	    -beta	=  gamma
		*/

		//check if there is solution
		if (u * beta != (-v) * alpha) {
			pair<double, double> yValues = solve_linear_system(i);
			string step = step_to_string(i, yValues.first, yValues.second);
			std::cout << step; //отладочная печать
			fout << step;
		}
		else {
			/*Icod = 1;
			throw std::invalid_argument("Система несовместна.");*/
			fout << "Icod = 1.\nСистема несовместна.";
		}

		curr_u = u;
		curr_v = v;
		curr_w = w;
		curr_a = alpha;
		curr_b = beta;
		curr_g = gamma;
	}
	fout.close();
}

pair<double, double> diffSweep::solve_linear_system(int nodeNumber)
{
	//добавить в отчет вывод этого
	double det = 1 / (beta * u - v * alpha);
	double yD = det*(w * beta - v * gamma);
	double y = -det*(gamma * u - alpha * w);
	return pair<double, double>(y,yD);
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


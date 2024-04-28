#include "diffSweep.h"

double p(double x) {
	return 1;
	//return 0;
}
double f(double x) {
	return 2+2*x+x*x;
	//return x * x;
}
double q(double x) {
	return 1;
}

int main() {
	diffSweep testDiff("data.txt", "output.txt", p, q, f);
	std::cin.get();
}

/*
генерация теста с уплотнением сетки в надежде на лучшее
	std::ofstream test("data2.txt");
	int k = 0;
	for (double i = 0.0; i <= 10; i += 0.1) {
		test << k++ << '\t' << i << '\n';
	}
	test.close();*/
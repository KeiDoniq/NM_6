#include "diffSweep.h"

double p(double x) {
	return 2 * x;
}
double f(double x) {
	return 0;
}
double q(double x) {
	return 10 * x * x;
}

int main() {
	diffSweep testDiff("data.txt", "output.txt", p, q, f);
	std::cin.get();
}
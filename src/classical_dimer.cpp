#include "DimerLattice2D.hpp"
#include <iostream>
#include <cmath>
void Test_DimerLattice(int loop) {
	DimerLattice2D DL(64, 64, 8, 1.0, exp(-4.0));
	//DL.print_configuration();
	for (int i = 0; i < loop; ++i) {
		DL.update_configuration();
	}
	//DL.print_configuration();

	std::cout << DL.check_degree() << std::endl;
	DL.print_corr();

	//DL.getd_ref(0, 0, 0) = true;
	//DL.print_configuration();
}
int main() {
	Test_DimerLattice(100);
	return 0;
}
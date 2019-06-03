#include "DimerLattice2D.hpp"
#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>

double w1 = default_w1, w2 = default_w2;
int Dx = default_dx, Dy = default_dy;
int L = default_L, loop = default_loop;
void Test_DimerLattice() {
	DimerLattice2D DL(L, L, w1, w2, Dx, Dy);
	//DL.print_configuration();
	for (int i = 0; i < loop; ++i) {
		//if (i == 84) {
		//	DL.debug = true;
		//}
		DL.update_configuration();
		if (!DL.check_degree()) {
			std::cerr << "error at " << i << "-th loop" << std::endl;
			DL.print_configuration();
			return ;
		}
		//else {
		//	DL.stdlog_file << "correct at " << i << "-th loop" << std::endl;
		//}
	}
	//DL.print_configuration();

	if (!DL.check_degree()) {
		DL.stdlog_file << "something wrong!!!!!" << std::endl;
	}
	DL.print_corr();
	unsigned end_time = (unsigned)time(NULL);
	DL.stdlog_file << "total time = " << end_time - DL.random_seed << " seconds." << std::endl;

	//DL.getd_ref(0, 0, 0) = true;
	//DL.print_configuration();
}
std::string to_str(char *argv)
{
	std::string s = "";
	for (int i = 0; argv[i] != '\0'; ++i)
		s.push_back(argv[i]);
	return s;
}
void load_arg(char *argv)
{
	char *equ = strchr(argv, '=');
	equ[0] = '\0';
	std::string control = to_str(argv);
	if (control == "L") {
		L = atoi(equ + 1);
	}
	if (control == "loop") {
		loop = atoi(equ + 1);
	}
	if (control == "w1") {
		w1 = atof(equ + 1);
	}
	if (control == "w2") {
		w2 = atof(equ + 1);
	}
	if (control == "dx") {
		Dx = atoi(equ + 1);
	}
	if (control == "dy") {
		Dy = atoi(equ + 1);
	}
	if (control == "logw1") {
		w1 = exp(atof(equ + 1));
	}
	if (control == "logw2") {
		w2 = exp(atof(equ + 1));
	}

}
void load_args(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
		load_arg(argv[i]);
}
int main(int argc, char **argv) {
	load_args(argc, argv);
	Test_DimerLattice();
	return 0;
}
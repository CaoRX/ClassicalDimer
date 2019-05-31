#include "DimerLattice2D.hpp"
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdlib>

int modify_int(int x, int range) {
	return (x + range) % range;
}

double DimerLattice2D::random_double() {
	return Random_double(random_engine);
}
int DimerLattice2D::random_int(int range) {
	return rand() % range;
}
void DimerLattice2D::malloc_space() {
	dimer = new bool***[W];
	for (int x = 0; x < W; ++x) {
		dimer[x] = new bool**[H];
		for (int y = 0; y < H; ++y) {
			dimer[x][y] = new bool*[D];
			for (int dir = 0; dir < D / 2; ++dir) {
				dimer[x][y][dir] = new bool;
				*dimer[x][y][dir] = false;
			}
		}
	}
	int lx = -1, ly = -1;
	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			for (int dir = D / 2; dir < D; ++dir) {
				lx = modify_int(x + dx[dir], W);
				ly = modify_int(y + dy[dir], H);
				dimer[x][y][dir] = dimer[lx][ly][dir - (D / 2)];
			}
		}
	}
	M = new double[W];
	for (int i = 0; i < W; ++i) {
		M[i] = 0.0;
	}
}
void DimerLattice2D::set_initial_state() {
	for (int x = 0; x < W; ++x) {
		if (x % 2 == 0) {
			continue;
		}
		for (int y = 0; y < H; ++y) {
			*dimer[x][y][0] = true;
		}
	}
}
void DimerLattice2D::set_initial_values() {
	if (w1 > w2) {
		weight[0][0] = (w1 - 4.0 * w2 / 7.0) / 3.0;
		weight[0][1] = weight[1][0] = weight[1][1] = w2 / 7.0;
	}
	else {
		weight[1][1] = (w2 - 4.0 * w1 / 7.0) / 3.0;
		weight[0][0] = weight[0][1] = weight[1][0] = w1 / 7.0;
	}

	if (fabs(w2) > eps) {
		weight[1][0] /= w2; weight[1][1] /= w2;
	}
	if (fabs(w1) > eps) {
		weight[0][0] /= w1; weight[0][1] /= w1;
	}

	//std::cout << weight[0][0] << ' ' << weight[0][1] << std::endl;
	//std::cout << weight[1][0] << ' ' << weight[1][1] << std::endl;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			defect[i][j] = -1;
		}
	}
	dir_in = -1;
	debug = false;
}
void DimerLattice2D::set_random() {
	random_seed = (unsigned)time(NULL);
	random_engine = std::default_random_engine(random_seed);
	Random_double = std::uniform_real_distribution<double>(0.0, 1.0);
	srand(random_seed);
}

DimerLattice2D::DimerLattice2D(int _H, int _W, int _D, double _w1, double _w2): H(_H), W(_W), D(_D), w1(_w1), w2(_w2) {
	malloc_space();
	set_initial_state();
	set_initial_values();
	set_random();
}

bool & DimerLattice2D::getd_ref(int x, int y, int dir) {
	x = modify_int(x, W);
	y = modify_int(y, H);
	dir = modify_int(dir, D);
	return *dimer[x][y][dir];
}
bool DimerLattice2D::getd_val(int x, int y, int dir) {
	x = modify_int(x, W);
	y = modify_int(y, H);
	dir = modify_int(dir, D);
	return *dimer[x][y][dir];
}
void DimerLattice2D::setd(int x, int y, int dir, bool d_val) {
	x = modify_int(x, W);
	y = modify_int(y, H);
	dir = modify_int(dir, D);
	*dimer[x][y][dir] = d_val;
}
void DimerLattice2D::print_configuration() {
	std::cout << "--------------------------------------------------------" << std::endl;
	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			for (int dir = 0; dir < D; ++dir) {
				std::cout << getd_val(x, y, dir) << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << "--------------------------------------------------------" << std::endl;
}
int DimerLattice2D::find_exit(int x, int y) {
	for (int dir = 0; dir < D; ++dir) {
		if (getd_val(x, y, dir)) {
			return dir;
		}
	}
	return -1;
}
void DimerLattice2D::make_defect() {
	int dfx = random_int(W), dfy = random_int(H);
	defect[0][0] = dfx; defect[0][1] = dfy;
	int exit_n = find_exit(dfx, dfy);
	int dfex = modify_int(dfx + dx[exit_n], W), dfey = modify_int(dfy + dy[exit_n], H);
	defect[1][0] = dfex; defect[1][1] = dfey;

	dir_in = exit_n;

	setd(dfx, dfy, exit_n, false);
}

int DimerLattice2D::choose_dir_out(int dir_in) {
	double choice = random_double();
	double cur = 0.0;
	for (int dir = 0; dir < D; ++dir) {
		if (dir != dir_in) {
			cur += weight[(dir_in % 2)][dir % 2];
			//std::cout << cur << std::endl;
			if (cur >= choice) {
				return dir;
			}
		}
	}
	return -1;
}
bool DimerLattice2D::move_defect() {
	//defect 1 will not move
	//defect 0 will move until meet 1
	int dir_out = choose_dir_out(dir_in);
	//if (dir_out == -1) {
	//	std::cout << "Error: cannot choose direction out" << std::endl;
	//}
	int dfx_ext = modify_int(defect[0][0] + dx[dir_out], W);
	int dfy_ext = modify_int(defect[0][1] + dy[dir_out], H);

	//std::cout << "dfx_ext = " << dfx_ext << " dfy_ext = " << dfy_ext << std::endl;
	//std::cout << "dir_out = " << dir_out << std::endl;

	//setd(defect[0][0], defect[0][1], dir_out, true);
	if (dfx_ext == defect[1][0] && dfy_ext == defect[1][1]) {
		setd(defect[0][0], defect[0][1], dir_out, true);
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				defect[i][j] = -1;
			}
		}
		return true;
	}

	int exit_n = find_exit(dfx_ext, dfy_ext);
	setd(defect[0][0], defect[0][1], dir_out, true);
	setd(dfx_ext, dfy_ext, exit_n, false);
	defect[0][0] = modify_int(dfx_ext + dx[exit_n], W);
	defect[0][1] = modify_int(dfy_ext + dy[exit_n], H);
	return false;
}
void DimerLattice2D::print_defect() {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			std::cout << defect[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}
void DimerLattice2D::measure_corr() {
	if (defect[1][1] == defect[0][1]) {
		M[modify_int(defect[1][0] - defect[0][0], W)] += 1.0;
	}
}
void DimerLattice2D::update_configuration() {
	make_defect();
	//print_defect();
	while (!move_defect()) {
		measure_corr();
		//if (debug) {
		//	print_defect();
		//	print_configuration();
		//}
	}
}

bool DimerLattice2D::check_degree(int degree) {
	int local_degree = 0;
	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			local_degree = 0;
			for (int dir = 0; dir < D; ++dir) {
				local_degree += getd_val(x, y, dir);
			}
			if (local_degree != degree) {
				return false;
			}
		}
	}
	return true;
}
void DimerLattice2D::print_corr() {
	double normal_m = M[1];
	for (int i = 0; i < W; ++i) {
		M[i] /= normal_m;
		std::cout << M[i] << ' ';
	}
	std::cout << std::endl;
}
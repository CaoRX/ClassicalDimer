#include "DimerLattice2D.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

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
				dimer[x][y][dir * 2] = new bool;
				*dimer[x][y][dir * 2] = false;
			}
		}
	}

	/*if_in_set = new bool**[W];
	for (int x = 0; x < W; ++x) {
		if_in_set[x] = new bool*[H];
		for (int y = 0; y < H; ++y) {
			if_in_set[x][y] = new bool[SET_TYPE];
			for (int set_no = 0; set_no < SET_TYPE; ++set_no) {
				if_in_set[x][y][set_no] = false;
			}
		}
	}*/
	int lx = -1, ly = -1;
	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			for (int dir = 0; dir < D / 2; ++dir) {
				lx = modify_int(x + local_dx[dir * 2], W);
				ly = modify_int(y + local_dy[dir * 2], H);
				dimer[lx][ly][dir * 2 + 1] = dimer[x][y][dir * 2];
				//stdlog_file << x << ' ' << y << ' ' << (dir * 2) << ' ' << lx << ' ' << ly << ' ' << (dir * 2 + 1) << std::endl;
			}
		}
	}
	M = new double[W];
	for (int i = 0; i < W; ++i) {
		M[i] = 0.0;
	}/*
	corrD = new double*[SET_TYPE];
	for (int i = 0; i < SET_TYPE; ++i) {
		corrD[i] = new double[W];
		for (int j = 0; j < W; ++j) {
			corrD[i][j] = 0.0;
		}
	}*/
	update_size = new int[loop];
	for (int i = 0; i < loop; ++i) {
		update_size[i] = 0;
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
void DimerLattice2D::print_log(std::fstream &logfile) {
	logfile << "H = " << H << std::endl;
	logfile << "W = " << W << std::endl;
	logfile << "loop = " << loop << std::endl;
	logfile << "equil loop = " << equil_loop << std::endl;
	//logfile << "(dx, dy) = (" << edx << ", " << edy << ")" << std::endl;
	//logfile << "D = " << D << std::endl;
	logfile << "random seed = " << random_seed << std::endl;
}
void DimerLattice2D::set_initial_values() {
	/*if (w1 > w2) {
		weight[0][0] = (w1 - (D - 4) * w2 / (D - 1)) / (4 - 1);
		weight[0][1] = weight[1][0] = weight[1][1] = w2 / (D - 1);
	}
	else {
		weight[1][1] = (w2 - 4 * w1 / (D - 1)) / (D - 4 - 1);
		weight[0][0] = weight[0][1] = weight[1][0] = w1 / (D - 1);
	}

	if (fabs(w2) > eps) {
		weight[1][0] /= w2; weight[1][1] /= w2;
	}
	if (fabs(w1) > eps) {
		weight[0][0] /= w1; weight[0][1] /= w1;
	}*/

	//stdlog_file << weight[0][0] << ' ' << weight[0][1] << std::endl;
	//stdlog_file << weight[1][0] << ' ' << weight[1][1] << std::endl;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			defect[i][j] = -1;
		}
	}
	dir_in = -1;
	debug = false;
	stdlog_filename = "data/log" + std::to_string(random_seed) + ".dat";
	stdlog_file.open(stdlog_filename, std::ios::out);
	corrM_filename = "data/corrM" + std::to_string(random_seed) + ".dat";
	corrM_file.open(corrM_filename, std::ios::out | std::ios::binary);
	update_filename = "data/update" + std::to_string(random_seed) + ".dat";
	update_file.open(update_filename, std::ios::out | std::ios::binary);
	/*
	stdlog_filename = "data/log" + std::to_string(random_seed) + ".dat";
	stdlog_file.open(stdlog_filename, std::ios::out);
	corrD_filename = "data/corrD" + std::to_string(random_seed) + ".dat";
	corrD_file.open(corrD_filename, std::ios::out | std::ios::binary);
*/

	//update_size.clear();

	equil_loop = int(equil * loop);
	now_loop = 0;
	print_log(stdlog_file);
}
void DimerLattice2D::set_random() {
	random_seed = (unsigned)time(NULL);
	random_engine = std::default_random_engine(random_seed);
	Random_double = std::uniform_real_distribution<double>(0.0, 1.0);
	srand(random_seed);
}
/*void DimerLattice2D::set_dd() {
	D = ((edx == edy) || (edx == 0) || (edy == 0)) ? (4 + 4) : (4 + 8);
	local_dx = new int[D];
	local_dy = new int[D];
	local_dc = new int[D];
	for (int i = 0; i < 4; ++i) {
		local_dx[i] = sqr_dx[i];
		local_dy[i] = sqr_dy[i];
		local_dc[i] = 0;
	}
	for (int i = 4; i < D; ++i) {
		local_dc[i] = 1;
	}
	int dcur = 4;
	if (edx == edy) {
		local_dx[dcur] = edx; local_dy[dcur] = edy; ++dcur;
		local_dx[dcur] = -edx; local_dy[dcur] = -edy; ++dcur;
		local_dx[dcur] = -edx; local_dy[dcur] = edy; ++dcur;
		local_dx[dcur] = edx; local_dy[dcur] = -edy; ++dcur;
		return ;
	}
	if (edx == 0 || edy == 0) {
		local_dx[dcur] = edx; local_dy[dcur] = edy; ++dcur;
		local_dx[dcur] = -edx; local_dy[dcur] = -edy; ++dcur;
		local_dx[dcur] = edy; local_dy[dcur] = edx; ++dcur;
		local_dx[dcur] = -edy; local_dy[dcur] = -edx; ++dcur;
		return ;
	}

	local_dx[dcur] = edx; local_dy[dcur] = edy; ++dcur;
	local_dx[dcur] = -edx; local_dy[dcur] = -edy; ++dcur;
	local_dx[dcur] = -edx; local_dy[dcur] = edy; ++dcur;
	local_dx[dcur] = edx; local_dy[dcur] = -edy; ++dcur;

	std::swap(edx, edy);

	local_dx[dcur] = edx; local_dy[dcur] = edy; ++dcur;
	local_dx[dcur] = -edx; local_dy[dcur] = -edy; ++dcur;
	local_dx[dcur] = -edx; local_dy[dcur] = edy; ++dcur;
	local_dx[dcur] = edx; local_dy[dcur] = -edy; ++dcur;

	std::swap(edx, edy);

}*/
void DimerLattice2D::set_dd() {
	D = DIR_NUM;
	local_dx = new int[D];
	local_dy = new int[D];
	for (int i = 0; i < D; ++i) {
		local_dx[i] = sqr_dx[i];
		local_dy[i] = sqr_dy[i];
	}
}

DimerLattice2D::DimerLattice2D(int _H, int _W, double _w1, double _w2, int _edx, int _edy, int _loop, double _equil): H(_H), W(_W), w1(_w1), w2(_w2), edx(_edx), edy(_edy), loop(_loop), equil(_equil) {
	std::cout << "Begin initialization" << std::endl;
	set_random();
	std::cout << "Finish setting random" << std::endl;
	set_dd();
	std::cout << "Finish setting dd" << std::endl;
	malloc_space();
	set_initial_state();
	set_initial_values();
	std::cout << "Finish initialization" << std::endl;

	//for (int i = 0; i < D; ++i) {
	//	stdlog_file << local_dx[i] << ' ' << local_dy[i] << ' ' << local_dc[i] << std::endl;
	//}
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
	stdlog_file << "--------------------------------------------------------" << std::endl;
	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			for (int dir = 0; dir < D; ++dir) {
				stdlog_file << getd_val(x, y, dir) << ' ';
			}
			stdlog_file << std::endl;
		}
		stdlog_file << std::endl;
	}
	stdlog_file << "--------------------------------------------------------" << std::endl;
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
	int dfex = modify_int(dfx + local_dx[exit_n], W), dfey = modify_int(dfy + local_dy[exit_n], H);
	defect[1][0] = dfex; defect[1][1] = dfey;

	dir_in = exit_n;

	setd(dfx, dfy, exit_n, false);
}

int DimerLattice2D::choose_dir_out(int dir_in) {
	int choice = random_int(DIR_NUM - 1);
	for (int dir = 0; dir < D; ++dir) {
		if (dir != dir_in) {
			//cur += weight[local_dc[dir_in]][local_dc[dir]];
			--choice;
			//stdlog_file << cur << std::endl;
			if (choice == -1) {
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
	//	stdlog_file << "Error: cannot choose direction out" << std::endl;
	//}
	int dfx_ext = modify_int(defect[0][0] + local_dx[dir_out], W);
	int dfy_ext = modify_int(defect[0][1] + local_dy[dir_out], H);

	//stdlog_file << "dfx_ext = " << dfx_ext << " dfy_ext = " << dfy_ext << std::endl;
	//stdlog_file << "dir_out = " << dir_out << std::endl;

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
	defect[0][0] = modify_int(dfx_ext + local_dx[exit_n], W);
	defect[0][1] = modify_int(dfy_ext + local_dy[exit_n], H);
	dir_in = (exit_n ^ 1);
	return false;
}
void DimerLattice2D::print_defect() {
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			stdlog_file << defect[i][j] << ' ';
		}
		stdlog_file << std::endl;
	}
}
void DimerLattice2D::measure_corrM() {
	if (defect[1][1] == defect[0][1]) {
		M[modify_int(defect[1][0] - defect[0][0], W)] += 1.0;
	}
}
/*void DimerLattice2D::calculate_set() {
	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			for (int set_no = 0; set_no < SET_TYPE; ++set_no) {
				if_in_set[x][y][set_no] = is_in_set(x, y, set_no);
			}
		}
	}
}
void DimerLattice2D::measure_corrD() {
	//calculate_set();
	for (int dx = 0; dx < W; ++dx) {
		for (int x = 0; x < 2; ++x) {
			for (int y = 0; y < H; ++y) {
				for (int set_no = 0; set_no < SET_TYPE; ++set_no) {
					if (is_in_set(x, y, set_no) && is_in_set(x + dx, y, set_no)) {
						corrD[set_no][dx] += 1.0;
					}
				}
			}
		}
	}
}*/
void DimerLattice2D::update_configuration() {
	make_defect();
	update_size[now_loop] = 2;
	//print_defect();
	while (!move_defect()) {
		update_size[now_loop] += 2;
		if (now_loop >= equil_loop) {
			measure_corrM();
		}
		//if (debug) {
		//	print_defect();
		//	print_configuration();
		//}
	}
	//if (now_loop >= equil_loop) {
	//	measure_corrD();
	//}
	//std::cout << update_size[now_loop] << std::endl;
	++now_loop;
	//update_size.push_back(update_length);
	//std::cout << update_length << std::endl;
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
/*void DimerLattice2D::Normalize_corr() {
	double norm_m = M[1];
	for (int i = 0; i < W; ++i) {
		M[i] /= norm_m;
	}
	double norm_d = 1.0;
	for (int set_no = 0; set_no < SET_TYPE; ++set_no) {
		norm_d = corrD[set_no][0];
		for (int i = 0; i < W; ++i) {
			corrD[set_no][i] /= norm_d;
		}
	}
}*/
void DimerLattice2D::print_corr() {
	//double normal_m = M[1];
	//for (int i = 0; i < W; ++i) {
	//	M[i] /= normal_m;
	//	stdlog_file << M[i] << ' ';
	//}

	//stdlog_file << std::endl;
	//Normalize_corr();
	//for (int set_no = 0; set_no < SET_TYPE; ++set_no) {
	//	corrD_file.write((char *)corrD[set_no], sizeof(double) * W);
	//}
	//stdlog_file << "data file no = " << random_seed << std::endl;
	corrM_file.write((char *)M, sizeof(double) * W);
	update_file.write((char *)update_size, sizeof(int) * loop);
}

/*bool DimerLattice2D::is_in_set(int x, int y, int set_type) {
	x = modify_int(x, W);
	y = modify_int(y, H);
	int exit_n = find_exit(x, y);
	if (set_type == SET1) {
		//return exit_n < 2;
		return (exit_n == 0);
	}
	if (set_type == SET2) {
		//return (exit_n == 4) || (exit_n == 5);
		return (exit_n == 4);
	}
	if (set_type == SETA) {
		return (exit_n < 4) && (exit_n & 1);
	}
	if (set_type == SETB) {
		return (exit_n >= 4) && (exit_n & 1);
	}
	return false;
}
bool DimerLattice2D::is_in_set_direct(int x, int y, int set_type) {
	x = modify_int(x, W);
	y = modify_int(y, H);
	return if_in_set[x][y][set_type];
}*/

DimerLattice2D::~DimerLattice2D() {
	corrM_file.close();
}
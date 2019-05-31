#pragma once

#include <random>
#include <cmath>
#define eps 1e-9

const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

//const int dx[12] = {1, 2, 1, 0, -1, -2, -1, -2, -1, 0, 1, 2};
//const int dy[12] = {0, 1, 2, 1, 2, 1, 0, -1, -2, -1, -2, -1, 0};

class DimerLattice2D {
public:
	int H, W, D;
	bool ****dimer;
	double w1, w2;
	int defect[2][2];
	double weight[2][2];
	unsigned random_seed;
	double *M;
	std::default_random_engine random_engine;
	std::uniform_real_distribution<double> Random_double;
	int dir_in;

	DimerLattice2D(int _H, int _W, int _D = 8, double _w1 = 1.0, double _w2 = 0.0);
	void malloc_space();
	void set_initial_state();
	void set_initial_values();
	void set_random();

	double random_double();
	int random_int(int range);

	bool & getd_ref(int x, int y, int dir);
	bool getd_val(int x, int y, int dir);
	void setd(int x, int y, int dir, bool d_val);
	int find_exit(int x, int y);

	void make_defect();
	bool move_defect();

	void print_configuration();
	void print_defect();
	int choose_dir_out(int dir_in);

	void update_configuration();
	void measure_corr();

	bool check_degree(int degree = 1);
	void print_corr();


};
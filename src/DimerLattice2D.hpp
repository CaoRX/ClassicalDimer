#pragma once

#include <random>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#define eps 1e-9

#define SET1 0
#define SET2 1
#define SETA 2
#define SETB 3

#define SET_TYPE 4

const int dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
const int dy[8] = {0, 0, 1, -1, -1, 1, 1, -1};
const int dc[8] = {0, 0, 0, 0, 1, 1, 1, 1};

const int sqr_dx[4] = {1, -1, 0, 0};
const int sqr_dy[4] = {0, 0, 1, -1};

//const int dx[12] = {1, 2, 1, 0, -1, -2, -1, -2, -1, 0, 1, 2};
//const int dy[12] = {0, 1, 2, 1, 2, 1, 0, -1, -2, -1, -2, -1, 0};

const double default_w1 = 1.0, default_w2 = 0.0;
const int default_L = 8, default_loop = 128;
const double default_equil = 0.03;
const int default_dx = 1, default_dy = 1;

class DimerLattice2D {
public:
	int H, W, D, d1, d2;
	bool ****dimer;
	int edx, edy;
	std::vector<int> dxx, dyy;
	double w1, w2;
	int defect[2][2];
	double weight[2][2];
	unsigned random_seed;
	double *M;
	double **corrD;
	std::default_random_engine random_engine;
	std::uniform_real_distribution<double> Random_double;
	int dir_in;
	bool debug;
	std::fstream corrM_file, stdlog_file, corrD_file;
	std::string corrM_filename, stdlog_filename, corrD_filename;

	int loop, now_loop, equil_loop;
	double equil;
	int *local_dx, *local_dy, *local_dc;


	DimerLattice2D(int _H, int _W, double _w1 = 1.0, double _w2 = 0.0, int _edx = 1, int _edy = 1, int _loop = default_loop, double _equil = default_equil);
	void malloc_space();
	void set_initial_state();
	void set_initial_values();
	void set_random();
	void set_dd();
	void print_log(std::fstream &logfile);

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
	void measure_corrD();
	void measure_corrM();

	bool check_degree(int degree = 1);
	void Normalize_corr();
	void print_corr();

	bool is_in_set(int x, int y, int set_type);

	~DimerLattice2D();

};
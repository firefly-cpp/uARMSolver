#ifndef SETUP_H_
#define SETUP_H_

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

#define	SOLVER_NONE		0
#define	SOLVER_DE		1
#define	SOLVER_PSO		2

#define VISUAL_NONE		0
#define VISUAL_FLOW		1
#define VISUAL_METRO	2

#define SQUASH_NONE		0
#define SQUASH_CAUCHY	1
#define SQUASH_EUCLID	2

using namespace std;

	// algorithm's setups
	struct de_param {
		double 	scale;
		double	xover;
		int strategy;
	};

	// visualization's setups
	struct flow_param {
		int 	M;
		int		N;
	};

	// squashing setups
	struct squashing_param {
		double 	Thresh;
	};

/**
 * Definition of setup parameter input file
 *
 * The class is defined for manipulation of input parameters saved
 * with which a behavior of the solver is controlled. These parameters
 * are saved into an input file that is divided into three parts:
 * problem definition, parameter setting of algorithm dedicated for
 * ARM, and parameter setting of visualization method dedicated for
 * preparing data for visualization. The third part is not implemented
 * yet.
 *
 */
class Setup {
public:
	Setup(string fname);
	virtual ~Setup();

	bool read();
	void token_line(string line, vector<string> &tokens);
	void parse_prob_blk(vector<string>tokens);
	void parse_alg_blk(vector<string>tokens);
	void parse_alg_de(vector<string>tokens);
	void parse_alg_pso(vector<string>tokens);
	int alg_to_int(string alg);
	void parse_vis_blk(vector<string>tokens);
	void parse_vis_flow(vector<string>tokens);
	int vis_to_int(string vis);
	void parse_sq_blk(vector<string>tokens);
	void parse_sq_cauchy(vector<string>tokens);
	void parse_sq_euclid(vector<string>tokens);
	int sq_to_int(string sq);

	void print_tokens(vector<string>tokens);
	void print_prob_blk();
	void print_alg_blk();
	void print_vis_blk();
	void print_sq_blk();
	void print_param();

	// getters
	int get_solver() { return solver; }
	int get_period() { return period; }
	int get_squash() { return squash; }
	int get_Np() { return Np; }
	int get_FEs() { return FEs; }
	int get_RUNs() { return RUNs; }
	string get_tdbase_name() { return tdbase_name; }
	string get_sq_dbase_name() { return sq_dbase_name; }
	string get_rule_name() { return rule_name; }
	string get_out_name() { return out_name; }

	// setters
	void set_solver(int val) { solver = val; }
	void set_period(int val) { period = val; }
	void set_squash(int val) { squash = val; }
	void set_Np(int val) { Np = val; }
	void set_FEs(int val) { FEs = val; }
	void set_RUNs(int val) { RUNs = val; }
	void set_tdbase_name(string str) { tdbase_name = str; }
	void set_rule_name(string str) { rule_name = str; }
	void set_out_name(string str) { out_name = str; }

private:
	int 	period;			///< number of transaction databases & archives
	int		solver;			///< algorithm for solving ARM
	int		visual;			///< visualization method
	int		squash;			///< squashing method
	int 	Np;				///< population size
	int		FEs;			///< number of fitness function evaluations
	int		RUNs;			///< number of independent runs

	string	arm_set_name;	///< ARM setup file name
	string 	tdbase_name;	///< transaction database
	string 	sq_dbase_name;	///< squashed database
	string 	rule_name;		///< rule database
	string 	out_name;		///< output file name

public:
	union {
		de_param de;
	} alg_param;			///< algorithm parameters

	union {
		flow_param flow;
	} vis_param;			///< visualization parameters

	union {
		squashing_param sq;
	} squ_param;				///< squashing parameters

};

#endif /* SETUP_H_ */

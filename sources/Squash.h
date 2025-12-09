/*
 * Squash.h
 *
 *  Created on: Mar 10, 2022
 *      Author: iztok
 */

#ifndef SOURCE_SQUASH_H_
#define SOURCE_SQUASH_H_

#include "Problem.h"
#include "Setup.h"


/**
 * Squashing the transaction database class
 *
 *
 */

class Squash {
public:
	Squash();
	virtual ~Squash();

	int make_sq_dbase(Setup setup, Problem prob);
	void init_squashed(int n);
	bool is_squashed(int pos) { return sq_squashed[pos]; }
	void set_squashed(int pos) { sq_squashed[pos] = true; }
	void reset_squashed(int pos) { sq_squashed[pos] = false; }
	double sq_similarity(vector<Attribute> vec_1, vector<Attribute> vec_2);
	double sq_distance(vector<Attribute> vec_1, vector<Attribute> vec_2, vector<Feature> feat);
	void make_sq_vector(int sq_set_name, vector<Attribute> &sq_vec, int n_attr);
	int in_similar_set(vector<string> set_str, string str);
	int find_max_count(vector<int> count);
	void stat(Problem prob);
	void write(Setup setup);
	void print_vec(vector<Attribute> vec);
	void print_vec(vector<bool> vec);
	void print_vec(vector<int> vec);
	void print_vec(vector<string> vec);
	void print();

public:
	vector< vector<Attribute> > sq_dbase;	///< squashing transaction database represented as a collection of attributes
	vector< vector<Attribute> > sq_set;		///< squashing set database represented as a collection of attributes
	vector< bool > sq_squashed;				///< indicate if the transaction is already squashed
};

#endif /* SOURCE_SQUASH_H_ */

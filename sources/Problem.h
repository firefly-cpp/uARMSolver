#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <utility>
#include <bits/stdc++.h>

#include "Feature.h"
#include "Rule.h"
#include "Setup.h"

#define uint unsigned int

using namespace std;

/**
 * Problem definition class
 *
 * The class enable reading, parsing, building, and manipulating
 * with a transaction database. As a result, the problem is
 * represented with a list of features and a transaction database.
 * The transaction database is a matrix with rows and columns, where
 * each row represents a particular transaction and column the
 * corresponding feature. However, elements of the matrix identify
 * attributes.
 *
 */
class Problem {
public:
	Problem();
	~Problem(void);

	bool init_tdbase(Setup setup, string tfile_name);
	void token_line(string line, vector<string> &tokensm, char ch);
	int analyze_token(string token, int n_feat, int n_tran);
	bool containsFloatingPoint(string str);
	bool containsOnlyDigit(string str);
	void new_feat(string token);
	uint new_tran();
	double calc_support(Rule rule);
	double calc_confidence(Rule rule);
	void make_pair(int i, int j, string &attr);
	bool isIncluded(int i, int j, Rule rule);
	void disc_attr();

	void print_hash();
	void print_tokens(vector<string>tokens);
	void print_feat();
	void print_attr();
	void print_hash(vector<string> hash);

	int get_dimension() { return feat.size(); }

public:
	vector<Feature> feat;				///< list of features
	vector< vector<Attribute> > dbase;	///< transaction database represented as a collection of attributes
};

#endif /* PROBLEM_H_ */

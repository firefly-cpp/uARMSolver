#ifndef SOURCES_RULE_H_
#define SOURCES_RULE_H_

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

#include "Feature.h"

using namespace std;

/**
 * Definition of mined association rule
 *
 * The class is defined for manipulation of the association
 * rule. Each association rule is identified by its characteristics
 * measures, like: fitness, support, confidence, and inclusion. In
 * addition, an antecedent and consequent of <feature,attribute>
 * pairs are represented as a sets of strings, while vectors needed
 * for genotype/phenotype mapping as integer or floating-point values.
 *
 */
class Rule {
public:
	Rule();
	virtual ~Rule();

	void init(int i, double x);
	int find_feat(vector<Feature>feat, string item);
	double calc_inclusion();
	void sort();
	bool isValid();
	void print();	// log output format
	void write(ofstream &out);	// standard format

	double get_fitness() { return fitness; }
	double get_support() { return support; }
	double get_confidence() { return confidence; }
	double get_inclusion() { return inclusion; }
	void set_fitness(double fit) { fitness = fit; }
	void set_support(double supp) { support = supp; }
	void set_confidence(double conf) { confidence = conf; }
	void set_inclusion(double incl) { inclusion = incl; }

private:
	double fitness;			///< fitness of the association rule
	double support;			///< support of the association rule
	double confidence;		///< confidence of the association rule
	double inclusion;		///< inclusion of the association rule
public:
	set <string> ante;		///< antecedent part of the association rule
	set <string> cons;		///< consequent part of the association rule

	vector <int> norm;		///< normalized vector
	vector <int> perm;		///< permutation vector
	vector <double> val;	///< floating-point vector, which elements determine an ordering of features in permutation
};

#endif /* SOURCES_RULE_H_ */

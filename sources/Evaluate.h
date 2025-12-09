#ifndef SOURCES_EVALUATE_H_
#define SOURCES_EVALUATE_H_

#include <vector>

#include "Archive.h"
#include "Problem.h"
#include "Setup.h"

using namespace std;

/**
 * Definition of the class for fitness function evaluation
 *
 * The unique class for utilizing fitness function evaluation that is
 * available for all the used algorithmic solvers. This class implements
 * a genotype/phenotype mapping dedicated for transforming the real-valued
 * vector into the solution in problem space. In line with this, functions
 * encode() and decode() are used, where the first helps us by encoding the
 * solution in problem space into real-valued vector in genotype space, and
 * the second operates in the opposite direction. The EnergyFunction() evaluates
 * the quality of solution in the problem space.
 *
 */
class Evaluate {
public:
	Evaluate();
	~Evaluate();

	double EnergyFunction(vector<double> x, Problem prob, Rule &rule, int intervals);
	int encode(int D, Problem prob);
	void decode(vector<double> x, vector<Feature> feat, Rule &rule, int intervals);
	void sort(vector<int>&index, vector<double>&val);

	void print_vec(string str, vector<int>vec);
	void print_vec(string str, vector<double>vec);

public:
	vector<int> pointers;		///< maps features into positions within real-valued vector
};


#endif /* SOURCES_EVALUATE_H_ */

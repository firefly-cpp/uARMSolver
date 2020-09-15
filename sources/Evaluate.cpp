#include "Evaluate.h"

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>

#define random01()	(rand()/(double)RAND_MAX)

Evaluate::Evaluate()
{
	// TODO Auto-generated constructor stub
	srand(time(NULL));
}

Evaluate::~Evaluate()
{
	// TODO Auto-generated destructor stub
}

/**
 * Implementation of the fitness function evaluation.
 *
 * @param the trial vector, the problem definition, and the mined association rule.
 * @return double return code: the calculated fitness value.
 */
double Evaluate::EnergyFunction(vector<double> x, Problem prob, Rule &rule)
{
	double result = 0;

	decode(x, prob.feat, rule);

	if(rule.isValid()) {
		rule.set_support(prob.calc_support(rule));
		if(rule.get_support() > 0) {
			rule.set_confidence(prob.calc_confidence(rule));
			rule.set_inclusion(rule.calc_inclusion());
			result = (rule.get_support()+rule.get_confidence()+rule.get_inclusion())/3.0;
			rule.set_fitness(result);
		}
	}

	return result;
}

/**
 * Encode the features into proper positions within the real-valued vector.
 *
 * @param dimension of the problem, and a definition of the problem.
 * @return integer return code: the length of the real-valued vector encoded within genotype space.
 */
int Evaluate::encode(int D, Problem prob)
{
	// Categorical attributes: <x_i = Permutation, x_{i+1} in Set, x_{i+2} = Threshold>
	// Numerical attributes:   <x_i = Permutation, x_{i+1} = LB, x_{i+2} = UB, x_{i+3} = Threshold>

	int ptr = 0;
	for(int i=0;i<D;i++) {
		pointers.push_back(ptr);
		int len = (prob.feat[i].type == ATTR_CATEGORICAL)?3:4;
		ptr += len;
	}

	return ptr+1;
}

/**
 * Decode the real-valued vector into association rule by using the list of features.
 *
 * @param the real-valued vector, the list of features, and the mined association rule.
 * @return no return code.
 */
void Evaluate::decode(vector<double> x, vector<Feature> feat, Rule &rule)
{
	for(uint i=0;i<pointers.size();i++) {
		rule.init(i, x[pointers[i]]);
	}

	rule.sort();
	uint cp = x[x.size()-1]*(pointers.size()-2)+1;

	if(cp == 1)
		cp = pointers.size()/2+1;

	for(uint i=0;i<pointers.size();i++) {
		bool taken = false;
		int ptr = pointers[rule.perm[i]];
		string pair = feat[rule.perm[i]].f_name+"_";
		if(feat[rule.perm[i]].type == ATTR_CATEGORICAL) {	// categorical attribute
			if(random01() < x[ptr+2]) {
				int val = x[ptr+1]*feat[rule.perm[i]].hash.size();
				pair.append(feat[rule.perm[i]].hash[val]);
				taken = true;
			}
		} else if(feat[rule.perm[i]].type == ATTR_NUMERICAL) {	// numerical attribute
			if (random01() < x[ptr + 3]) {
				int a, b;
				if (x[ptr + 1] > x[ptr + 2]) {
					a = (feat[rule.perm[i]].i_num.upper - feat[rule.perm[i]].i_num.lower) * x[ptr + 2] + feat[rule.perm[i]].i_num.lower;
					b = (feat[rule.perm[i]].i_num.upper - feat[rule.perm[i]].i_num.lower) * x[ptr + 1] + feat[rule.perm[i]].i_num.lower;
				} else {
					a = (feat[rule.perm[i]].i_num.upper - feat[rule.perm[i]].i_num.lower) * x[ptr + 1] + feat[rule.perm[i]].i_num.lower;
					b = (feat[rule.perm[i]].i_num.upper - feat[rule.perm[i]].i_num.lower) * x[ptr + 2] + feat[rule.perm[i]].i_num.lower;
				}
				char str_pom[256];
				sprintf(str_pom, "%d_%d", a, b);
				string attr(str_pom);
				pair.append(attr);
				taken = true;
			}
		} else {
			if (random01() < x[ptr + 3]) {
				double a, b;
				if (x[ptr + 1] > x[ptr + 2]) {
					a = (feat[rule.perm[i]].f_num.upper - feat[rule.perm[i]].f_num.lower) * x[ptr + 2] + feat[rule.perm[i]].f_num.lower;
					b = (feat[rule.perm[i]].f_num.upper - feat[rule.perm[i]].f_num.lower) * x[ptr + 1] + feat[rule.perm[i]].f_num.lower;
				} else {
					a = (feat[rule.perm[i]].f_num.upper - feat[rule.perm[i]].f_num.lower) * x[ptr + 1] + feat[rule.perm[i]].f_num.lower;
					b = (feat[rule.perm[i]].f_num.upper - feat[rule.perm[i]].f_num.lower) * x[ptr + 2] + feat[rule.perm[i]].f_num.lower;
				}
				char str_pom[256];
				sprintf(str_pom, "%.4f_%.4f", a, b);
				string attr(str_pom);
				pair.append(attr);
				taken = true;
			}
		}
		if (taken) {
			if (i < (uint) cp) {
				rule.ante.insert(pair);
				rule.norm[rule.perm[i]] = 1;
			} else {
				rule.cons.insert(pair);
				rule.norm[rule.perm[i]] = -1;
			}
		}
	}
}

void Evaluate::print_vec(string str, vector<int>vec)
{
	cout << str << "= {";
	for(uint i=0;i<vec.size();i++) {
		cout << vec[i] << ",";
	}
	cout << "}" << endl;
}

void Evaluate::print_vec(string str, vector<double>vec)
{
	cout << str << "= {";
	for(uint i=0;i<vec.size();i++) {
		cout << vec[i] << ",";
	}
	cout << "}" << endl;
}

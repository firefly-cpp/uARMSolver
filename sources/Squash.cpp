/*
 * Squash.cpp
 *
 *  Created on: Mar 10, 2022
 *      Author: iztok
 */

#include "Squash.h"

Squash::Squash() {
	// TODO Auto-generated constructor stub

}

Squash::~Squash() {
	// TODO Auto-generated destructor stub
}

/**
 * Make squashing database.
 *
 * @param setup, problem classes.
 * @return the size of database.
 */
int Squash::make_sq_dbase(Setup setup, Problem prob)
{
	uint pos = 0;
	uint k = 0;
	cout << "make_sq_dbase started..." << endl;
	init_squashed(prob.dbase.size());
	while(pos < prob.dbase.size()) {
		sq_set.clear();
		// avoid all squashed rows
		while(pos < prob.dbase.size() && is_squashed(pos))
			pos++;
		int sq_set_name = pos;
		if(pos+1 < prob.dbase.size()) {
			sq_set.push_back(prob.dbase[pos]);
			set_squashed(pos);
			for(uint i=pos+1;i<prob.dbase.size();i++) {
				// avoid all squashed rows
				while(i < prob.dbase.size() && is_squashed(i))
					i++;
				if(i < prob.dbase.size()) {
					double sim;
					if(setup.get_squash() == SQUASH_CAUCHY)
						sim = sq_similarity(prob.dbase[pos], prob.dbase[i]);
					else
						sim = sq_distance(prob.dbase[pos], prob.dbase[i], prob.feat);
					if(sim >= setup.squashing_param.sq.Thresh) {
						sq_set.push_back(prob.dbase[i]);
						set_squashed(i);
						k++;
					}
				}
			}
		}
		if(sq_set.size() > 0) {
			vector<Attribute> sq_vec;
			make_sq_vector(sq_set_name, sq_vec, prob.feat.size());
			sq_dbase.push_back(sq_vec);
			sq_vec.clear();
		}
		pos++;
	}
	sq_set.clear();
	return sq_dbase.size();
}

/**
 * Reset the visited transaction.
 *
 * @param the number of transactions.
 * @return no return code.
 */
void Squash::init_squashed(int n)
{
	for(int i=0;i<n;i++)
		sq_squashed.push_back(false);
}

/**
 * Similarity according to the Cauchy-Schwartz inequality.
 *
 * @param the partition transaction, the similar transaction.
 * @return cos \phy between both transactions.
 */
double Squash::sq_similarity(vector<Attribute> vec_1, vector<Attribute> vec_2)
{
    uint n_attr = vec_1.size();
    double inner = 0.0, a = 0.0, b = 0.0;
    for(uint i=0;i<n_attr;i++) {
    	if(vec_1[i].type == ATTR_CATEGORICAL) {
    		if(vec_1[i].s_val == vec_2[i].s_val) {
    			inner += 1.0;
    			a += 1.0;
    			b += 1.0;
    		} else {
    			inner += 0.0;
    			a += 1.0;
    			b += 1.0;
    		}
    	} else if(vec_1[i].type == ATTR_NUMERICAL) {
			inner += vec_1[i].i_val*vec_2[i].i_val;
			a += (vec_1[i].i_val*vec_1[i].i_val);
			b += (vec_2[i].i_val*vec_2[i].i_val);
    	} else {
			inner += vec_1[i].f_val*vec_2[i].f_val;
			a += (vec_1[i].f_val*vec_1[i].f_val);
			b += (vec_2[i].f_val*vec_2[i].f_val);
    	}
    }
    return fabs((double) inner)/(sqrt((double)a)*sqrt((double)b));
}

/**
 * Similarity according to the Euclidian distance.
 *
 * @param the partition transaction, the compared transaction, the feature for identifying min and max values.
 * @return Euclidian distance between both transactions.
 */
double Squash::sq_distance(vector<Attribute> vec_1, vector<Attribute> vec_2, vector<Feature> feat)
{
    uint n_attr = vec_1.size();
    double dist = 0.0, weight = 0.0;
    for(uint i=0;i<n_attr;i++) {
    	if(vec_1[i].type == ATTR_CATEGORICAL) {
    		weight = 1.0/((double) feat[i].hash.size());
    		if(vec_1[i].s_val == vec_2[i].s_val)
    			dist += 0.0;
    		else
    			dist += weight*weight;
    	} else if(vec_1[i].type == ATTR_NUMERICAL) {
    		weight = 1.0/((double) feat[i].i_num.upper-(double) feat[i].i_num.lower);
			dist += ((double) (vec_1[i].i_val-vec_2[i].i_val)*(double) (vec_1[i].i_val-vec_2[i].i_val))*weight*weight;
    	} else {
    		weight = 1.0/((double) feat[i].f_num.upper-(double) feat[i].f_num.lower);
			dist += ((double) (vec_1[i].f_val-vec_2[i].f_val)*(double) (vec_1[i].f_val-vec_2[i].f_val))*weight*weight;
    	}
    }
	return 1-sqrt(dist);
}

/**
 * Making the squash vector.
 *
 * @param the starting transaction, the squashing vector, the number of attributes.
 * @return no return code.
 */
void Squash::make_sq_vector(int sq_set_name, vector<Attribute> &sq_vec, int n_attr)
{
	for(int i=0;i<n_attr;i++) {
		sq_vec.push_back(sq_set[0][i]);
		sq_vec[i].f_val = 0.0;
		sq_vec[i].i_val = 0;
		sq_vec[i].s_val = "";
		vector<string> similar_set;
		vector<int> similar_count;
		// for all similar transactions
		for(uint j=0;j<sq_set.size();j++) {
			if(sq_set[j][i].type == ATTR_CATEGORICAL) {
				int pos = in_similar_set(similar_set, sq_set[j][i].s_val);
				if(pos < 0) {
					pos = similar_set.size();
					similar_set.push_back(sq_set[j][i].s_val);
					similar_count.push_back(0);
				}
				similar_count[pos]++;
			} else if(sq_set[j][i].type == ATTR_NUMERICAL) {
				sq_vec[i].i_val += sq_set[j][i].i_val;
			} else {
				sq_vec[i].f_val += sq_set[j][i].f_val;
			}
		}
		// post processing
		if(sq_vec[i].type == ATTR_CATEGORICAL) {
			int pos = find_max_count(similar_count);
			sq_vec[i].s_val = similar_set[pos];
		} else if(sq_vec[i].type == ATTR_NUMERICAL) {
			sq_vec[i].i_val = trunc((double)sq_vec[i].i_val/(double)sq_set.size()+.5);
		} else {
			sq_vec[i].f_val = sq_vec[i].f_val/(double)sq_set.size();
		}
	}
}

/**
 * Check if the string is already in set.
 *
 * @param the string set, the string checked.
 * @return position of the string into set or -1.
 */
int Squash::in_similar_set(vector<string> set_str, string str)
{
	int pos = -1;
	uint i = 0;
	while(pos < 0 && i<set_str.size()) {
		if(set_str[i] == str)
			pos = i;
		else
			i++;
	}
	return pos;
}

/**
 * Find the string with the maximum occurrences into string set.
 *
 * @param the number of occurrences.
 * @return the maximum number of occurrences.
 */
int Squash::find_max_count(vector<int> count)
{
	int max_i = 0;
	for(uint i=1;i<count.size();i++) {
		if(count[i] > count[max_i])
			max_i = i;
	}
	return max_i;
}

/**
 * Print out the statistics obtained by the squashing method.
 *
 * @param the problem class.
 * @return no return code.
 */
void Squash::stat(Problem prob)
{
	cout << "Squshing statistics:" << endl;
	cout << "Original dbase: " << prob.dbase.size() << endl;
	cout << "Squashed dbase: " << sq_dbase.size() << endl;
	cout << "Squashing rate: " << (1.0-(double) sq_dbase.size()/(double) prob.dbase.size())*100.0 << " %" << endl;
}

/**
 * Print vector attributes.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Squash::print_vec(vector<Attribute> vec)
{
	cout << "{";
	for (uint i = 0; i < vec.size(); i++) {
		cout << "(" << vec[i].type << "," << vec[i].present << ",";
		if (vec[i].type == ATTR_CATEGORICAL)
			cout << vec[i].s_val;
		else if (vec[i].type == ATTR_NUMERICAL)
			cout << vec[i].i_val;
		else
			cout << vec[i].f_val;
		cout << ")";
	}
	cout << "}" << endl;
}

/**
 * Print bool vector (for debugging purposes).
 *
 * @param no input parameters.
 * @return no return code.
 */
void Squash::print_vec(vector<bool> vec)
{
	cout << "(";
	for (uint i = 0; i < vec.size(); i++) {
		cout << vec[i] << ",";
	}
	cout << ")" << endl;
}

/**
 * Print int vector (for debugging purposes).
 *
 * @param no input parameters.
 * @return no return code.
 */
void Squash::print_vec(vector<int> vec)
{
	cout << "(";
	for (uint i = 0; i < vec.size(); i++) {
		cout << vec[i] << ",";
	}
	cout << ")" << endl;
}

/**
 * Print int vector (for debugging purposes).
 *
 * @param no input parameters.
 * @return no return code.
 */
void Squash::print_vec(vector<string> vec)
{
	cout << "(";
	for (uint i = 0; i < vec.size(); i++) {
		cout << vec[i] << ",";
	}
	cout << ")" << endl;
}

/**
 * Print attributes.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Squash::print()
{
	cout << "Squashed matrix:" << endl;
	for (uint i = 0; i < sq_dbase.size(); i++) {
		cout << "[" << i+1 << "] (";
		for (uint j = 0; j < sq_dbase[i].size(); j++) {
			if(sq_dbase[i][j].type == ATTR_CATEGORICAL)
				cout << sq_dbase[i][j].s_val << ",";
			else if(sq_dbase[i][j].type == ATTR_NUMERICAL)
				cout << sq_dbase[i][j].i_val << ",";
			else
				cout << sq_dbase[i][j].f_val << ",";
		}
		cout << ")" << endl;
	}
}

/**
 * Write attributes into output file.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Squash::write(Setup setup)
{
	ofstream out(setup.get_sq_dbase_name());

	if (out.is_open()) {
		for (uint i = 0; i < sq_dbase.size(); i++) {
			for (uint j = 0; j < sq_dbase[i].size(); j++) {
				if (sq_dbase[i][j].type == ATTR_CATEGORICAL)
					out << sq_dbase[i][j].s_val << ",";
				else if (sq_dbase[i][j].type == ATTR_NUMERICAL)
					out << sq_dbase[i][j].i_val << ",";
				else
					out << sq_dbase[i][j].f_val << ",";
			}
			out << endl;
		}
	}
	out.close();
}

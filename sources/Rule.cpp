#include "Rule.h"

Rule::Rule() {
	// TODO Auto-generated constructor stub

}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

/**
 * Initialize association rule class.
 *
 * @param the sequence number, on which permutation bases, random value determining the permutation ordering of features.
 * @return no return code.
 */
void Rule::init(int i, double x)
{
	perm.push_back(i);
	val.push_back(x);
	norm.push_back(0);
}

/**
 * Sort the permutation vector according to decreasing order of values obtaining from encoded solution.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Rule::sort()
{
	for(uint i=0;i<perm.size()-1;i++) {
		for(uint j=i+1;j<perm.size();j++) {
			if(val[i] > val[j]) {
				int k = perm[i]; perm[i] = perm[j]; perm[j] = k;
				double x = val[i]; val[i] = val[j]; val[j] = x;
			}
		}
	}
}

/**
 * Find the feature into the list of features.
 *
 * @param the list of features, and the string denoting item to be find.
 * @return integer return code denoting the index in the list of feature, when the item is found or -1, when it is not found.
 */
int Rule::find_feat(vector<Feature>feat, string item)
{
	vector<Feature>::iterator it=feat.begin();
	bool found = false;
	int index = -1;

	while(!found && it!=feat.begin()) {
		if(it->f_name == item) {
			found = true;
		} else {
			it++;
			index++;
		}
	}

	return index;
}

/**
 * Check the association rule for validity.
 *
 * @param no input parameters.
 * @return binary return code: =1, if the rule consist of valid antecedent and consequent, and =0, if the rule does not have either the antecedent or consequent.
 */
bool Rule::isValid()
{
	return (ante.size() > 0) && (cons.size() > 0);
}

/**
 * Calculate inclusion ARM measure for the corresponding association rule.
 *
 * @param no input parameters.
 * @return floating-point return code denoting the value of inclusion.
 */
double Rule::calc_inclusion()
{
	return (ante.size()+cons.size())/(double) perm.size();
}

/**
 * Print a particular association rule.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Rule::print()
{
	cout << "Rule printout: fitness= " << fitness << ", support= " << support <<
			", confidence= " << confidence  << ", inclusion= " << inclusion << ", ante={";
	for(set<string>::iterator it=ante.begin();it != ante.end();++it) {
		cout << *it << ",";
	}
	cout << "}, cons={";
	for(set<string>::iterator it=cons.begin();it != cons.end();++it) {
		cout << *it << ",";
	}
	cout << "}" << endl;
}

/**
 * Write the association rule into output file in specific format.
 *
 * @param the output file.
 * @return no return code.
 */
void Rule::write(ofstream &out)
{
	out << fitness << " [";
	for(set<string>::iterator it=ante.begin();it != ante.end();++it) {
		if(it != ante.begin())
			out << ",";
		out << "\'" << *it << "\'";
	}
	out << "]===>[";
	for(set<string>::iterator it=cons.begin();it != cons.end();++it) {
		if(it != cons.begin())
			out << ",";
		out << "\'" << *it << "\'";
	}
	out << "] " << support << " " << confidence << endl;
}

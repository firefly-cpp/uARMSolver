#include "Feature.h"

Feature::Feature() {
	// TODO Auto-generated constructor stub

}

Feature::~Feature() {
	// TODO Auto-generated destructor stub
}

/**
 * Adjust the corresponding bound values in the list of features by entering an integer value.
 *
 * @param the integer value of an attribute.
 * @return no return code.
 */
void Feature::enter(int val)
{
	if(type != ATTR_NUMERICAL) {
		if(type == ATTR_REAL_VALUED) {
			enter((double) val);
			return;
		}
		cout << "Invalid attribute type." << endl;
		exit(-1);
	}

	if ( val < i_num.lower ) {	// new lower bound
		i_num.lower = val;
	}
	if ( val > i_num.upper ) {	// new upper bound
		i_num.upper = val;
	}
}

/**
 * Adjust the corresponding bound values in the list of features by entering a floating-point value.
 *
 * @param the floating-point value of an attribute.
 * @return no return code.
 */
void Feature::enter(double val)
{
	if(type != ATTR_REAL_VALUED) {
		cout << "Invalid attribute type." << endl;
		exit(-1);
	}

	if ( val < f_num.lower ) {	// new lower bound
		f_num.lower = val;
	}
	if ( val > f_num.upper ) {	// new upper bound
		f_num.upper = val;
	}
}

/**
 * Adjust the corresponding bound values in the list of features by entering a categorical value.
 *
 * @param the categorical value of an attribute, and bool indicator determining strict type-checking.
 * @return no return code.
 */
void Feature::enter(string item, bool check)
{
	if(check && type != ATTR_CATEGORICAL) {
		cout << "Invalid attribute type." << endl;
		exit(-1);
	}

	vector<string>::iterator pos = find(hash.begin(), hash.end(), item);

	if ( pos == hash.end() ) {	// add new row
		hash.push_back(item);
	}
}

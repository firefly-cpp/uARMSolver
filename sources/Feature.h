#ifndef SOURCE_FEATURE_H_
#define SOURCE_FEATURE_H_

#include <vector>

#include "Attribute.h"

using namespace std;

typedef struct {
	int lower;
	int upper;
} int_bounds;

typedef struct {
	double lower;
	double upper;
} float_bounds;


/**
 * Definition of the feature items
 *
 * Each feature is defined by its name, type and corresponding value. There
 * are three types of features: numerical, real-valued, and categorical. While
 * the numerical and real-valued features are represented as intervals of either
 * integer or floating-point values, the categorical features are defined by
 * discrete set of attributes. Functions enter() are dedicated for manipulation
 * with data structures.
 *
 */
class Feature {
public:
	Feature();
	virtual ~Feature();

	void enter(int val);
	void enter(double val);
	void enter(string item, bool check = true);

public:
	bool init;
	string f_name;			///< feature name
	unsigned int type;		///< feature type
	int_bounds i_num;		///< numerical features with attributes limited by integer bounds
	float_bounds f_num;		///< real-valued features with attributes limited by floating-point bounds
	vector<string> hash;	///< categorical features represented as a set of discrete attributes
};

#endif /* SOURCE_FEATURE_H_ */

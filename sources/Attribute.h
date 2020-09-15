#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#define	ATTR_CATEGORICAL	1
#define	ATTR_NUMERICAL		2
#define	ATTR_REAL_VALUED	3

using namespace std;


/**
 * Definition of an archive of mined association rules
 *
 * An archive is represented as a list of association rules. On this list,
 * functions, like read(), write(), find_feat(), check_item(), inArchive(),
 * and add(), are implemented, which enable manipulation with elements within
 * the data structure. The purpose of the rest of class functions is tracing
 * the contents of the data structure.
 *
 */
class Attribute {
public:
	Attribute();
	virtual ~Attribute();

	void enter(int val);
	void enter(double val);
	void enter(string val);
	void not_present() { present = false; }

public:
	unsigned int type;
	bool present;
	int i_val;
	double f_val;
	string s_val;
};

#endif /* ATTRIBUTE_H_ */

#ifndef ARCHIVE_H_
#define ARCHIVE_H_

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

#include "Problem.h"
#include "Rule.h"
#include "Setup.h"

using namespace std;

/**
 * Implementation of an archive of mined association rules
 *
 * An archive is represented as a list of association rules. On this list,
 * functions, like read(), write(), find_feat(), check_item(), inArchive(),
 * and add(), are implemented, which enable manipulation with elements within
 * the data structure. The purpose of the rest of class functions is tracing
 * the contents of the data structure.
 *
 */
class Archive {
public:
	Archive();
	virtual ~Archive();

	bool read(string if_name, Problem prob);
	bool write(string of_name, Problem prob);
	void token_line(string line, vector<string> &tokens, char ch);
	bool check_item(vector<Feature> feat, string item);
	int find_feat(vector<Feature> feat, string item);
	void add(Rule rule);

	void print_tokens(vector<string>tokens);
	void print();

public:
	vector <Rule> arch;	///< Archive of mined association rules data structure.
};

#endif /* ARCHIVE_H_ */

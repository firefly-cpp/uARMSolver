#include "Problem.h"
#include <cstring>

Problem::Problem()
{
}

Problem::~Problem(void)
{
}

/**
 * Read input file containing a collection of transactions, parse the line and build the internal data structures.
 *
 * @param the parameter setup, and the name of the input file(s).
 * @return boolean return code: =1, if input file is processed successfully, =0, if input file cannot be found.
 */
bool Problem::init_tdbase(Setup setup, string tfile_name)
{
	ifstream in(tfile_name);
	bool readed = false;
	bool auto_parsing = false;
	int num_line = 0;
	string line;

	cout << "init_tdbase= started..." << endl;
	if(tfile_name.compare(tfile_name.size()-5,4,".csv"))
		auto_parsing = true;

	if (in.is_open()) {
		cout << "File: " << tfile_name << endl;
//int li = 1;
		while (getline(in, line)) {
cout << "Line " << 	num_line+1 << ": \'" << line << "\'" << endl;
			vector<string> tokens;
			token_line(line, tokens, ',');
/*
			// debug
			for (uint i = 0; i < tokens.size(); i++) {
				cout << "Token[" << i << "]: " << tokens[i];
				for (uint j = 0; j < tokens[i].size(); j++) {
					cout << "\'" << tokens[i][j] << "\' ";
				}
				cout << endl;
			}
*/
			if (auto_parsing) {
				for (uint i = 0; i < tokens.size(); i++) {
//cout << "Token: " << tokens[i] << endl;
					if (num_line == 0) {
						new_feat(tokens[i]);
					} else {
						static int n_pos = 0;
						n_pos = analyze_token(tokens[i], i, n_pos);
					}
				}
			}
			num_line++;
			tokens.clear();
//if (num_line == 5) exit(-1);
		}
		readed = true;
		cout << "File " << tfile_name << " read successfully." << endl;
	} else {
		cout << "File " << tfile_name << " not open." << endl;
		exit(-1);
	}

	disc_attr();
	print_feat(setup.get_intervals());
//	print_rand_var(10);

	cout << "init_tdbase= ...OK." << endl;

	return readed;
}

/**
 * Enter a new feature into a list of features.
 *
 * @param the name of the feature.
 * @return no return code.
 */
void Problem::new_feat(string token)
{
	Feature tmp;
	tmp.f_name = token;
	tmp.init = false;

	feat.push_back(tmp);
}

/**
 * Analyze the parsed token from an input line.
 *
 * @param the corresponding token to be analyzed, the number of feature, and the number of attribute.
 * @return integer return code denoting the number of transaction already entered into a database.
 */
int Problem::analyze_token(string token, int n_feat, int n_tran)
{
	if(n_feat==0) {	// enter a new transaction
		n_tran = new_tran();
	}
//cout << "Token: " << token << ", init= " << feat[n_feat].init << ", n_feat= " << n_feat << ", n_tran= " << n_tran << ", type= ";
	if(token.size() == 0) {	// attribute not present?
		dbase[n_tran][n_feat].not_present();
	} else if (containsOnlyDigit (token)) {	// is numerical?
//cout << "containsOnlyDigit " << endl;
		if (!feat[n_feat].init) {
			feat[n_feat].type = ATTR_NUMERICAL;
			feat[n_feat].i_num.lower = feat[n_feat].i_num.upper = stoi(token);
			feat[n_feat].init = true;
		}
		if(feat[n_feat].type == ATTR_NUMERICAL) {
			dbase[n_tran][n_feat].enter(stoi(token));
			feat[n_feat].enter(stoi(token));
		} else {
//cout << "Expected type= " << feat[n_feat].type << " proposed type= ATTR_NUMERICAL" << endl;
			dbase[n_tran][n_feat].enter(stod(token));
			feat[n_feat].enter(stod(token));
		}
//cout << "Numerical " << feat[n_feat].type  << endl;
	} else if (containsFloatingPoint(token)) {	// is real valued?
//cout << "containsFloatingPoint " << endl;
		if (!feat[n_feat].init) {
			feat[n_feat].type = ATTR_REAL_VALUED;
			feat[n_feat].f_num.lower = feat[n_feat].f_num.upper = stod(token);
			feat[n_feat].init = true;
		}
//cout << "Float " << feat[n_feat].type  << endl;
		dbase[n_tran][n_feat].enter(stod(token));
		if(!dbase[n_tran][n_feat].present) {
			reassign_type(n_tran, n_feat);
			dbase[n_tran][n_feat].present = true;
		}
		feat[n_feat].enter(stod(token));
	} else {	// is categorial!
//cout << "Is categorical " << endl;
		if (!feat[n_feat].init) {
			feat[n_feat].type = ATTR_CATEGORICAL;
			feat[n_feat].init = true;
		}
//cout << "Categorical " << feat[n_feat].type << endl;
		dbase[n_tran][n_feat].enter(token);
		if(!dbase[n_tran][n_feat].present) {
			reassign_type(n_tran, n_feat);
		}
		feat[n_feat].enter(token, true);
	}

	return n_tran;
}

void Problem::reassign_type(int n_tran, int n_feat) {
cout << "reassign_type: from type= " << feat[n_feat].type << " [" << feat[n_feat].i_num.lower << "," << feat[n_feat].i_num.upper << "] to [";
	feat[n_feat].type = ATTR_REAL_VALUED;
	feat[n_feat].f_num.lower = feat[n_feat].f_num.upper = (double) dbase[n_tran][n_feat].f_val;
	for(int i=0;i<=n_tran-1;i++) {
		dbase[i][n_feat].f_val = (double) dbase[i][n_feat].i_val;
		dbase[i][n_feat].type = dbase[n_tran][n_feat].type;
		if (dbase[i][n_feat].f_val  < feat[n_feat].f_num.lower ) {	// new lower bound
			feat[n_feat].f_num.lower = dbase[i][n_feat].f_val;
		}
		if ( dbase[i][n_feat].f_val > feat[n_feat].f_num.upper ) {	// new upper bound
			feat[n_feat].f_num.upper = dbase[i][n_feat].f_val;
		}

	}
	dbase[n_tran][n_feat].present = true;
cout << feat[n_feat].f_num.lower << "," << feat[n_feat].f_num.upper << "]." << endl;
}

/**
 * Discretize numerical and real-valued feature as interval of integer or floating-point values and save this as a discrete attribute.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Problem::disc_attr()
{
	char str_pom[256];

    for(uint i=0;i<feat.size();i++) {
    	if(feat[i].type == ATTR_NUMERICAL) {
			sprintf(str_pom, "%d_%d", feat[i].i_num.lower, feat[i].i_num.upper);
   			string item(str_pom);
   			feat[i].enter(item, false);
    	}
    	if(feat[i].type == ATTR_REAL_VALUED) {
    		sprintf(str_pom, "%.4f_%.4f", feat[i].f_num.lower, feat[i].f_num.upper);
    		string item(str_pom);
    		feat[i].enter(item, false);
    	}
    }
}

/**
 * Create an empty row in a transaction database.
 *
 * @param no input parameters.
 * @return integer return code denoting the number of transaction already entered into database.
 */
uint Problem::new_tran()
{
    vector<Attribute> row;
    uint n_rows = dbase.size();

    for(uint i=0;i<feat.size();i++) {
    	Attribute att;
    	att.type = feat[i].type;
    	row.push_back(att);
    }
    dbase.push_back(row);

    return n_rows;
}

/**
 * This function returns floating point part of a number-string.
 *
 * @param the observed token.
 * @return boolean return code: =1, if the token includes floating-point (char '.'), or =0, if token represents an integer.
 */
bool Problem::containsFloatingPoint(string str)
{
    size_t pos = str.find(".");
    if (pos == string::npos)
        return false;
    else
        return true;
}

/**
 * This function checks whether a string contains all digit or not.
 *
 * @param the observed token.
 * @return boolean return code: =1, if the token consisting of digits only, or =0, otherwise.
 */
bool Problem::containsOnlyDigit(string str)
{
    int l = str.length();
//cout << "containsOnlyDigit: " << str << " l= " << l << " " << endl;
    for (int i = 0; i < l; i++)
    {
//cout << "\'" << str.at(i) << "\' [" << std::hex << str.at(i) << "] " << std::dec << endl;
        if ((str.at(i) < '0' || str.at(i) > '9') && str.at(i) != '-' && str.at(i) != '\r') {
//cout << " return FALSE" << endl;
            return false;
        }
    }
//cout << " return TRUE" << endl;
    return true;
}

/**
 * Tokenize the input line.
 *
 * @param the input line, the vector of output tokens, and the delimiter character.
 * @return no return code.
 */
void Problem::token_line(string line, vector<string> &tokens, char ch) {

	// Tokenizing w.r.t. space ' '
	char *cstr = new char[line.length() + 1];
	std::strcpy(cstr, line.c_str());

	// cstr now contains a c-string copy of str
	char delim[4];
	sprintf(delim, "%c%c%c", ch, 0x0a, 0x0d);
	char *p = std::strtok(cstr, delim);

	while (p != 0) {
		tokens.push_back(p);
		p = std::strtok(NULL, delim);
//		std::cout << "[" << ++i << "]= \'" << p << "\'" << '\n';
	}

	delete[] cstr;
	/*
	 stringstream check1(line);
	 string intermediate;
	 while(getline(check1, intermediate, ch)) {
	 tokens.push_back(intermediate);
	 cout << "check1= \'" << check1.str() << "\' : [" << i+1 << "]= \'" << intermediate << "\'" << endl;
	 i++;
	 }
	*/
}

/**
 * Calculate support ARM measure for a particular mined association rule.
 *
 * @param the observed association rule.
 * @return floating-point return code denoting the calculated support.
 */
double Problem::calc_support(Rule rule)
{
	uint hits = 0;
	for(uint i=0;i<dbase.size();i++) {
		vector<string> tran;
		uint hit_ante = 0;
		uint hit_cons = 0;
		for(int j=0;j<get_dimension();j++) {
			if (dbase[i][j].present) {
				if (feat[j].type == ATTR_CATEGORICAL) {
					string attr;
					make_pair(i, j, attr);
					set<string>::iterator it = rule.ante.find(attr);
					if (it == rule.ante.end()) {
						it = rule.cons.find(attr);
						if (it != rule.cons.end())
							hit_cons++;
					} else {
						hit_ante++;
					}
				} else {	// numerical and real_valued
					if (isIncluded(i, j, rule)) {
						if(rule.norm[j] > 0)
							hit_ante++;
						else
							hit_cons++;
					}
				}
			}
		}
		if((hit_ante+hit_cons) == (rule.ante.size()+rule.cons.size())) {
			hits++;
		}
	}
	return (double) hits/(double) dbase.size();
}

/**
 * Verify, if the particular numerical or real-valued feature is already included into the list of features.
 *
 * @param the observed transaction, the feature, and the association rule.
 * @return binary return code: =1, if the feature is already present in the list, or =0, if the feature is not presented.
 */
bool Problem::isIncluded(int i, int j, Rule rule)
{
	bool found = false;

	if (feat[j].type == ATTR_NUMERICAL) {
		if (rule.norm[j] != 0) {
			if (dbase[i][j].i_val >= feat[j].i_num.lower && dbase[i][j].i_val <= feat[j].i_num.upper) {
				found = true;
			}
		}
	} else {
		if (rule.norm[j] != 0) {
			if (dbase[i][j].f_val >= feat[j].f_num.lower && dbase[i][j].f_val <= feat[j].f_num.upper) {
				found = true;
			}
		}
	}

	return found;
}

/**
 * Calculate confidence ARM measure for a particular mined association rule.
 *
 * @param the observed association rule.
 * @return floating-point return code denoting the calculated confidence.
 */
double Problem::calc_confidence(Rule rule)
{
	int hit_1 = 0;
	int hit_2 = 0;
	for(uint i=0;i<dbase.size();i++) {
		vector<string> tran;
		uint hit_ante = 0;
		uint hit_union = 0;
		for(int j=0;j<get_dimension();j++) {
			if (dbase[i][j].present) {
				if (feat[j].type == ATTR_CATEGORICAL) {
					string attr;
					make_pair(i, j, attr);
					set<string>::iterator it = rule.ante.find(attr);
					if (it == rule.ante.end()) {
						it = rule.cons.find(attr);
						if (it != rule.cons.end())
							hit_union++;
					} else {
						hit_ante++;
						hit_union++;
					}
				} else {
					if (isIncluded(i, j, rule)) {
						if(rule.norm[j] > 0)
							hit_ante++;
						hit_union++;
					}
				}
			}
		}
		if((hit_union) == (rule.ante.size()+rule.cons.size())) {
			hit_1++;
		}
		if(hit_ante == rule.ante.size()) {
			hit_2++;
		}
	}
	return (double) hit_1/(double) hit_2;
}

/**
 * Make a pair <feature,attribute> from the string.
 *
 * @param the observed transaction, the feature, and the input strung.
 * @return no return code.
 */
void Problem::make_pair(int i, int j, string &attr)
{
	switch(dbase[i][j].type) {
	case ATTR_CATEGORICAL:
		attr = feat[j].f_name+"_"+dbase[i][j].s_val;
		break;
	case ATTR_NUMERICAL: {
		attr = feat[j].f_name+"_"+"NUMERICAL";	//feat[j].hash[k];
	}
	break;
	default: {
		attr = feat[j].f_name+"_"+"REAL_VALUED";	//"feat[j].hash[k];

	}
	}
}

/**
 * Print a vector of parsed tokens.
 *
 * @param the input vector.
 * @return no return code.
 */
void Problem::print_tokens(vector<string>tokens)
{
	cout << "Tokens printout:" << endl;
	cout << "---------------------" << endl;
	for(uint i=0;i<tokens.size();i++) {
		cout << i << ": " << tokens[i] << endl;
	}
	cout << "---------------------" << endl;
}

/**
 * Print a list of tokens.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Problem::print_feat(int intervals)
{

	// print feature
	cout << "Feature table:" << endl;
	for (uint i = 0; i < feat.size(); i++) {
		cout << "[" << i+1 << "] <\'" << feat[i].f_name << "\',";
		if(feat[i].type == ATTR_CATEGORICAL) {
			cout << "Type=CATEGORICAL,";
		} else if (feat[i].type == ATTR_NUMERICAL) {
			int Delta = feat[i].i_num.upper - feat[i].i_num.lower;
			int delta = 0;
			if(intervals > 1)
				delta = Delta/intervals;
			cout << "Type=NUMERICAL,Interval=[" << feat[i].i_num.lower << "," << feat[i].i_num.upper <<
					"], Delta= " << Delta << ", offset= " << delta << ", ";
		} else {
			double Delta = feat[i].f_num.upper - feat[i].f_num.lower;
			double delta = 0;
			if(intervals > 1)
				delta = Delta/(double) intervals;
			cout << "Type=REAL_VALUED,Interval=[" << feat[i].f_num.lower << "," << feat[i].f_num.upper <<
					"], Delta= " << Delta << ", offset= " << delta << ", ";
		}
		print_hash(feat[i].hash);
	}
}

/**
 * Print attributes.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Problem::print_attr()
{
	cout << "Attribute matrix:" << endl;
	for (uint i = 0; i < dbase.size(); i++) {
		cout << "[" << i+1 << "] (";
		for (uint j = 0; j < feat.size(); j++) {
			if(!dbase[i][j].present)
				cout << "NULL,";
			else if(feat[j].type == ATTR_CATEGORICAL)
				cout << dbase[i][j].s_val << ",";
			else if(feat[j].type == ATTR_NUMERICAL)
				cout << dbase[i][j].i_val << ",";
			else
				cout << dbase[i][j].f_val << ",";
		}
		cout << ")" << endl;
	}
}

/**
 * Print the hash table, where the attributes of the particular feature are saved.
 *
 * @param the vector representing the hash table.
 * @return no return code.
 */
void Problem::print_hash(vector<string> hash)
{
	// print hash table
	cout << "Attributes={";
	for (uint i = 0; i < hash.size(); i++) {
		cout << hash[i] << ",";
	}
	cout << "}>" << endl;
}

void Problem::print_rand_var(int classes)
{
	cout << "Classes= " << classes << endl;
	vector < vector <int> > freq(feat.size(), vector<int>(classes, 0));

	for (uint i = 0; i < feat.size(); i++) {
		for (uint j = 0; j < dbase.size(); j++) {
			if(dbase[j][i].present) {
				if(feat[i].type == ATTR_CATEGORICAL) {
					int k = find_classS(feat[i].hash, dbase[j][i].s_val);
					if(k>=0) {
						freq[i][k]++;
//						cout << "s_val= " << dbase[j][i].s_val << " : freq[" << i << "," << k << "]= " << freq[i][k] << endl;
					}
				} else if(feat[i].type == ATTR_NUMERICAL) {
					int k = find_classI(feat[i].i_num, classes, dbase[j][i].i_val);
					if(k>=0) {
						freq[i][k]++;
//						cout << dbase[j][i].f_val << " : [";
//						cout << feat[i].f_num.lower << "," << feat[i].f_num.upper << "] --> ";
//						cout << "freq[" << i << "," << k << "]= " << freq[i][k] << endl;
					}
//					exit(-1);
				} else {
					int k = find_classF(feat[i].f_num, classes, dbase[j][i].f_val);
					if(k>=0) {
						freq[i][k]++;
//						cout << dbase[j][i].f_val << " : [";
//						cout << feat[i].f_num.lower << "," << feat[i].f_num.upper << "] --> ";
//						cout << "freq[" << i << "," << k << "]= " << freq[i][k] << endl;
					}
//					exit(-1);
				}
			}
		}
		print_freq(freq, i, classes);
	}
	cout << "Classes= ...OK" << endl;
}

void Problem::print_freq(vector < vector <int> > freq, int k, int classes)
{
	int summ = 0;
	cout << "Feat: " << k << " [";
	for(int i=0;i<classes;i++) {
		cout << freq[k][i] << ",";
		summ += freq[k][i];
	}
	cout << "] --> " << summ << endl;
}

int Problem::find_classS(vector<string> hash, string str)
{
	for (uint i = 0; i < hash.size(); i++) {
		if(hash[i] == str)
			return i;
	}
	return -1;
}

int Problem::find_classI(int_bounds bound, int classes, int val)
{
	int delta = bound.upper-bound.lower;
	int map = trunc(classes*(val-bound.lower)/delta);
	if(map == classes)
		map--;

	return map;
}

int Problem::find_classF(float_bounds bound, int classes, float val)
{
	float delta = bound.upper-bound.lower;
	int map = trunc((float) classes*(val-bound.lower)/delta);
	if(map == classes)
		map--;

	return map;
}

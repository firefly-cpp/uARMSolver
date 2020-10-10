#include "Archive.h"

#include <iterator>

Archive::Archive()
{
	// TODO Auto-generated constructor stub

}

Archive::~Archive()
{
	// TODO Auto-generated destructor stub
}

/**
 * Read an existing archive of mined association rules.
 *
 * @param an input file name, and a problem definition.
 * @return binary return code: =1, if input file has processed OK, and =0, if the file does not exist.
 */
bool Archive::read(string if_name, Problem prob)
{
	string line;
	set <string> ante;
	set <string> cons;

	ifstream in(if_name);

	if (in.is_open()) {
		while (getline(in, line) && !line.empty()) {
			vector<string> tokens;
			token_line(line, tokens, ' ');
			Rule rule;
			for(uint i=0;i<tokens.size();i++) {
				switch(i) {
				case 0:		// fitness
					rule.set_fitness(atof(tokens[i].c_str()));
				break;
				case 1: {	// rule
					char *p, *s;
					bool first = true;
					s = (char *) tokens[1].c_str();
					while ((p = strchr(s, '\'')) != NULL) {
						char *t = p;
						if ((p = strchr(t + 1, '\'')) != NULL) {
							*p = '\0';
							string item = "";
							item.append(t + 1);
 							if (!check_item(prob.feat, item)) {	// item not found
 								exit(-1);
 							}
							if (first) {
								rule.ante.insert(item);
							} else {
								rule.cons.insert(item);
							}
							if (*(p + 1) == ',')
								s = p + 1;
							else if (*(p + 1) == ']') {
								s = p + 1;
								if (first)
									first = !first;
							}
						}
					}
				}
				break;
				case 2:		// support
					rule.set_support(atof(tokens[i].c_str()));
				break;
				case 3:		// confidence
					rule.set_confidence(atof(tokens[i].c_str()));
				break;
				}
			}
			arch.push_back(rule);
			ante.clear();
			cons.clear();
		}
		in.close();
	} else {
		cout << "File " << if_name << " not open." << endl;
		return false;
	}
	cout << "Rule(s) parsed: " << arch.size() << endl;
	print();
	return true;
}

/**
 * Create a new archive of mined association rules.
 *
 * @param output file, where the mined association rule are saved.
 * @return binary return code: =1, if writing stop successfully, and =0, if the output file cannot be opened.
 */
bool Archive::write(string of_name, Problem prob)
{
	string line;
	set <string> ante;
	set <string> cons;

	ofstream out(of_name);

	if (out.is_open()) {
		for(uint i=0;i<arch.size();i++) {
			arch[i].write(out);
		}
		out.close();
	} else {
		cout << "File " << of_name << " not open." << endl;
		return false;
	}
	cout << "Rule(s) processed: " << arch.size() << endl;

	return true;
}

/**
 * Check, if parsed <feature,attribute> pair already exists in feature/attribute list.
 *
 * @param list of features, and the corresponding <feature,attribute> pair.
 * @return binary return code: =1, if the pair was found in the lists, and =0, if the new pair needs to be build.
 */
bool Archive::check_item(vector<Feature> feat, string item)
{
	vector<string> elem;

	token_line(item, elem, '_');
	int index = find_feat(feat, elem[0]);

	if (index < 0) {	// feature not found
		cout << "Feature \'" << elem[0] << "\' not found. " << endl;
		return false;
	}

	vector<string>::iterator pos = find(feat[index].hash.begin(),feat[index].hash.end(),elem[1]);

	if(pos == feat[index].hash.end()) {	// attribute not found
		cout << "Attribute \'" << elem[1] << "\' not found. " << endl;
		return false;
	}
	return true;
}

/**
 * Find the feature into the list of features.
 *
 * @param list of features, and the name of the feature to be found.
 * @return an integer value denoting either an index of the element into the list or -1 when the feature is not found.
 */
int Archive::find_feat(vector<Feature> feat, string item)
{
	vector<Feature>::iterator first = feat.begin();
	vector<Feature>::iterator pos = first;
	bool found = false;

	while (!found && pos != feat.end()) {
		if (pos->f_name == item)
			found = true;
		else
			pos++;
	}
	return (found)?distance(first, pos):-1;
}

/**
 * Tokenize the input line into tokens delimited by delimiting character.
 *
 * @param the observed input line from the file, list of parsed tokens, and the delimiting character.
 * @return no return code.
 */
void Archive::token_line(string line, vector<string> &tokens, char ch)
{
	// stringstream class check1
	stringstream check1(line);
	string intermediate;

	// Tokenizing w.r.t. space ' '
	while(getline(check1, intermediate, ch))
	{
		tokens.push_back(intermediate);
	}
}

/**
 * Add the mined association rule into an archive.
 *
 * @param the mined association rule.
 * @return no return code.
 */
void Archive::add(Rule rule)
{
	arch.push_back(rule);
}

/**
 * Sort and an archive of mined association rules according to support.
 *
 * @param no parameters.
 * @return no return code.
 */
void Archive::asort()
{
	sort(arch.begin(), arch.end(), compare);
}

bool compare(Rule a, Rule b)
{
	if(a.get_fitness() <= b.get_fitness())
		return false;
	return true;
}
void Archive::print_tokens(vector<string>tokens)
{
	cout << "Tokens printout:" << endl;
	cout << "---------------------" << endl;
	for(uint i=0;i<tokens.size();i++) {
		cout << i << ": " << tokens[i] << endl;
	}
	cout << "---------------------" << endl;
}

void Archive::print()
{
	cout << "Rules printout:" << endl;
	cout << "---------------------" << endl;
	for(uint i=0;i<arch.size();i++) {
		cout << "[" << i << "]: ";
		arch[i].print();
	}
	cout << "---------------------" << endl;
}

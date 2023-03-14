#include <bits/stdc++.h>
#include "Setup.h"

Setup::Setup(string fname)
{
	// TODO Auto-generated constructor stub
	arm_set_name = fname;
}

Setup::~Setup()
{
	// TODO Auto-generated destructor stub
}

/**
 * Read setup file.
 *
 * @param no input parameters.
 * @return binary return code: =1, if setup file is read successfully, or =0, if input file cannot be found.
 */
bool Setup::read()
{
	ifstream in(arm_set_name);
	string line;
	bool readed = true;

	if (in.is_open()) {
		int block = 0;
		int parse_level = 0;

		while (getline(in, line)) {
			// Vector of string to save tokens
			vector <string> tokens;
			token_line(line, tokens);

			if (line.size() > 0) {	// line cannot be empty!

				if (tokens[0].compare("Problem") == 0) {	// program block
					block = 1;
				} else if (tokens[0].compare("Algorithm") == 0) {// algorithm block
					block = 2;
					solver = alg_to_int(tokens[2]);
				} else if (tokens[0].compare("Visualisation") == 0) {// visualizationblock
					block = 3;
					visual = vis_to_int(tokens[2]);
				} else if (tokens[0].compare("Squash") == 0) {// squashing
					block = 4;
					squash = sq_to_int(tokens[2]);
				} else if (tokens[0][0] == '%') {	// comment line - ignore
					continue;
				} else if (tokens[0][0] == '{') {	// start of block
					parse_level = block;
				} else if (tokens[0][0] == '}') {	// end of block
					parse_level = 0;
				} else {	// parsing blocks
					switch (parse_level) {
					case 1:		// parsing program block
						parse_prob_blk(tokens);
						break;
					case 2:		// parsing algorithm block
						parse_alg_blk(tokens);
						break;
					case 3:		// parsing visualization block
						parse_vis_blk(tokens);
						break;
					case 4:		// parsing squashing block
						parse_sq_blk(tokens);
						break;
					}
				}

			}
			tokens.clear();
		}
	} else {
		readed = false;
	}

	return readed;
}

/**
 * Tokenize input line stream into tokens.
 *
 * @param the input line, and the vector of parsed tokens.
 * @return no return code.
 */
void Setup::token_line(string line, vector<string> &tokens)
{
	// stringstream class check1
	stringstream check1(line);
	string intermediate;

	// Tokenizing w.r.t. space ' '
	while(getline(check1, intermediate, ' '))
	{
		tokens.push_back(intermediate);
	}
}

/**
 * Parse the problem definition part.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_prob_blk(vector<string>tokens)
{
	if(tokens[0].compare("Tdbase_name") == 0) {
		tdbase_name = tokens[2];
	} else if(tokens[0].compare("SqDbase_name") == 0) {
		sq_dbase_name = tokens[2];
	} else if(tokens[0].compare("Rule_name") == 0) {
		rule_name = tokens[2];
	} else if(tokens[0].compare("Out_name") == 0) {
		out_name = tokens[2];
	} else if(tokens[0].compare("Period") == 0) {
		period = stoi(tokens[2]);
	}
}

/**
 * Parse the kind of algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_alg_blk(vector<string>tokens)
{
	switch(solver) {
	case SOLVER_DE:
		parse_alg_de(tokens);
		break;
	case SOLVER_PSO:
		parse_alg_pso(tokens);
		break;
	}
}

/**
 * Parse the parameter setting of the DE algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_alg_de(vector<string>tokens)
{
	if (tokens[0].compare("DE_PARAM") == 0) {
		cout << "DE_PARAM started..." << endl;
	} else if (tokens[0].compare("DE_NP") == 0) {
		Np = stoi(tokens[2]);
	} else if (tokens[0].compare("DE_FES") == 0) {
		FEs = stoi(tokens[2]);
	} else if (tokens[0].compare("DE_RUNS") == 0) {
		RUNs = stoi(tokens[2]);
	} else if (tokens[0].compare("DE_F") == 0) {
		alg_param.de.scale = stof(tokens[2]);
	} else if (tokens[0].compare("DE_CR") == 0) {
		alg_param.de.xover = stof(tokens[2]);
	} else if (tokens[0].compare("DE_STRATEGY") == 0) {
		alg_param.de.strategy = stoi(tokens[2]);
	}
}

/**
 * Parse the parameter setting of the PSO algorithm for solving ARM - not implemened yet.
 *
 * @param the output file.
 * @return no return code.
 */
void Setup::parse_alg_pso(vector<string>tokens)
{

}

/**
 * Parse the kind of algorithm for ARM solving.
 *
 * @param the name of an algorithm.
 * @return integer return code identifying the selected algorithm.
 */
int Setup::alg_to_int(string alg)
{
	int n_alg = SOLVER_NONE;

	if(alg.compare("DE") == 0) {
		n_alg = SOLVER_DE;
	} else if(alg.compare("PSO") == 0) {
		n_alg = SOLVER_PSO;
	}
	return n_alg;
}

/**
 * Parse the visualization part of parameter setting.
 *
 * @param the vector of tokens.
 * @return no return code.
 */
void Setup::parse_vis_blk(vector<string>tokens)
{
	switch(visual) {
	case VISUAL_FLOW:
		parse_vis_flow(tokens);
		break;
	}
}

/**
 * Parse the visualization part determining the flow visualization - not implemented yet.
 *
 * @param the vector of tokens.
 * @return no return code.
 */
void Setup::parse_vis_flow(vector<string>tokens)
{
	if(tokens[0].compare("FLOW_M") == 0) {
		vis_param.flow.M = stoi(tokens[2]);
	} else if(tokens[0].compare("FLOW_N") == 0) {
		vis_param.flow.N = stoi(tokens[2]);
	}
}

/**
 * Map the visualization method from string.
 *
 * @param the string.
 * @return integer return code identifying the visualization method.
 */
int Setup::vis_to_int(string vis)
{
	int n_vis = VISUAL_NONE;

	if(vis.compare("FLOW") == 0) {
		n_vis = VISUAL_FLOW;
	} else if(vis.compare("METRO") == 0) {
		n_vis = VISUAL_METRO;
	}
	return n_vis;
}

/**
 * Parse the kind of squashing algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_sq_blk(vector<string>tokens)
{
	switch(squash) {
	case SQUASH_CAUCHY:
		parse_sq_cauchy(tokens);
		break;
	case SQUASH_EUCLID:
		parse_sq_euclid(tokens);
		break;
	}
}

/**
 * Parse the parameter setting of the Cauchy squashing algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_sq_cauchy(vector<string>tokens)
{
	if (tokens[0].compare("CAUCHY_PARAM") == 0) {
		cout << "CAUCHY_PARAM started..." << endl;
	} else if (tokens[0].compare("THRESHOLD") == 0) {
		squ_param.sq.Thresh = stof(tokens[2]);
	}
}

/**
 * Parse the parameter setting of the Euclidian squashing algorithm for solving ARM.
 *
 * @param the vector of parsed tokens.
 * @return no return code.
 */
void Setup::parse_sq_euclid(vector<string>tokens)
{
	if (tokens[0].compare("EUCLID_PARAM") == 0) {
		cout << "EUCLID_PARAM started..." << endl;
	} else if (tokens[0].compare("THRESHOLD") == 0) {
		squ_param.sq.Thresh = stof(tokens[2]);
	}
}

/**
 * Map the squashing method from string.
 *
 * @param the string.
 * @return integer return code identifying the visualization method.
 */
int Setup::sq_to_int(string sq)
{
	int n_sq = SQUASH_NONE;

	if(sq.compare("CAUCHY") == 0) {
		n_sq = SQUASH_CAUCHY;
	} else if(sq.compare("EUCLID") == 0) {
		n_sq = SQUASH_EUCLID;
	}
	return n_sq;
}

/**
 * Print out all three parameter parts determining a behavior of the solver.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_param()
{
    print_prob_blk();
    print_alg_blk();
    print_vis_blk();
    print_sq_blk();
}

/**
 * Print out the problem definition part.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_prob_blk()
{
	cout << "Problem block setup: " << endl;
	cout << "---------------------" << endl;
	cout << "Tdbase_name= " << tdbase_name << endl;
	cout << "SqDbase_name= " << sq_dbase_name << endl;
	cout << "Rule_name= " << rule_name << endl;
	cout << "Out_name= " << out_name << endl;
	cout << "Period= " << period << endl;
//	cout << "Intervals= " << intervals << endl;
	cout << "---------------------" << endl;
}

/**
 * Print out the algorithm's parameter setting.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_alg_blk()
{
	cout << "Optimization block setup: " << endl;
	cout << "---------------------" << endl;

	switch(solver) {
	case SOLVER_DE:
		cout << "Algorithm= DE" << endl;
		cout << "NP= " << Np << endl;
		cout << "FES= " << FEs << endl;
		cout << "RUNS= " << RUNs << endl;
		cout << "F= " << alg_param.de.scale << endl;
		cout << "CR= " << alg_param.de.xover << endl;
		cout << "Strategy= " << alg_param.de.strategy << endl;
		break;
	case SOLVER_PSO:
		cout << "Algorithm= PSO" << endl;
		break;
	default:
		cout << "Algorithm= NONE" << endl;
	}
	cout << "---------------------" << endl;
}

/**
 * Print out the parameter setting of the visualization method.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_vis_blk()
{
	cout << "Visualization block setup: " << endl;
	cout << "---------------------" << endl;

	switch(visual) {
	case VISUAL_FLOW:
		cout << "Method= FLOW" << endl;
		cout << "M= " << vis_param.flow.M << endl;
		cout << "N= " << vis_param.flow.N << endl;
		break;
	case VISUAL_METRO:
		cout << "Method= METRO_MAP" << endl;
		break;
	default:
		cout << "Method= NONE" << endl;
	}
	cout << "---------------------" << endl;
}

/**
 * Print out the parameter setting of the squashing method.
 *
 * @param no input parameters.
 * @return no return code.
 */
void Setup::print_sq_blk()
{
	cout << "Squashing block setup: " << endl;
	cout << "---------------------" << endl;

	switch(squash) {
	case SQUASH_CAUCHY:
		cout << "Method= CAUCHY" << endl;
		cout << "Threshold= " << squ_param.sq.Thresh << endl;
		break;
	case SQUASH_EUCLID:
		cout << "Method= EUCLID" << endl;
		break;
	default:
		cout << "Method= NONE" << endl;
	}
	cout << "---------------------" << endl;
}

/**
 * Print out the tokens parsed from the input line.
 *
 * @param the vector of tokens.
 * @return no return code.
 */
void Setup::print_tokens(vector<string>tokens)
{
	cout << "Tokens printout:" << endl;
	cout << "---------------------" << endl;
	for(uint i=0;i<tokens.size();i++) {
		cout << i << ": " << tokens[i] << endl;
	}
	cout << "---------------------" << endl;
}

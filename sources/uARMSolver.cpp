/**
 * Association Rule Mining Test Program
 * Based on nature-inspired algorithms (DE, PSO etc.)
 * Written By: Iztok Fister
 *             FERI Maribor
 *             Maribor, Slovenia
 *             iztok.fister@um.si
 * Created: July 2020
 * Last Modified: 11/9/20
 * Revision: 1.0
 *
 * Help function (add version number) - fixed at 2.10.2020 by Fister Iztok.
 *
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "DESolver.h"
#include "Setup.h"
#include "Archive.h"
#include "Problem.h"

#define N_DIM 10
#define N_POP 100
#define N_FES (1000*N_DIM)
#define MAX_GENERATIONS	1000
#define STRT_FUN 0
#define END_FUN 10
#define MAX_RUN 25
#define F 0.5
#define CR 0.9


	void help(FILE *stream = stdout);
	void solve(Setup setup, Problem problem, Archive &rules);
	void visualize(Setup setup, Archive rules);

using namespace std;

/**
 * Main C++ function.
 *
 * @param command line from which the program is launched.
 * @return return code (0, if program stops normally).
 */
int main(int argc, char* argv[])
{
	string s_name = "arm.set";
	
    for(int i=1;i<argc;i++)
    {
    	if((strncmp(argv[i], "-v", 2) == 0) || (strncmp(argv[i], "-?", 2) == 0))
    	{
    		help();
		return EXIT_SUCCESS;
    	}
	else if(strncmp(argv[i], "-s", 2) == 0)	// setup file name
    	{
		s_name.assign(&argv[i][2]);
		// allow -s FILE, not just -sFILE:
		if (s_name.empty() && i < argc - 1)
		{
			s_name.assign(argv[++i]);
		}
	} else {
		fprintf(stderr, "Unexpected argument: %s\n\n", argv[i]);
		help(stderr);
		return EXIT_FAILURE;
	}
    }

    srand(1);

    printf("ARM setup: setup_file_name = %s.\n", s_name.c_str());

    // read setup file
    Setup setup(s_name);

    if(!setup.read()) {
        printf("ARM setup: File %s not read.\n", s_name.c_str());
        exit (-1);
    }

    setup.print_param();

    // problem definition - read transaction database(s)
    Problem prob[setup.get_period()];

	for (int i = 0; i < setup.get_period(); i++) {	// number of problems
		string str = setup.get_tdbase_name();
		if(setup.get_period() > 1) {
			char f_name[256];
			sprintf(f_name, ".%d", i + 1);
			str.append(f_name);
		}

		printf("Reading file= %d, f_name= %s...\n", i + 1, str.c_str());
	    // read transaction database(s)
		prob[i].init_tdbase(setup, str);
	}

	Archive rules[setup.get_period()];

	if (setup.get_solver() == SOLVER_NONE) {		// reading the existing ARM archive
		for (int i = 0; i < setup.get_period(); i++) {	// number of periods
			string str = setup.get_rule_name();
			if(setup.get_period() > 1) {
				char f_name[256];
				sprintf(f_name, ".%d", i + 1);
				str.append(f_name);
			}

			printf("Reading file= %d, f_name= %s...\n", i + 1, str.c_str());
		    // read transaction database(s)
			rules[i].read(str, prob[i]);
		}
	} else {		// producing the new ARM archive
		for (int i = 0; i < setup.get_period(); i++) {	// number of periods
			// evaluate solution
			solve(setup, prob[i], rules[i]);
			// obtain output file name
			string str = setup.get_out_name();
			if(setup.get_period() > 1) {
				char f_name[256];
				sprintf(f_name, ".%d", i + 1);
				str.append(f_name);
			}
			printf("Writing file= %d, f_name= %s...\n", i + 1, str.c_str());
		    // write archive of rules found
			rules[i].asort();	// sort the archive
			rules[i].write(str, prob[i]);
		}
	}
	exit(-1);

	// visualization
	visualize(setup, rules[0]);

	return 0;
}

/**
 * Display syntax of the command.
 *
 * @param no parameters.
 * @return no return value.
 */
void help(FILE * stream)
{
	fprintf(stream, "uARMSolver version 1.0 (October 2020)\n\n");	// fixed at 2.10.2020 by Fister Iztok
	fprintf(stream, "Syntax\n");
	fprintf(stream, "  uARMSolver [-v|-?] [-s'arm.set'|-s 'arm.set']\n");
}

/**
 * Call the appropriate ARM problem solver.
 *
 * @param incorporate parameter setup, problem definition and produced archive of association rules.
 * @return no return value.
 */
void solve(Setup setup, Problem problem, Archive &rules)
{
	clock_t start_t, end_t; 	// time measuring in miliseconds

	start_t = clock();

	switch(setup.get_solver()) {
	case SOLVER_DE: {
		int n_dim = problem.get_dimension();
		int n_np = setup.get_Np();
		int n_run = setup.get_RUNs();

		DESolver solver(n_dim, n_np, problem);
		solver.Setup(setup.alg_param.de.strategy, setup.alg_param.de.scale, setup.alg_param.de.xover);
		for(int i=0;i<n_run;i++) {
			int n_fes = setup.get_FEs();
			solver.Evolve(i, n_fes, rules);
		}
	}
	break;
	case SOLVER_PSO:
		break;
	}

	end_t = clock();
	printf("Total time= %lf\n", (double) (end_t - start_t) / CLOCKS_PER_SEC);
}

/**
 * Prepare data needed for visualization - not implemented yet.
 *
 * @param incorporate parameter setup, and archive of association rules.
 * @return no return value.
 */
void visualize(Setup set, Archive rules)
{

}

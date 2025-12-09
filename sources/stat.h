/*
 * stat.h
 *
 *  Created on: Nov 21, 2011
 *      Author: iztok
 */

#ifndef SOURCES_STAT_H_
#define SOURCES_STAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>

#define MAX_RUNS		100
#define N				5

#define COMP_BEST		0x01
#define COMP_MEDIANA	0x02
#define COMP_WORST		0x04
#define COMP_MEAN		0x08
#define COMP_STDEV		0x10
#define COMP_ALL		0xff

using namespace std;

class stat {
public:
	stat();
	virtual ~stat();
private:
	int num_runs;

	double data[MAX_RUNS][2];

public:
	double best[2];
	double median[2];
	double worst[2];
	double mean[2];
	double stdev[2];

public:
	void init(int n_run);
	void eval();
	void add(int run, double fit, int n_rules);
	void dump();
};

#endif /* SOURCES_STAT_H_ */

/*
 * stat.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: iztok
 */

#include "stat.h"

#include <float.h>
#include <math.h>

#define sqr(a)	((a)*(a))


stat::stat() {
	num_runs = 0;
}

stat::~stat() {
}

void stat::init(int n_run) {
	num_runs = n_run;
//cout << "STAT: init num_func= " << num_func << " num_runs= " << num_runs << " num_parts= " << num_parts << endl;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < num_runs; i++) {
			data[i][j] = 0.0;
		}
		best[j] = 0.0;
		median[j] = 0.0;
		worst[j] = 0.0;
		mean[j] = 0.0;
		stdev[j] = 0.0;
	}
}

void stat::add(int run, double fit, int n_rules)
{
	data[run][0] = fit;
	data[run][1] = n_rules;
}

void stat::eval() {
	double median_t[num_runs][2];

	for (int j = 0; j < 2; j++) {
cout << "j= " << j << ", num_runs=" << num_runs << endl;
		worst[j] = data[0][j];
		best[j] = data[0][j];
		mean[j] += data[0][j];
		median_t[0][j] = data[0][j];

		for (int i = 1; i < num_runs; i++) {
			if (data[i][j] < worst[j])
				worst[j] = data[i][j];
			if (data[i][j] > best[j])
				best[j] = data[i][j];
			mean[j] += data[i][j];
			median_t[i][j] = data[i][j];
		}
cout << "Ok... " << endl;
		mean[j] /= (double) num_runs;
cout << "OkOk... " << endl;
		stdev[j] = 0.0;
		for (int i = 0; i < num_runs; i++) {
			stdev[j] += sqr(data[i][j] - mean[j]);
			for (int k = i + 1; k < num_runs; k++) {
				if (median_t[i][j] > median_t[k][j]) {
					double x = median_t[i][j];
					median_t[i][j] = median_t[k][j];
					median_t[k][j] = x;
				}
			}
		}
cout << "StDev[" << j << "]= " << stdev[j] << endl;
		stdev[j] = (num_runs <= 1) ? 0.0 : sqrt(stdev[j] / (double) (num_runs - 1));
		median[j] = median_t[num_runs / 2][j];
	}
}

void stat::dump() {
	cout << "Dump statistics: " << endl;
	for(int i=0;i<2;i++) {
	cout << "[" << i << "]: " << best[i] << "," << worst[i] << ","
			<< mean[i] << "," << median[i] << "," << stdev[i] << endl;
	}
}

#undef sqr

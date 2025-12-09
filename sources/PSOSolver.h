/*
 * Swarm.h
 *
 *  Created on: Jul 2, 2025
 *      Author: iztok
 */

#ifndef PSOSOLVER_H
#define PSOSOLVER_H

#include "Particle.h"
#include "Archive.h"
#include "Problem.h"
#include "Archive.h"
#include "Evaluate.h"

class PSOSolver;

using namespace std;

class PSOSolver {
public:
	PSOSolver(int dim, int popSize, Problem problem);
	~PSOSolver(void);

	void Setup(double w, double c1, double c2, int n_intervals);
    void optimize(int run, int maxFEs, Archive &rules);
    void print_swarm();
	double GetBestEnergy() { return gBestFitness; }

private:
    std::vector<Particle> particles;
    std::vector<double> gBestPosition;
    double gBestFitness;
    int dimension;
    int nDim;
    int numParticles;
	int intervals;
    double inertia;
    double social;
    double cognitive;

	Problem prob;		///< problem definition
	Evaluate eval;		///< evaluation of a fitness function
};

#endif

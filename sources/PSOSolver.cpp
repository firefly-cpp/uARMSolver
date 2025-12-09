/*
 * Swarm.cpp
 *
 *  Created on: Jul 2, 2025
 *      Author: iztok
 */


#include <algorithm>
#include "PSOSolver.h"


PSOSolver::PSOSolver(int dim, int popSize, Problem problem) : gBestFitness(0.0), nDim(dim), numParticles(popSize), inertia(0.9), social(0.1), cognitive(0.1) {
	prob = problem;
	dimension = eval.encode(dim, prob);
    particles.reserve(numParticles);
}

PSOSolver::~PSOSolver(void) {
    particles.clear();
    gBestPosition.clear();
}

void PSOSolver::Setup(double w, double c1, double c2, int n_intervals) {
	inertia	= w;
	cognitive = c1;
	social = c2;
	intervals	= n_intervals;
    for (int i = 0; i < numParticles; i++) {
		Particle row(dimension, inertia, cognitive, social);
		particles.push_back(row);
		gBestPosition.push_back(0.0);
    }
}

void PSOSolver::optimize(int run, int maxFEs, Archive &rules) {
	int maxGenerations = maxFEs/numParticles;	// determine maximum number of evaluation
	if((maxGenerations%numParticles) > 0)
		maxGenerations++;

	gBestFitness = 0.0;
//	int i = 0;

    for (int iter = 0; iter < maxGenerations; iter++) {
    	cout << "generation= " << iter << endl;
        for (auto& particle : particles) {
			Rule rule;
			particle.fitness = eval.EnergyFunction(particle.position, prob, rule, intervals);
//cout << "trial= " << ++i << " trialEnergy= " << particle.fitness << " vs. globalBestFitness" << gBestFitness << " ===> " << endl;

        	// determining the personal best solution
			if (particle.fitness > particle.pBestFitness) {
                particle.pBestFitness = particle.fitness;
                particle.pBestPosition = particle.position;
            	rules.add(rule);

            	// determining the global best solotion
                if (particle.fitness > gBestFitness) {
                    gBestFitness = particle.fitness;
                    gBestPosition = particle.position;
cout << "bestFitness= " << gBestFitness << " at " << iter << " generation." << endl;
					if(gBestFitness == 1.0)
						return;
                }
            }
        }
//		print_swarm();
//exit(-1);
        for (auto& particle : particles) {
            particle.updateVelocity(gBestPosition);
            particle.updatePosition();
        }
    }
}

void PSOSolver::print_swarm() {
	cout << "Print swarm:" << endl;
    for (int i = 0; i < numParticles; i++) {
    	cout << "[" << i << "] : (";
    	for(int j=0;j<dimension;j++) {
    		cout << particles[i].position[j] << ",";
    	}
    	cout << ") --> fit= " << particles[i].fitness << endl;
    }
}


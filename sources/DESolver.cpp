#include "DESolver.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory.h>
#include <math.h>

#define sqr(a) 	((a)*(a))

/**
 * Initialization of ARM solver using DE.
 *
 * @param dimension of the problem in a real solution space (i.e., the number of feature into transaction database), population size, and a definition of the problem.
 * @return no return code.
 */
DESolver::DESolver(int dim,int popSize,	Problem problem) :
					D(dim), nDim(0), nPop(popSize),
					generations(0), strategy(stRand1Exp),
					scale(0.7), probability(0.5),
					bestEnergy(0.0),
					calcTrialSolution(NULL)
{
	

	trialSolutions = vector<vector<double> >(nPop);
	trialEnergies = vector<double>(nPop);
	generationRules = vector<Rule>(nPop);
	distributionUni = uniform_real_distribution<double>(0.0, 1.0);
	distributionPop = uniform_int_distribution<int>(0, popSize-1);

	prob = problem;
	nDim = eval.encode(D, prob);

	for(int i=0;i<nDim;i++) {
		for (int k=0;k<nPop;k++)
			trialSolutions.at(k).push_back(0.0);
		bestSolution.push_back(0.0);
	}
	for(int i=0;i<nPop;i++) {
		popEnergy.push_back(0-0);
		vector<double> row;
		for(int j=0;j<nDim;j++) {
			row.push_back(0.0);
		}
		population.push_back(row);
	}

	return;
}

/**
 * Release dynamic allocated classes.
 *
 * @param no parameters.
 * @return no return code.
 */
DESolver::~DESolver(void)
{
	trialSolutions.clear();
	bestSolution.clear();
	popEnergy.clear();
	population.clear();

	return;
}

/**
 * Setup of the DE solver.
 *
 * @param DE mutation strategy, scale factor, and crossover probability algorithm's parameters.
 * @return no return code.
 */
void DESolver::Setup(int deStrategy, double diffScale, double crossoverProb)
{
	int i;

	strategy	= deStrategy;
	scale		= diffScale;
	probability = crossoverProb;
	
	for (i=0; i < nPop; i++)
	{
		for (int j=0; j < nDim; j++)
			population[i][j] = distributionUni(generator);

		popEnergy[i] = 0;
	}

	for (i=0; i < nDim; i++)
		bestSolution[i] = 0.0;

	switch (strategy)
	{
		case stBest1Exp:
			calcTrialSolution = &DESolver::Best1Exp;
			break;

		case stRand1Exp:
			calcTrialSolution = &DESolver::Rand1Exp;
			break;

		case stRandToBest1Exp:
			calcTrialSolution = &DESolver::RandToBest1Exp;
			break;

		case stBest2Exp:
			calcTrialSolution = &DESolver::Best2Exp;
			break;

		case stRand2Exp:
			calcTrialSolution = &DESolver::Rand2Exp;
			break;

		case stBest1Bin:
			calcTrialSolution = &DESolver::Best1Bin;
			break;

		case stRand1Bin:
			calcTrialSolution = &DESolver::Rand1Bin;
			break;

		case stRandToBest1Bin:
			calcTrialSolution = &DESolver::RandToBest1Bin;
			break;

		case stBest2Bin:
			calcTrialSolution = &DESolver::Best2Bin;
			break;

		case stRand2Bin:
			calcTrialSolution = &DESolver::Rand2Bin;
			break;
	}

	return;
}

/**
 * Optimization step of the association rule miner based on DE.
 *
 * @param the number of independent run, the maximum number of fitness function evaluations, and the archive of the mined association rules.
 * @return no return code.
 */
void DESolver::Evolve(int run, int maxFEs, Archive &rules)
{
	int maxGenerations;

	bestEnergy = 0;
	maxGenerations = maxFEs/nPop;	// determine maximum number of evaluation
	if((maxGenerations%nPop) > 0)
		maxGenerations++;

	for (int generation=0;generation < maxGenerations;generation++) {
cout << "generation= " << generation << endl;
		#pragma omp parallel for
		for (int candidate=0; candidate < nPop; candidate++)
		{
			(this->*calcTrialSolution)(candidate);
			trialEnergies.at(candidate) = eval.EnergyFunction(trialSolutions.at(candidate), prob, generationRules.at(candidate));
		}
		for (int candidate=0; candidate < nPop; candidate++)
		{
			if (trialEnergies.at(candidate) > popEnergy[candidate])
			{
				// New low for this candidate
				popEnergy[candidate] = trialEnergies.at(candidate);
				population[candidate] = trialSolutions.at(candidate);
				rules.add(generationRules.at(candidate));

				// Check if all-time low
				if (trialEnergies.at(candidate) > bestEnergy)
				{
					bestEnergy = trialEnergies.at(candidate);
					bestSolution = trialSolutions.at(candidate);
cout << "bestFitness= " << bestEnergy << " at " << generation << " generation." << endl;
				}
			}
		}
	}

	return;
}

/**
 * Implementation of 'DE/Best/1/Exp' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Best1Exp(int candidate)
{
	int n;
	vector<int> r = {candidate,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; (distributionUni(generator) < probability) && (i < nDim); i++) 
	{
		trialSolutions.at(candidate)[n] = bestSolution[n] + scale * (population[r.at(1)][n] - population[r.at(2)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Rand/1/Exp' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Rand1Exp(int candidate)
{
	int n;
	vector<int> r = {candidate,0,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; (distributionUni(generator) < probability) && (i < nDim); i++) 
	{
		trialSolutions.at(candidate)[n] = population[r.at(1)][n] + scale * (population[r.at(2)][n] - population[r.at(3)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/RandToBest/1/Exp' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::RandToBest1Exp(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; (distributionUni(generator) < probability) && (i < nDim); i++) 
	{
		trialSolutions.at(candidate)[n] += scale * (bestSolution[n] - trialSolutions.at(candidate)[n]) + scale * (population[r.at(1)][n] - population[r.at(2)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Best/2/Exp' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Best2Exp(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; (distributionUni(generator) < probability) && (i < nDim); i++) 
	{
		trialSolutions.at(candidate)[n] = bestSolution[n] + scale * (population[r.at(1)][n] + population[r.at(2)][n] - population[r.at(3)][n] - population[r.at(4)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Rand/2/Exp' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Rand2Exp(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0,0,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; (distributionUni(generator) < probability) && (i < nDim); i++) 
	{
		trialSolutions.at(candidate)[n] = population[r.at(1)][n] + scale * (population[r.at(2)][n] + population[r.at(3)][n] - population[r.at(4)][n] - population[r.at(5)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Best/1/Bin' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Best1Bin(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((distributionUni(generator) < probability) || (i == (nDim - 1)))
			trialSolutions.at(candidate)[n] = bestSolution[n] + scale * (population[r.at(1)][n] - population[r.at(2)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Rand/1/Bin' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Rand1Bin(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((distributionUni(generator) < probability) || (i  == (nDim - 1))) {
			trialSolutions.at(candidate)[n] = population[r.at(1)][n] + scale * (population[r.at(2)][n] - population[r.at(3)][n]);
			if(trialSolutions.at(candidate)[n] < 0.0)
				trialSolutions.at(candidate)[n] = 0.0;
			if(trialSolutions.at(candidate)[n] > 1.0)
				trialSolutions.at(candidate)[n] = 1.0;
		}
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/RandToBest/1/Bin' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::RandToBest1Bin(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((distributionUni(generator) < probability) || (i  == (nDim - 1)))
			trialSolutions.at(candidate)[n] += scale * (bestSolution[n] - trialSolutions.at(candidate)[n]) + scale * (population[r.at(1)][n] - population[r.at(2)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Best/2/Bin' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Best2Bin(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((distributionUni(generator) < probability) || (i  == (nDim - 1)))
			trialSolutions.at(candidate)[n] = bestSolution[n] + scale * (population[r.at(1)][n] + population[r.at(2)][n] - population[r.at(3)][n] - population[r.at(4)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Implementation of 'DE/Rand/2/Bin' mutation strategy.
 *
 * @param index pointing the target solution.
 * @return no return code.
 */
void DESolver::Rand2Bin(int candidate)
{
	int n;

	vector<int> r = {candidate,0,0,0,0,0};
	SelectSamples(r);
	n = distributionPop(generator);

	trialSolutions.at(candidate) = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((distributionUni(generator) < probability) || (i  == (nDim - 1)))
			trialSolutions.at(candidate)[n] = population[r.at(1)][n] + scale * (population[r.at(2)][n] + population[r.at(3)][n] - population[r.at(4)][n] - population[r.at(5)][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Select candidate solutions entering into DE mutation strategies.
 *
 * @param r a vector of indexes where the first component must be equal to the the target vector.
 * @return up to five randomly selected indexes. If the alias of the index is not entered, index 0 is assumed.
 */
void DESolver::SelectSamples(vector<int> &r)
{
	int proposedIndex;
	for (size_t k = 0; k < r.size(); k++) {
		do {
			proposedIndex = distributionPop(generator);
		} while(find(r.begin(), r.begin()+k, proposedIndex)!=r.begin()+k);
		r.at(k) = proposedIndex;
	}
	return;
}

/*------Constants for RandomUniform()---------------------------------------*/
#define SEED 3
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

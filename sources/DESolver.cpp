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
					trialEnergy(0.0), bestEnergy(0.0),
					calcTrialSolution(NULL)
{
	prob = problem;
	nDim = eval.encode(D, prob);
cout << "DESolver: dim= " << nDim << ", nPop= " << nPop << endl;
	for(int i=0;i<nDim;i++) {
		trialSolution.push_back(0.0);
		bestSolution.push_back(0.0);
	}
	for(int i=0;i<nPop;i++) {
		popEnergy.push_back(0.0);
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
	trialSolution.clear();
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
void DESolver::Setup(int deStrategy, double diffScale, double crossoverProb, int n_intervals)
{
	int i;

	strategy	= deStrategy;
	scale		= diffScale;
	probability = crossoverProb;
	intervals	= n_intervals;
cout << "Initialisation of population..." << endl;
	for (i=0; i < nPop; i++)
	{
		for (int j=0; j < nDim; j++)
			population[i][j] = RandomUniform(0.0,1.0);

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
		for (int candidate=0; candidate < nPop; candidate++)
		{
			Rule rule;
			(this->*calcTrialSolution)(candidate);
			trialEnergy = eval.EnergyFunction(trialSolution, prob, rule, intervals);
			if (trialEnergy > popEnergy[candidate])
			{
				// New low for this candidate
				popEnergy[candidate] = trialEnergy;
				population[candidate] = trialSolution;
				rules.add(rule);

				// Check if all-time low
				if (trialEnergy > bestEnergy)
				{
					bestEnergy = trialEnergy;
					bestSolution = trialSolution;
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
	int r1, r2;
	int n;

	SelectSamples(candidate,&r1,&r2);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; (RandomUniform(0.0,1.0) < probability) && (i < nDim); i++) 
	{
		trialSolution[n] = bestSolution[n] + scale * (population[r1][n] - population[r2][n]);
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
	int r1, r2, r3;
	int n;

	SelectSamples(candidate,&r1,&r2,&r3);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; (RandomUniform(0.0,1.0) < probability) && (i < nDim); i++) 
	{
		trialSolution[n] = population[r1][n] + scale * (population[r2][n] - population[r3][n]);
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
	int r1, r2;
	int n;

	SelectSamples(candidate,&r1,&r2);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; (RandomUniform(0.0,1.0) < probability) && (i < nDim); i++) 
	{
		trialSolution[n] += scale * (bestSolution[n] - trialSolution[n]) + scale * (population[r1][n] - population[r2][n]);
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
	int r1, r2, r3, r4;
	int n;

	SelectSamples(candidate,&r1,&r2,&r3,&r4);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; (RandomUniform(0.0,1.0) < probability) && (i < nDim); i++) 
	{
		trialSolution[n] = bestSolution[n] + scale * (population[r1][n] + population[r2][n] - population[r3][n] - population[r4][n]);
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
	int r1, r2, r3, r4, r5;
	int n;

	SelectSamples(candidate,&r1,&r2,&r3,&r4,&r5);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; (RandomUniform(0.0,1.0) < probability) && (i < nDim); i++) 
	{
		trialSolution[n] = population[r1][n] + scale * (population[r2][n] + population[r3][n] - population[r4][n] - population[r5][n]);
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
	int r1, r2;
	int n;

	SelectSamples(candidate,&r1,&r2);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((RandomUniform(0.0,1.0) < probability) || (i == (nDim - 1)))
			trialSolution[n] = bestSolution[n] + scale * (population[r1][n] - population[r2][n]);
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
	int r1, r2, r3;
	int n;

	SelectSamples(candidate,&r1,&r2,&r3);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((RandomUniform(0.0,1.0) < probability) || (i  == (nDim - 1))) {
			trialSolution[n] = population[r1][n] + scale * (population[r2][n] - population[r3][n]);
			if(trialSolution[n] < 0.0)
				trialSolution[n] = 0.0;
			if(trialSolution[n] > 1.0)
				trialSolution[n] = 1.0;
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
	int r1, r2;
	int n;

	SelectSamples(candidate,&r1,&r2);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((RandomUniform(0.0,1.0) < probability) || (i  == (nDim - 1)))
			trialSolution[n] += scale * (bestSolution[n] - trialSolution[n]) + scale * (population[r1][n] - population[r2][n]);
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
	int r1, r2, r3, r4;
	int n;

	SelectSamples(candidate,&r1,&r2,&r3,&r4);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((RandomUniform(0.0,1.0) < probability) || (i  == (nDim - 1)))
			trialSolution[n] = bestSolution[n] + scale * (population[r1][n] + population[r2][n] - population[r3][n] - population[r4][n]);
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
	int r1, r2, r3, r4, r5;
	int n;

	SelectSamples(candidate,&r1,&r2,&r3,&r4,&r5);
	n = (int)RandomUniform(0.0,(double)nDim);

	trialSolution = population[candidate];
	for (int i=0; i < nDim; i++) 
	{
		if ((RandomUniform(0.0,1.0) < probability) || (i  == (nDim - 1)))
			trialSolution[n] = population[r1][n] + scale * (population[r2][n] + population[r3][n] - population[r4][n] - population[r5][n]);
		n = (n + 1) % nDim;
	}

	return;
}

/**
 * Select candidate solutions entering into DE mutation strategies.
 *
 * @param an index of the target vectors together with references to randomly selected candidate indexes.
 * @return up to five randomly selected indexes. If the alias of the index is not entered, index 0 is assumed.
 */
void DESolver::SelectSamples(int candidate,int *r1,int *r2,
										int *r3,int *r4,int *r5)
{
	if (r1)
	{
		do
		{
			*r1 = (int)RandomUniform(0.0,(double)nPop);
		}
		while (*r1 == candidate);
	}

	if (r2)
	{
		do
		{
			*r2 = (int)RandomUniform(0.0,(double)nPop);
		}
		while ((*r2 == candidate) || (*r2 == *r1));
	}

	if (r3)
	{
		do
		{
			*r3 = (int)RandomUniform(0.0,(double)nPop);
		}
		while ((*r3 == candidate) || (*r3 == *r2) || (*r3 == *r1));
	}

	if (r4)
	{
		do
		{
			*r4 = (int)RandomUniform(0.0,(double)nPop);
		}
		while ((*r4 == candidate) || (*r4 == *r3) || (*r4 == *r2) || (*r4 == *r1));
	}

	if (r5)
	{
		do
		{
			*r5 = (int)RandomUniform(0.0,(double)nPop);
		}
		while ((*r5 == candidate) || (*r5 == *r4) || (*r5 == *r3)
													|| (*r5 == *r2) || (*r5 == *r1));
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

/**
 * Proprietary random number generator.
 *
 * @param minimum and maximum values determining the interval of feasible random numbers.
 * @return double return code: denoting the random number within the predefined interval.
 */
double DESolver::RandomUniform(double minValue,double maxValue)
{
	long j;
	long k;
	static long idum;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	double result;

	if (iy == 0)
		idum = SEED;

	if (idum <= 0)
	{
		if (-idum < 1)
			idum = 1;
		else
			idum = -idum;

		idum2 = idum;

		for (j=NTAB+7; j>=0; j--)
		{
			k = idum / IQ1;
			idum = IA1 * (idum - k*IQ1) - k*IR1;
			if (idum < 0) idum += IM1;
			if (j < NTAB) iv[j] = idum;
		}

		iy = iv[0];
	}

	k = idum / IQ1;
	idum = IA1 * (idum - k*IQ1) - k*IR1;

	if (idum < 0)
		idum += IM1;

	k = idum2 / IQ2;
	idum2 = IA2 * (idum2 - k*IQ2) - k*IR2;

	if (idum2 < 0)
		idum2 += IM2;

	j = iy / NDIV;
	iy = iv[j] - idum2;
	iv[j] = idum;

	if (iy < 1)
		iy += IMM1;

	result = AM * iy;

	if (result > RNMX)
		result = RNMX;

	result = minValue + result * (maxValue - minValue);
	return(result);
}

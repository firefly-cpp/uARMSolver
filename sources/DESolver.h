// Differential Evolution Solver Class
// Based on algorithms developed by Dr. Rainer Storn & Kenneth Price
// Written By: Lester E. Godwin
//             PushCorp, Inc.
//             Dallas, Texas
//             972-840-0208 x102
//             godwin@pushcorp.com
// Created: 6/8/98
// Last Modified: 6/8/98
// Revision: 1.0

#if !defined(_DESOLVER_H)
#define _DESOLVER_H

#include <vector>

#include "Archive.h"
#include "Archive.h"
#include "Evaluate.h"
#include "Problem.h"

#define stBest1Exp			0
#define stRand1Exp			1
#define stRandToBest1Exp	2
#define stBest2Exp			3
#define stRand2Exp			4
#define stBest1Bin			5
#define stRand1Bin			6
#define stRandToBest1Bin	7
#define stBest2Bin			8
#define stRand2Bin			9

class DESolver;

typedef void (DESolver::*StrategyFunction)(int);

using namespace std;

/**
 * Definition of the DE algorithm for association rule mining
 *
 * In definition of the algorithm for mining association rules, three
 * main function must be implemented: Setup(), Evolve(), and Energy().
 * The first function determines DE algorithm parameters, like DE mutation
 * strategy, scale factor F and crossover rate CR, the second implements
 * the evolutionary cycle, while the third evaluates the fitness function
 * of the trial solution.
 *
 */
class DESolver
{
public:
	DESolver(int dim,int popSize, Problem problem);
	~DESolver(void);
	
	// Setup() must be called before solve to set min, max, strategy etc.
	// void Setup(double min[],double max[],int deStrategy, double diffScale,double crossoverProb);
	void Setup(int strategy, double scale, double xover);
	// Solve() returns true if EnergyFunction() returns true.
	// Otherwise it runs maxGenerations generations and returns false.
	void Evolve(int run, int maxFEs, Archive &rules);
//	virtual bool Solve(int maxGenerations);
	// Call these functions after Solve() to get results.
	double Energy(void) { return(bestEnergy); }
	
	int Dimension(void) { return(nDim); }
	int Population(void) { return(nPop); }
	int Generations(void) { return(generations); }

protected:
	void SelectSamples(int candidate,int *r1,int *r2=0,int *r3=0, int *r4=0,int *r5=0);
	double RandomUniform(double min,double max);

	int D;
	int nDim;
	int nPop;
	int generations;
	int strategy;

	double scale;
	double probability;
	double trialEnergy;
	double bestEnergy;

	StrategyFunction calcTrialSolution;	///< selected DE mutation strategy

	vector<double> trialSolution;		///< trial solution
	vector<double> bestSolution;		///< the best solution found
	vector<double> popEnergy;			///< fitness function values of the particular vector within population
	vector<vector <double>> population;	///< the current population of individuals

	Problem prob;		///< problem definition
	Evaluate eval;		///< evaluation of a fitness function

private:
	void Best1Exp(int candidate);
	void Rand1Exp(int candidate);
	void RandToBest1Exp(int candidate);
	void Best2Exp(int candidate);
	void Rand2Exp(int candidate);
	void Best1Bin(int candidate);
	void Rand1Bin(int candidate);
	void RandToBest1Bin(int candidate);
	void Best2Bin(int candidate);
	void Rand2Bin(int candidate);

};

#endif // _DESOLVER_H

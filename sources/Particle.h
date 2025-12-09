/*
 * Particle.h
 *
 *  Created on: Jul 2, 2025
 *      Author: iztok
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

class Particle {
public:
    Particle(int dim, double inertia, double cognitive, double social);
    void updateVelocity(const std::vector<double>& gBestPosition);
    void updatePosition();
    void evaluateFitness();

    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> pBestPosition;
    double pBestFitness;
    double fitness;

private:
    int dimension;
    double w = 0.5; // inertia weight
    double c1 = 1.0; // cognitive coefficient
    double c2 = 1.0; // social coefficient
};

#endif

/*
 * Particle.cpp
 *
 *  Created on: Jul 2, 2025
 *      Author: iztok
 */

#include "Particle.h"
#include <algorithm>
#include <cmath>

Particle::Particle(int dim, double inertia, double cognitive, double social) : pBestFitness(0.0), fitness(0.0), dimension(dim), w(inertia), c1(cognitive), c2(social) {
    position.resize(dimension);
    velocity.resize(dimension);
    pBestPosition.resize(dimension);
//    std::srand(std::time(0));
    for (int i = 0; i < dimension; ++i) {
        position[i] = (double)std::rand() / RAND_MAX;
        velocity[i] = (double)std::rand() / RAND_MAX;
    }
}

void Particle::updateVelocity(const std::vector<double>& gBestPosition) {
    for (int i = 0; i < dimension; ++i) {
        double r1 = (double)std::rand() / RAND_MAX;
        double r2 = (double)std::rand() / RAND_MAX;
        velocity[i] = w * velocity[i] + c1 * r1 * (pBestPosition[i] - position[i]) + c2 * r2 * (gBestPosition[i] - position[i]);
    }
}

void Particle::updatePosition() {
    for (int i = 0; i < dimension; ++i) {
        position[i] += velocity[i];
        // Boundary conditions (can be adjusted)
        if (position[i] > 1.0) position[i] = 1.0;
        if (position[i] < 0.0) position[i] = 0.0;
    }
}

void Particle::evaluateFitness() {
    // Fitness function (example: simple sum of squares)
    fitness = 0.0;
    for (double x : position) {
        fitness += x * x;
    }
    if (fitness < pBestFitness) {
        pBestFitness = fitness;
        pBestPosition = position;
    }
}




#ifndef _MyModel_
#define _MyModel_

#include <vector>
#include "DNest4/code/DNest4.h"
#include "MyDistribution.h"

class MyModel
{
	private:
		DNest4::RJObject<MyDistribution> objects;

		double background;
		double extra_sigma; // Noise scale parameter
		double nu; // Noise shape parameter

		// The signal
		std::vector<long double> mu;
		void calculate_mu();
		unsigned int staleness;

	public:
		MyModel();

		// Generate the point from the prior
		void from_prior(DNest4::RNG& rng);

		// Metropolis-Hastings proposals
		double perturb(DNest4::RNG& rng);

		// Likelihood function
		double log_likelihood() const;

		// Print to stream
		void print(std::ostream& out) const;

		// Return string with column information
		std::string description() const;
};

#endif


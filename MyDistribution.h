#ifndef _MyDistribution_
#define _MyDistribution_

#include "DNest4/code/DNest4.h"

// Based on ClassicMassInf1D from RJObject
// Think of "position x" as log-period
// and mass as amplitude
class MyDistribution:public DNest4::ConditionalPrior
{
	private:
		// Parameters of bi-exponential distribution for log-periods
		double center, width;

		// Mean of exponential distribution for amplitudes
		double mu;

	public:
		MyDistribution();

		void from_prior(DNest4::RNG& rng);
        double perturb_hyperparameters(DNest4::RNG& rng);

		double log_pdf(const std::vector<double>& vec) const;
		void from_uniform(std::vector<double>& vec) const;
		void to_uniform(std::vector<double>& vec) const;

		void print(std::ostream& out) const;
		static const int weight_parameter = 1;

};

#endif


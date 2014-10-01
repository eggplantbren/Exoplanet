#include "MyDistribution.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace DNest3;

MyDistribution::MyDistribution(double x_min, double x_max,
					double mu_min, double mu_max)
:x_min(x_min)
,x_max(x_max)
,mu_min(mu_min)
,mu_max(mu_max)
{

}

void MyDistribution::fromPrior()
{
	mu = exp(tan(M_PI*(0.97*randomU() - 0.485)));
}

double MyDistribution::perturb_parameters()
{
	double logH = 0.;

	mu = log(mu);
	mu = (atan(mu)/M_PI + 0.485)/0.97;
	mu += randh();
	wrap(mu, 0., 1.);
	mu = tan(M_PI*(0.97*mu - 0.485));
	mu = exp(mu);

	return logH;
}

// vec[0] = "position" (log-period)
// vec[1] = amplitude
// vec[2] = phase
// vec[3] = v0
// vec[4] = viewing angle

double MyDistribution::log_pdf(const std::vector<double>& vec) const
{
	if(vec[0] < x_min || vec[0] > x_max || vec[1] < 0. ||
			vec[2] < 0. || vec[2] > 2.*M_PI ||
			vec[3] < 0.4 || vec[3] > 1. ||
			vec[4] < 0. || vec[4] > 2.*M_PI)
		return -1E300;

	return -log(mu) - vec[1]/mu;
}

void MyDistribution::from_uniform(std::vector<double>& vec) const
{
	vec[0] = x_min + (x_max - x_min)*vec[0];
	vec[1] = -mu*log(1. - vec[1]);
	vec[2] = 2.*M_PI*vec[2];
	vec[3] = 0.4 + 0.6*vec[3];
	vec[4] = 2.*M_PI*vec[4];
}

void MyDistribution::to_uniform(std::vector<double>& vec) const
{
	vec[0] = (vec[0] - x_min)/(x_max - x_min);
	vec[1] = 1. - exp(-vec[1]/mu);
	vec[2] = vec[2]/(2.*M_PI);
	vec[3] = (vec[3] - 0.4)/0.6;
	vec[4] = vec[4]/(2.*M_PI);
}

void MyDistribution::print(std::ostream& out) const
{
	out<<mu<<' ';
}


#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>
#include "Lookup.h"
#include <gsl/gsl_sf_gamma.h>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:objects(5, 10, false, MyDistribution(-10., 10., 1E-3, 1E3))
,mu(Data::get_instance().get_t().size())
{

}

void MyModel::fromPrior()
{
	objects.fromPrior();
	objects.consolidate_diff();
	sigma = exp(log(1E-3) + log(1E6)*randomU());
	nu = exp(log(0.1) + log(1000.)*randomU());
	calculate_mu();
}

void MyModel::calculate_mu()
{
	// Get the times from the data
	const vector<double>& t = Data::get_instance().get_t();

	// Update or from scratch?
	bool update = (objects.get_added().size() < objects.get_components().size());

	// Get the components
	const vector< vector<double> >& components = (update)?(objects.get_added()):
				(objects.get_components());

	// Zero the signal
	if(!update)
		mu.assign(mu.size(), 0.);

	double T, A, phi, v0, viewing_angle;
	vector<double> arg, evaluations;
	for(size_t j=0; j<components.size(); j++)
	{
		T = exp(components[j][0]);
		A = components[j][1];
		phi = components[j][2];
		v0 = components[j][3];
		viewing_angle = components[j][4];
		arg = t;
		for(size_t i=0; i<arg.size(); i++)
			arg[i] = 2.*M_PI*t[i]/T + phi;
		evaluations = Lookup::get_instance().evaluate(arg, v0, viewing_angle);
		for(size_t i=0; i<t.size(); i++)
			mu[i] += A*evaluations[i];
	}
}

double MyModel::perturb()
{
	double logH = 0.;

	if(randomU() <= 0.75)
	{
		logH += objects.perturb();
		objects.consolidate_diff();
		calculate_mu();
	}
	else
	{
		sigma = log(sigma);
		sigma += log(1E6)*randh();
		sigma = mod(sigma - log(1E-3), log(1E6)) + log(1E-3);
		sigma = exp(sigma);

		nu = log(nu);
		nu += log(1000.)*randh();
		wrap(nu, log(0.1), log(1000.));
		nu = exp(nu);
	}

	return logH;
}

double MyModel::logLikelihood() const
{
	// Get the data
	const vector<double>& y = Data::get_instance().get_y();

	double logL = 0.;
	for(size_t i=0; i<y.size(); i++)
	{
		logL += gsl_sf_lngamma(0.5*(nu + 1.)) - gsl_sf_lngamma(0.5*nu)
			- 0.5*log(M_PI*nu) - log(sigma)
			- 0.5*(nu + 1.)*log(1. + pow((y[i] - mu[i])/sigma, 2)/nu);
	}

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	for(size_t i=0; i<mu.size(); i++)
		out<<mu[i]<<' ';
	out<<sigma<<' '<<nu<<' ';
	objects.print(out); out<<' ';
}

string MyModel::description() const
{
	return string("objects, sigma");
}

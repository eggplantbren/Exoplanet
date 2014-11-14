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
:objects(5, 10, false, MyDistribution())
,mu(Data::get_instance().get_t().size())
{

}

void MyModel::fromPrior()
{
	objects.fromPrior();
	objects.consolidate_diff();
	background = Data::get_instance().get_y_min() +
		(Data::get_instance().get_y_max() - Data::get_instance().get_y_min())*randomU();
	extra_sigma = exp(tan(M_PI*(0.97*randomU() - 0.485)));
	nu = exp(log(0.1) + log(1000.)*randomU());
	calculate_mu();
}

void MyModel::calculate_mu()
{
	// Get the times from the data
	const vector<double>& t = Data::get_instance().get_t();

	// Update or from scratch?
	bool update = (objects.get_added().size() < objects.get_components().size()) &&
			(staleness <= 10);

	// Get the components
	const vector< vector<double> >& components = (update)?(objects.get_added()):
				(objects.get_components());

	// Zero the signal
	if(!update)
	{
		mu.assign(mu.size(), background);
		staleness = 0;
	}
	else
		staleness++;

	double T, A, phi, v0, viewing_angle;
	vector<double> arg, evaluations;
	for(size_t j=0; j<components.size(); j++)
	{
		T = exp(components[j][0]);
		A = components[j][1];
		phi = components[j][2];
		v0 = sqrt(1. - components[j][3]);
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
	else if(randomU() <= 0.5)
	{
		extra_sigma = log(extra_sigma);
		extra_sigma = (atan(extra_sigma)/M_PI + 0.485)/0.97;
		extra_sigma += randh();
		wrap(extra_sigma, 0., 1.);
		extra_sigma = tan(M_PI*(0.97*extra_sigma - 0.485));
		extra_sigma = exp(extra_sigma);

		nu = log(nu);
		nu += log(1000.)*randh();
		wrap(nu, log(0.1), log(1000.));
		nu = exp(nu);
	}
	else
	{
		for(size_t i=0; i<mu.size(); i++)
			mu[i] -= background;

		background += (Data::get_instance().get_y_max() - Data::get_instance().get_y_min())*randh();
		wrap(background, Data::get_instance().get_y_min(), Data::get_instance().get_y_max());

		for(size_t i=0; i<mu.size(); i++)
			mu[i] += background;
	}

	return logH;
}

double MyModel::logLikelihood() const
{
	// Get the data
	const vector<double>& y = Data::get_instance().get_y();
	const vector<double>& sig = Data::get_instance().get_sig();

	double logL = 0.;
	double var;
	for(size_t i=0; i<y.size(); i++)
	{
		var = sig[i]*sig[i] + extra_sigma*extra_sigma;
		logL += gsl_sf_lngamma(0.5*(nu + 1.)) - gsl_sf_lngamma(0.5*nu)
			- 0.5*log(M_PI*nu) - 0.5*log(var)
			- 0.5*(nu + 1.)*log(1. + pow(y[i] - mu[i], 2)/var/nu);
	}

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	// Make a mock signal on a finer grid than the data
	// Much of this code is copied from calculate_mu()
	// Get times from the data
	const vector<double>& tt = Data::get_instance().get_t();
	double t_min = *min_element(tt.begin(), tt.end());
	double t_max = *max_element(tt.begin(), tt.end());

	vector<double> t(1000);
	double dt = (t_max - t_min)/((int)t.size() - 1);
	for(size_t i=0; i<t.size(); i++)
		t[i] = t_min + i*dt;

	// Get the components
	const vector< vector<double> >& components = objects.get_components();

	// Zero the signal
	vector<double> signal(t.size());
	signal.assign(signal.size(), background);

	double T, A, phi, v0, viewing_angle;
	vector<double> arg, evaluations;
	for(size_t j=0; j<components.size(); j++)
	{
		T = exp(components[j][0]);
		A = components[j][1];
		phi = components[j][2];
		v0 = sqrt(1. - components[j][3]);

		viewing_angle = components[j][4];
		arg = t;
		for(size_t i=0; i<arg.size(); i++)
			arg[i] = 2.*M_PI*t[i]/T + phi;
		evaluations = Lookup::get_instance().evaluate(arg, v0, viewing_angle);
		for(size_t i=0; i<t.size(); i++)
			signal[i] += A*evaluations[i];
	}

	for(size_t i=0; i<signal.size(); i++)
		out<<signal[i]<<' ';
	out<<extra_sigma<<' '<<nu<<' ';
	objects.print(out); out<<' '<<staleness<<' ';
	out<<background<<' ';
}

string MyModel::description() const
{
	return string("");
}


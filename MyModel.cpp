#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>
#include "Lookup.h"
#include <gsl/gsl_sf_gamma.h>

using namespace std;
using namespace Eigen;
using namespace DNest3;

MyModel::MyModel()
:objects(5, 10, false, MyDistribution())
,mu(Data::get_instance().get_t().size())
,C(Data::get_instance().get_t().size(),
			Data::get_instance().get_t().size())
{

}

void MyModel::fromPrior()
{
	objects.fromPrior();
	objects.consolidate_diff();
	background = Data::get_instance().get_y_min() +
		(Data::get_instance().get_y_max() - Data::get_instance().get_y_min())*randomU();

	extra_sigma = exp(tan(M_PI*(0.97*randomU() - 0.485)));

	// Log-uniform prior from 10^(-2) to 10^2 m/s
	eta1 = exp(log(1E-2) + log(1E4)*randomU());

	// Log-uniform prior from 10^(-3) to 10 years
	eta2 = exp(log(1E-3) + log(1E4)*randomU());

	calculate_mu();
}

void MyModel::calculate_C()
{
	// Get the data
	const vector<double>& t = Data::get_instance().get_t();
	const vector<double>& sig = Data::get_instance().get_sig();

	for(size_t i=0; i<Data::get_instance().get_t().size(); i++)
	{
		for(size_t j=i; j<Data::get_instance().get_t().size(); j++)
		{
			C(i, j) = eta1*eta1*exp(-0.5*pow((t[i] - t[j])/eta2, 2));

			if(i==j)
				C(i, j) += sig[i]*sig[i] + extra_sigma*extra_sigma;
			else
				C(j, i) = C(i, j);
		}
	}
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

	if(randomU() <= 0.5)
	{
		logH += objects.perturb();
		objects.consolidate_diff();
		calculate_mu();
	}
	else if(randomU() <= 0.5)
	{
		if(randomU() <= 0.5)
		{
			eta1 = log(eta1);
			eta1 += log(1E4)*randh();
			wrap(eta1, log(1E-2), log(1E2));
			eta1 = exp(eta1);
		}
		else
		{
			eta2 = log(eta2);
			eta2 += log(1E4)*randh();
			wrap(eta2, log(1E-3), log(1E1));
			eta2 = exp(eta2);
		}
	}
	else if(randomU() <= 0.5)
	{
		extra_sigma = log(extra_sigma);
		extra_sigma = (atan(extra_sigma)/M_PI + 0.485)/0.97;
		extra_sigma += randh();
		wrap(extra_sigma, 0., 1.);
		extra_sigma = tan(M_PI*(0.97*extra_sigma - 0.485));
		extra_sigma = exp(extra_sigma);
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

	VectorXd residual(y.size());
	for(size_t i=0; i<y.size(); i++)
		residual(i) = y[i] - mu[i];

	Eigen::LLT<Eigen::MatrixXd> cholesky = C.llt();
	MatrixXd L = cholesky.matrixL();

	double logDeterminant = 0.;
	for(size_t i=0; i<y.size(); i++)
		logDeterminant += 2.*log(L(i,i));

	// C^-1*(y-mu)
	VectorXd solution = cholesky.solve(residual);

	// y*solution
	double exponent = 0.;
	for(size_t i=0; i<y.size(); i++)
		exponent += residual(i)*solution(i);

	double logL = -0.5*y.size()*log(2*M_PI)
					- 0.5*logDeterminant - 0.5*exponent;

	if(isnan(logL) || isinf(logL))
		logL = -1E300;

	return logL;


//	for(size_t i=0; i<y.size(); i++)
//	{
//		var = sig[i]*sig[i] + extra_sigma*extra_sigma;
//		logL += gsl_sf_lngamma(0.5*(nu + 1.)) - gsl_sf_lngamma(0.5*nu)
//			- 0.5*log(M_PI*nu) - 0.5*log(var)
//			- 0.5*(nu + 1.)*log(1. + pow(y[i] - mu[i], 2)/var/nu);
//	}

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
	out<<extra_sigma<<' ';
	objects.print(out); out<<' '<<staleness<<' ';
	out<<background<<' ';
}

string MyModel::description() const
{
	return string("");
}


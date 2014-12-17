#include "Orbit.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

Orbit::Orbit()
{

}

void Orbit::load(const char* filename)
{
	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"# ERROR: Couldn't open file "<<filename<<"."<<endl;
		return;
	}

	// Empty vectors
	vx.clear();
	vy.clear();

	double temp1, temp2;
	while(fin>>temp1 && fin>>temp2)
	{
		vx.push_back(temp1);
		vy.push_back(temp2);
	}

	cout<<"# Loaded "<<vx.size()<<" points from orbit file ";
	cout<<filename<<"."<<endl;

	fin.close();
}

vector<double> Orbit::evaluate(const std::vector<double>& arg_to_cos,
						double viewing_angle) const
{
	// Create radial velocities
	double C = cos(viewing_angle);
	double S = sin(viewing_angle);
	vector<double> vr = vx;
	double vrmax = -1E300;
	double vrmin =  1E300;
	int highest = 0;
	for(size_t i=0; i<vr.size(); i++)
	{
		vr[i] = C*vx[i] + S*vy[i];
		if(vr[i] > vrmax)
			vrmax = vr[i];
		if(vr[i] < vrmin)
			vrmin = vr[i];
		if(vr[i] > vr[highest])
			highest = i;
	}

	for(size_t i=0; i<vr.size(); i++)
		vr[i] = 2*(vr[i] - vrmin)/(vrmax - vrmin) - 1.;

	vector<double> result = arg_to_cos;
	int index;
	double cc = 2.*M_PI*((double)(highest))/vr.size();
	for(size_t i=0; i<result.size(); i++)
	{
		index = (int)(vr.size()*mod(arg_to_cos[i] + cc, 2.*M_PI)/(2*M_PI));
		result[i] = vr[index];
	}

	return result;
}


void Orbit::test()
{
	Orbit o;
	o.load("Orbits/orbits0.710.dat");

	vector<double> t;
	for(double tt=-10; tt <= 10; tt += 0.01)
		t.push_back(tt);

	vector<double> y = o.evaluate(t, 1.);
	for(size_t i=0; i<y.size(); i++)
		cout<<t[i]<<' '<<y[i]<<endl;
}


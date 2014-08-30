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

vector<double> Orbit::evaluate(const std::vector<double>& arg_to_sin,
						double v0,
						double viewing_angle)
{
	// Create radial velocities
	double C = cos(viewing_angle);
	double S = sin(viewing_angle);
	vector<double> vr = vx;
	double vrmax = -1E300;
	for(size_t i=0; i<vr.size(); i++)
	{
		vr[i] = C*vx[i] + S*vy[i];
		if(vr[i] > vrmax)
			vrmax = vr[i];
	}

	bool flip = vr[1] < vr[0];
	for(size_t i=0; i<vr.size(); i++)
	{
		vr[i] /= vrmax;
		if(flip)
			vr[i] *= -1.;
	}

	vector<double> result = arg_to_sin;
	int index;
	for(size_t i=0; i<result.size(); i++)
	{
		index = (int)(vr.size()*mod(arg_to_sin[i], 2.*M_PI)/(2*M_PI));
		result[i] = vr[index];
	}

	return result;
}


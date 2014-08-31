#include "Lookup.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

Lookup Lookup::instance;

Lookup::Lookup()
{

}


void Lookup::load()
{
	orbits.clear();

	double v0;
	Orbit o;
	string filename;

	for(int i=0; i<121; i++)
	{
		v0 = 0.4 + 0.005*i;
		stringstream ss(stringstream::out);
		ss.precision(3);
		ss<<fixed<<v0;
		filename = string("Orbits/orbits" + ss.str() + ".dat");

		o.load(filename.c_str());
		orbits.push_back(o);
	}
}

vector<double> Lookup::evaluate
		(const vector<double>& arg_to_sin,
			double v0,
			double viewing_angle) const
{
	vector<double> result;

	// If v0 is out of bounds, do nothing
	if(v0 < 0.4 || v0 > 1.)
		return result;

	int which = (v0 - 0.4)/0.005;
	return orbits[which].evaluate(arg_to_sin, viewing_angle);
}


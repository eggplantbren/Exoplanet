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


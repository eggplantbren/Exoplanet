#include "Lookup.h"
#include <string>

using namespace std;

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
//		filename = string("Orbits/orbit" + string())
	}
}


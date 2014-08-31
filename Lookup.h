#ifndef _Lookup_
#define _Lookup_

#include <vector>
#include "Orbit.h"

// singleton class

class Lookup
{
	private:
		std::vector<Orbit> orbits;


	public:
		Lookup();
		void load();

};

#endif


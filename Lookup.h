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

	// Instance
	private:
		static Lookup instance;
	public:
		static Lookup& get_instance()
		{ return instance; }

};

#endif


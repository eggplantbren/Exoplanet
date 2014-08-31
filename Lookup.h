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
		std::vector<double> evaluate
				(const std::vector<double>& arg_to_sin,
					double v0,
					double viewing_angle) const;

	// Instance
	private:
		static Lookup instance;
	public:
		static Lookup& get_instance()
		{ return instance; }

};

#endif


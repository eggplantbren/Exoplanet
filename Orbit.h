#ifndef _Orbit_
#define _Orbit_

#include <vector>

class Orbit
{
	private:
		std::vector<double> vx, vy;

	public:
		Orbit();
		void load(const char* filename);

		std::vector<double> evaluate
				(const std::vector<double>& arg_to_sin,
						double viewing_angle) const;
};

#endif

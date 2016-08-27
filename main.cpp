#include <iostream>
#include "Lookup.h"
#include "MyModel.h"
#include "Data.h"

using namespace std;

int main(int argc, char** argv)
{
	// Load the data and lookup tables
	Data::get_instance().load("fake_data_like_nuoph.txt");
	Lookup::get_instance().load();

    DNest4::start<MyModel>(argc, argv);

	return 0;
}


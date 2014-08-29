#include "Orbit.h"
#include <fstream>
#include <iostream>

using namespace std;

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


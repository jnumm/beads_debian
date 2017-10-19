#include "parameterConfluent.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

parameterConfluent::parameterConfluent() {
	//default parameters:
	_minflux = 100;
	_minpath = 2;
	_winconfl = 300;
	_minbeads = 200;
	_intmax = 50000;
	_minpct = 20;
}



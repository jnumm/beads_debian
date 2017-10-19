#include "parameterNumber.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

parameterNumber::parameterNumber() {
	//default parameters for Ag gel images:
	_enlarge = 3;
	_enlarge_fusion = 3;
	_npass = 3;
}

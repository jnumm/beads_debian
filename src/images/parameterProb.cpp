#include "parameterProb.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

parameterProb::parameterProb() {
	//default parameters for Ag gel images:
	_threshold = 10;
	_sx = 3;
	_sy = 3;
	_sx_bottom = 0;
	_sy_bottom = 0;
}

/** \brief give a sx depending on the gel ratio position
 * 
 * the gel is divided in "nslices" zones that have different sx
 * 
 * \param float ratio_position betwee 0 and 1 => 0.5 is half of the gel
 */
const float parameterProb::get_sx_with_gel_ratio_position(float ratio_position) const {
	//cerr << "parameterProb::get_sx_with_gel_ratio_position " << ratio_position << " " << _sx_bottom << endl;
	float result = _sx;
	if (_sx_bottom == 0)
		return (result);

	float difference(_sx_bottom - _sx);
	unsigned int nslices(10);
	float step_size(difference / ((float) (nslices - 1)));
	unsigned int step((unsigned int) (ratio_position * ((float) nslices)));
	result = result + (step * step_size);
	return (result);
}

const float parameterProb::get_sy_with_gel_ratio_position(float ratio_position) const {
	float result = _sy;
	if (_sy_bottom == 0)
		return (result);

	float difference(_sy_bottom - _sy);
	unsigned int nslices(10);
	float step_size(difference / (nslices - 1));
	unsigned int step((unsigned int) (ratio_position * ((float) nslices)));
	result = result + (step * step_size);
	return (result);
}


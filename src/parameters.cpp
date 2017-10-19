#include "parameters.h"

#include <string>
using namespace std;

parameters::parameters(const string config_filename) :
	ConfigFile(config_filename.c_str()) {
}

const parameterDetection & parameters::get_parameter_detection() {
	_parameter_detection.set_minproba(read("minproba",
			_parameter_detection.get_minproba()));
	return (_parameter_detection);
}

const parameterDirect & parameters::get_parameter_direction() {
	_parameter_direction.set_burned_pixel_threshold(read(
			"burned_pixel_threshold",
			_parameter_direction.get_burned_pixel_threshold()));
	return (_parameter_direction);
}

const parameterConfluent & parameters::get_parameter_confluent() {
	_parameter_confluent.set_minflux(read("minflux",
			_parameter_confluent.get_minflux()));
	_parameter_confluent.set_minpath(read("minpath",
			_parameter_confluent.get_minpath()));
	_parameter_confluent.set_winconfl(read("winconfl",
			_parameter_confluent.get_winconfl()));
	_parameter_confluent.set_minbeads(read("minbeads",
			_parameter_confluent.get_minbeads()));
	_parameter_confluent.set_intmax(read("confluent_intmax",
			_parameter_confluent.get_intmax()));
	_parameter_confluent.set_minpct(read("confluent_minpct",
			_parameter_confluent.get_minpct()));
	return (_parameter_confluent);
}

const parameterNumber & parameters::get_parameter_number() {

	_parameter_number.set_enlarge(read("quantification_enlarge",
			_parameter_number.get_enlarge()));
	_parameter_number.set_enlarge_fusion(read("quantification_enlarge_fusion",
			_parameter_number.get_enlarge_fusion()));
	_parameter_number.set_npass(read("npass",
			_parameter_number.get_npass()));
	return (_parameter_number);
}

const parameterProb & parameters::get_parameter_prob() {

	_parameter_prob.set_threshold(read("prob_threshold",
			_parameter_prob.get_threshold()));
	_parameter_prob.set_sx(read("sx", _parameter_prob.get_sx()));
	_parameter_prob.set_sy(read("sy", _parameter_prob.get_sy()));
	_parameter_prob.set_sx_bottom(read("sx_bottom",
			_parameter_prob.get_sx_bottom()));
	_parameter_prob.set_sy_bottom(read("sy_bottom",
			_parameter_prob.get_sy_bottom()));
	return (_parameter_prob);
}


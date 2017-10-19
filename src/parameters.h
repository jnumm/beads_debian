#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ConfigFile/ConfigFile.h"
#include "parameterDetection.h"
#include "images/parameterDirect.h"
#include "images/parameterConfluent.h"
#include "images/parameterNumber.h"
#include "images/parameterProb.h"

class parameters : public ConfigFile {
public:
	parameters(const string config_filename);
	~parameters(){};

	const parameterDetection & get_parameter_detection();
	
	const parameterDirect & get_parameter_direction();

	const parameterConfluent & get_parameter_confluent();

	const parameterNumber & get_parameter_number();

	const parameterProb & get_parameter_prob();

private:

	parameterDetection _parameter_detection;
	parameterDirect _parameter_direction;
	parameterConfluent _parameter_confluent;
	parameterNumber _parameter_number;
	parameterProb _parameter_prob;
	
};

#endif /*PARAMETERS_H_*/

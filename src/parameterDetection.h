#ifndef PARAMETERDETECTION_H_
#define PARAMETERDETECTION_H_

/** \brief handle parameters for detection
 *
 */

class parameterDetection {
public:
	parameterDetection();
	~parameterDetection() {
	}

	void set_minproba(float threshold) {
		_minproba = threshold;
	}

	const float get_minproba() const {
		return (_minproba);
	}

private:
	float _minproba;

};

#endif /*PARAMETERDETECTION_H_*/

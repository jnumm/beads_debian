#ifndef PARAMETERDIRECT_H_
#define PARAMETERDIRECT_H_

/** \brief handle parameters for parameterDirect
 *
 */

class parameterDirect {
public:
	parameterDirect() {
		//default parameters for Ag gel images:
		_burned_pixel_threshold = 63000;
	};
	~parameterDirect() {
	};

	//bool open ( const char * filename );
	void set_burned_pixel_threshold(unsigned short int threshold) {
		_burned_pixel_threshold = threshold;
	};

	const unsigned short int get_burned_pixel_threshold() const {
		return(_burned_pixel_threshold);
	};

private:
	unsigned short int _burned_pixel_threshold;
};

#endif /*PARAMETERDIRECT_H_*/

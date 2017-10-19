#ifndef IMAGECOULE_H_
#define IMAGECOULE_H_

#include "imageIntensity.h"
#include "imageDirect.h"

//Faire couler les beads
class imageCoule: public imageIntensity {
public:
	imageCoule(const char * & filename) :
		imageIntensity(filename) {
	}
	;
	imageCoule() {
	}
	;
	~imageCoule() {
	}
	;

	void compute_from_direct(const imageDirect & direct);
	//void copy_from_image (const cimg_library::CImg<unsigned short int> & image1);

private:
	static const unsigned short int _max_value = 65535;
};

#endif /*IMAGECOULE_H_*/

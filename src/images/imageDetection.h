#ifndef IMAGEDETECTION_H_
#define IMAGEDETECTION_H_

#include "imageIntensity.h"

class detection;

//images des spots détectés
class imageDetection : public cimg_library::CImg<unsigned short int> {
public:

	imageDetection(detection &);

	imageDetection(const char * & filename) : cimg_library::CImg<unsigned short int>(filename){
	};

	~imageDetection() {
	};

private:
};

#endif /*IMAGEDETECTION_H_*/

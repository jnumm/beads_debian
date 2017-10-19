#ifndef IMAGECONTOURS_H_
#define IMAGECONTOURS_H_

#include "imageIntensity.h"
#include "imageNumber.h"
#include "../detection.h"
//dessiner les contours des spots
class imageContours: public imageIntensity {
public:
	imageContours(const char * & filename) :
		imageIntensity(filename) {
	}

	imageContours() {
	}

	~imageContours() {
	}

	void compute_from_gel_image_and_numeros(
			const imageIntensity & image_depart,
			const imageNumber & image_numeros);
	void draw_contours(const detection & my_detection, bool inverse);

private:
};

#endif /*IMAGECONTOURS_H_*/

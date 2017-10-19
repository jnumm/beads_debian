#ifndef IMAGENUMBER_H_
#define IMAGENUMBER_H_

#include "../detection.h"
#include "imageCode.h"
#include "imageDirect.h"
#include "parameterNumber.h"

//stocker les numéros de spots
class imageNumber: public imageCode {
public:
	//imageNumber(const char * & filename): cimg_library::CImg<unsigned short int>(filename){};
	imageNumber() {
	}

	~imageNumber() {
	}

	void compute_from_direct(const imageDirect & direct,
			detection & my_detection, const parameterNumber & parameter_number);
	void expand_areas_down_the_slope(const imageIntensity & gel_image,
			int npass);
	void save(const QString & filename);

private:

	void
	fusion(int spot_number_1, int spot_number_2, detection & the_detection);
	void set_numbers_with_detection(detection & the_detection);
	void enlarge_and_fusion(detection & my_detection, int enlarge,
			const imageDirect & direct);
	void enlarge(int enlarge);
	void bouche_trou();

	void enlarge_burned_areas(const imageDirect & direct,
			detection & the_detection);
	void set_burned_zone_number(const imageDirect & direct, int number, int x,
			int y, unsigned int & depth);

	int neighbor_number(int x, int y) const;
};

#endif /*IMAGENUMBER_H_*/

#ifndef IMAGEPATHS_H_
#define IMAGEPATHS_H_

#include "imageDirect.h"
#include "imageCoule.h"

//Calcul des chemins pris par les beads
class imagePaths: public cimg_library::CImg<unsigned short int> {
public:
	imagePaths(const char * & filename) :
		cimg_library::CImg<unsigned short int>(filename) {
	}

	imagePaths() {
	}

	~imagePaths() {
	}

	void compute_from_direct_and_coule(const imageDirect & direct,
			const imageCoule & coule);

	void save(const QString & filename);

private:
	static const unsigned short int _max_value = 65535;
};

#endif /*IMAGEPATHS_H_*/

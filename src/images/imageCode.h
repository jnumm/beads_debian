#ifndef IMAGECODE_H_
#define IMAGECODE_H_

#include "../config.h"

#include "../CImg.h"
//#include <CImg.h>

//Images pour lesquelles les valeurs des pixels sont des codes et non des valeurs quantitatives
//Sont des codes: l'image des directions, l'image des numeros

/** \brief class of image that contains codes and not quantitative values
 * for example : the direction image or the number image
 */
class imageCode: public cimg_library::CImg<short int> {
public:
	imageCode(const char * & filename) :
		cimg_library::CImg<short int>(filename) {
	}

	imageCode() {
	}

	~imageCode() {
	}

private:

public:
	static const unsigned short int _max_value = 32767;

};

#endif /*IMAGECODE_H_*/

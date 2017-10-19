#ifndef DETECTION_H_
#define DETECTION_H_

/*
 * Olivier:
 * l'idée de l'objet détection est de représenter une collection de spots détectés sur une image
 *
 * les fonctions membres permettront de détecter les spots (créer et numéroter les spots trouvés sur une image)
 *
 * */
#include <map>
#include <string>
#include "spot.h"
#include "images/imageProb.h"
#include "images/imageDetection.h"
#include "parameterDetection.h"

class imageNumber;

/** \brief detection is a structure to handle a collection of spot objects
 * 
 * detection is a C++ STL map derived object
 * it is a collection of spots for an image, indexed by their spot number
 * 
 * the spot object is described in file spot.h
 * 
 */

typedef std::map<unsigned int, spot> spot_map;

class detection: public spot_map {
public:
	detection();
	detection(const detection & src);
	detection(const imageDetection &);
	~detection();

	void detect_from_prob(const imageProb & image_prob,
			const parameterDetection & parameter_detection);
	//void write_detection_image(cimg_library::CImg<unsigned short int> & detection_image);
	void quantify_spots_with_image(
			const cimg_library::CImg<int> & image_depart,
			const imageNumber & image_numeros);

	void store_spot_edges(const imageNumber & image_numeros);

	const int get_gel_image_width() const {
		return (_gel_image_width);
	}

	const int get_gel_image_height() const {
		return (_gel_image_height);
	}

	spot & get_spot(unsigned int spot_number);

private:

	//the original image file name on which spots were detected
	std::string _image_file_name;

	//the size of the original gel image
	int _gel_image_width;
	int _gel_image_height;
};

#endif /*DETECTION_H_*/

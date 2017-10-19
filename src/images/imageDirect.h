#ifndef IMAGEDIRECT_H_
#define IMAGEDIRECT_H_

#include "imageIntensity.h"
#include "parameterDirect.h"

class imagePaths;

/** \brief stores the directions of the greater slopes for each pixel
 * 
 * the resulting image describes the directions to the spot center with a code :
 * 0 - 7 are possible directions
 * if there is no slope, the value will be 1000
 * edges of the images directions are set to 100
 * burned pixels are coded with the _max_value (saturated zone)
 * 
 */
class imageDirect: public cimg_library::CImg<unsigned short int> {
public:
	imageDirect(const QString & filename) :
		cimg_library::CImg<unsigned short int>(filename.toStdString().c_str()) {
	}

	imageDirect() {
	}

	~imageDirect() {
	}

	void compute_from_gel_image(const imageIntensity & gel_image,
			const parameterDirect & parameter_direct);

	/** \brief set the pixel at position x, y as burned
	 */
	void set_burned_pixel(int x, int y) {
		//float *ptr = data(x, y);
		*(data(x, y)) = _max_value;
	}

	/** \brief tell if the pixel is burned (saturated zone)
	 */
	bool is_burned_pixel(int x, int y) const {
		if (*(data(x, y)) == _max_value) {
			return (true);
		}
		return (false);
	}

	bool move_to_spot_center(int & xx, int & yy) const;

	int get_nb_confluent_for_pixel(int x, int y, const imagePaths & paths,
			int minflux) const;
	void save(const QString & filename);

private:

public:
	static const unsigned short int _max_value = 65535;

};

#endif /*IMAGEDIRECT_H_*/

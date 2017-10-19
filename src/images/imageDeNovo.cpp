#include "imageDeNovo.h"

#include <iostream>
#include <fstream>

using namespace std;

/** \brief create a theoretical spot on a gel image
 *
 * \param posx x position of the spot center
 * \param posy y position of the spot center
 * \param max_intensity_percentage the theoretical maximum peak intensity given in percentage (0-100)
 * \param sx gaussian sx
 * \param sy gaussian sy
 *
 */

void imageDeNovo::addSpot(unsigned int posx, unsigned int posy,
		float max_intensity_percentage, float sx, float sy) {

	imageDeNovo & image = *this;

	unsigned short int max_intensity = (unsigned short int) ((float) _max_value
			* ((float) max_intensity_percentage / (float) 100));
	//	cout << _max_value << "    "<< max_intensity_percentage <<"    "<< max_intensity << endl;

	unsigned int maxx = (unsigned int) sx * 3;
	unsigned int maxy = (unsigned int) sy * 3;
	unsigned int minx = 0;
	unsigned int miny = 0;
	if (posx > maxx) {
		minx = posx - maxx;
	}
	maxx = posx + maxx;
	if (maxx > (unsigned int) image.width()) {
		maxx = image.width();
	}

	if (posy > maxy) {
		miny = posy - maxy;
	}
	maxy = posy + maxy;
	if (maxy > (unsigned int) image.height()) {
		maxy = image.height();
	}

	for (unsigned int x = minx; x < maxx; x++) {
		for (unsigned int y = miny; y < maxy; y++) {
			double fnx = (double) x - (double) posx;
			double fny = (double) y - (double) posy;
			double valx = (fnx * fnx) / (2 * (sx * sx));
			double valy = (fny * fny) / (2 * (sy * sy));

			double tmp = (((double) max_intensity) * (double) std::exp(-valx
					- valy)) + image(x, y);
			if (tmp > (double) _max_value) {
				image(x, y) = _max_value;
				//cout << image(x,y) << endl;
			} else {
				image(x, y) = (unsigned short int) tmp;
			}
		}
	}

	//generate the spot profile:
	/*
	 cimg_for_insideXY(image,x,y,0) {
	 double fnx = (double)x - (double) posx;
	 double fny = (double)y - (double) posy;
	 double valx=(fnx*fnx)/(2*(sx*sx));
	 double valy=(fny*fny)/(2*(sy*sy));

	 double tmp = (((double) max_intensity) * (double)std::exp(-valx-valy)) + image(x,y);
	 if (tmp > (double) _max_value) {
	 image(x,y) = _max_value;
	 //cout << image(x,y) << endl;
	 }
	 else {
	 image(x,y) = (unsigned short int)tmp;
	 }
	 //cout << fnx << " " << fny << " "<< std::exp(-valx-valy) << " " << x << " " << y << " " << image(x,y) << endl;
	 }
	 */
}

bool imageDeNovo::build_image_with_spot_file(const QString & filename) {
	ifstream ifs;

	ifs.open(filename.toStdString().c_str(), ifstream::in);
	if (ifs.is_open() == 0) {
		ifs.clear();
		cerr << "ERROR : denovo spot file " << filename.toStdString() << " not found" << endl;
		return (false);
	}
	std::string temp;

	ifs >> temp;
	ifs >> temp;

	//define image height and width :
	unsigned int height;
	unsigned int width;

	ifs >> height;
	ifs >> width;
	if (!ifs.good()) {
		cerr << "height and width not found" << endl;
		return (false);
	}
	this->resize(height, width, 1, 1);
	this->fill(0);

	unsigned int posx;
	unsigned int posy;
	float max_intensity_percentage;
	float sx;
	float sy;

	ifs >> temp;
	ifs >> temp;
	ifs >> temp;
	ifs >> temp;
	ifs >> temp;

	if (!ifs.good()) {
		cerr << "denovo spot file not well formated" << endl;
	}

	while (ifs.good()) {
		ifs >> posx;
		if (!ifs.good()) {
			break;
		}
		ifs >> posy;
		ifs >> max_intensity_percentage;
		ifs >> sx;
		ifs >> sy;
		if (ifs.good()) {
			this->addSpot(posx, posy, max_intensity_percentage, sx, sy);
		} else {
			cerr << "missing parameters to add spot " << posx << posy << endl;
			return (false);
		}
	}

	ifs.close();
	return (true);

}
;

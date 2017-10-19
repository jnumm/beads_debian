#include "imageIntensity.h"

#include <iostream>

using namespace std;
using namespace cimg_library;

imageIntensity::imageIntensity(int dimx, int dimy, int slices, int channel, bool is_inversed, const QString & image_filename) :
	cimg_library::CImg<unsigned short int>(dimx, dimy, slices, channel) {
	_is_inversed = is_inversed;
	_image_filename = image_filename;
}

imageIntensity::imageIntensity(const QString & filename) :
	cimg_library::CImg<unsigned short int>(filename.toStdString().c_str()) {
	_is_inversed = false;
	_image_filename = filename;

	if (this->max_intensity() < 256) {
		this->transform_16_bits();
	}
	//cerr << "imageIntensity::imageIntensity bits " << img.max();
}

void imageIntensity::save(const QString & filename) const {
	qDebug() << "imageIntensity::save(const QString & filename) begin";
	//1QFileInfo fileinfo(filename);
	cimg_library::CImg<unsigned short int>::save(filename.toStdString().c_str());
	qDebug() << "imageIntensity::save(const QString & filename) end";
}

unsigned short int imageIntensity::max_intensity() const {

	unsigned short int maximum(0);
	const imageIntensity & img = *this;

	cimg_forXY(img,x,y)
		{
			if (img(x, y) > maximum) {
				maximum = img(x, y);
			}
		}
	return (maximum);
}

void imageIntensity::transform_16_bits() {
	imageIntensity & img = *this;
	cimg_forXY(img,x,y)
		{
			img(x, y) *= 254;
		}
}

imageIntensity * imageIntensity::new_image_inversed() const {
	const imageIntensity & image = *this;
	imageIntensity * p_image_inversed = new imageIntensity(image.width(),
			image.height(), 1, 1, !_is_inversed, _image_filename);
	imageIntensity & image_inversed = *p_image_inversed;

	cimg_forXY(image,x,y)
		{
			image_inversed(x, y) = 65535 - image(x, y);
		}

	return (p_image_inversed);
}

void imageIntensity::save_inversed(const QString & filename) const {
	imageIntensity * image_inversed = this->new_image_inversed();
	image_inversed->save(filename);
}

/** \brief look for a neighbor pixel with greater intensity
 *
 */
int imageIntensity::amas(int &xx, int &yy, imageCode & mark) const {
	const imageIntensity & current_image = *this;
	// si result reste à 1 c'est qu'on n'a pas trouvé de voisin d'intensité supérieure
	//si result passe à 0 c'est qu'on en a trouvé 1. Mais s'il est déjà marqué, result est mis à -1.
	//comme on recherche toujours un max, on ne peut pas être éjà passé pour le
	//spot courant. Donc si on tombe sur du déjà marqué c'est que déjà marqué
	//à partir d'un autre pixel, et donc il faut abandonner. (result=-1)
	int result = 1;
	if ((current_image(xx + 1, yy - 1) - current_image(xx, yy) > 0)) {
		xx++;
		yy--;
		result = 0;
	} else if ((current_image(xx + 1, yy) - current_image(xx, yy) > 0)) {
		xx++;
		result = 0;
	} else if ((current_image(xx + 1, yy + 1) - current_image(xx, yy) > 0)) {
		xx++;
		yy++;
		result = 0;
	} else if ((current_image(xx, yy + 1) - current_image(xx, yy) > 0)) {
		yy++;
		result = 0;
	} else if ((current_image(xx - 1, yy + 1) - current_image(xx, yy) > 0)) {
		xx--;
		yy++;
		result = 0;
	} else if ((current_image(xx - 1, yy) - current_image(xx, yy) > 0)) {
		xx--;
		result = 0;
	} else if ((current_image(xx - 1, yy - 1) - current_image(xx, yy) > 0)) {
		xx--;
		yy--;
		result = 0;
	} else if ((current_image(xx, yy - 1) - current_image(xx, yy) > 0)) {
		yy--;
		result = 0;
	}

	if (result == 0 && mark(xx, yy) == 1)
		result = -1;
	mark(xx, yy) = 1;
	return result;

}

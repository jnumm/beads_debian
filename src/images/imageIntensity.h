#ifndef IMAGEINTENSITY_H_
#define IMAGEINTENSITY_H_

#include <QDebug>
#include <QString>

#include "imageCode.h"

//Images pour lesquelles les valeurs des pixels représentent des valeurs quantitatives
//L'image de départ et presque toutes les intermédiaires sont des imagesIntensity
class imageIntensity: public cimg_library::CImg<unsigned short int> {
public:
	imageIntensity(int dimx, int dimy, int slices, int channel, bool is_inversed, const QString & _image_filename);
	imageIntensity(const QString & filename);
	imageIntensity() {
		_is_inversed = false;
	}
	imageIntensity(const imageIntensity & original) :
		cimg_library::CImg<unsigned short int>(original) {
		_is_inversed = original.get_is_inversed();
		_image_filename = original.get_image_filename();
	}

	~imageIntensity() {
		_is_inversed = false;
	}

	//void image_inverse ();
	imageIntensity * new_image_inversed() const;

	int amas(int &xx, int &yy, imageCode & mark) const;

	const QString & get_image_filename() const {
		return _image_filename;
	}

	const bool get_is_inversed() const {
		return (_is_inversed);
	}

	void save(const QString & filename) const;

	void save_inversed(const QString & filename) const;

	unsigned short int max_intensity() const;

private:
	bool _is_inversed;

	QString _image_filename;

protected:
	void transform_16_bits();

public:
	static const unsigned short int _max_value = 65535;

};

#endif /*IMAGEINTENSITY_H_*/

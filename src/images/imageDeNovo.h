#ifndef IMAGEDENOVO_H_
#define IMAGEDENOVO_H_

#include "imageIntensity.h"

using namespace std;

class imageDeNovo : public imageIntensity {
public:
	imageDeNovo() {
	};
	~imageDeNovo() {
	};

	void addSpot(unsigned int posx, unsigned int posy, float max_intensity_percentage, float sx, float sy);

	bool build_image_with_spot_file ( const QString & filename );

private:
};

#endif /*IMAGEDENOVO_H_*/

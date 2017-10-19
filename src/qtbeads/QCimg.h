/*
 * QCimg.h
 *
 *  Created on: 27 nov. 2009
 *      Author: olivier
 */

#ifndef QCIMG_H_
#define QCIMG_H_

#include <QImage>
#include "../images/imageIntensity.h"

using namespace std;

class QCimg: public QImage {
public:
	QCimg(const imageIntensity & cimg);
	virtual ~QCimg();
};

#endif /* QCIMG_H_ */

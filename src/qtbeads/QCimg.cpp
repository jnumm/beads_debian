/*
 * QCimg.cpp
 *
 *  Created on: 27 nov. 2009
 *      Author: olivier
 */

#include "QCimg.h"
#include <QTemporaryFile>
#include <iostream>

QCimg::QCimg(const imageIntensity & cimg_image) :
	QImage(cimg_image.width(), cimg_image.height(), QImage::Format_RGB32) {

	qDebug()
				<< "QCimg::QCimg(const imageIntensity & cimg_image) begin ";

	//bool QImage::loadFromData ( const QByteArray & data, const char * format = 0 )
	/*	const unsigned short int * p_buffer = cimg.data;
	 unsigned int size = cimg.size();

	 loadFromData ( (uchar *) p_buffer, size);*/

	QImage & image = *this;
	QRgb value;
	float ratio = (float) 255 / (float) imageIntensity::_max_value;

	cimg_forXY(cimg_image,x,y)
		{
			unsigned int ivalue = (cimg_image(x, y) * ratio);
			value = qRgb(ivalue, ivalue, ivalue);
			image.setPixel(x, y, value);
		}

	/*
	 QTemporaryFile tempfile;
	 tempfile.open ();
	 std::cout << tempfile.fileName ().toStdString() << endl;
	 //cimg.save(tempfile.fileName ().toStdString().c_str());
	 cimg.save_pnm(tempfile.fileName ().toStdString().c_str());
	 load (tempfile.fileName (), "PPM");
	 int i;
	 cin >> i;
	 */

	qDebug()
				<< "QCimg::QCimg(const imageIntensity & cimg_image) end ";

}

QCimg::~QCimg() {
}

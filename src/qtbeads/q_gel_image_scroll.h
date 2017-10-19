/*
 * q_gel_image_scroll.h
 *
 *  Created on: 3 janv. 2010
 *      Author: olivier
 */

#ifndef Q_GEL_IMAGE_SCROLL_H_
#define Q_GEL_IMAGE_SCROLL_H_

#include <QScrollArea>
#include "q_gel_image.h"

class QGelImageScroll: public QScrollArea {
public:
	QGelImageScroll();
	virtual ~QGelImageScroll();

	const QGelImage * getGelImage() const;

	void setGelImage(const QImage & image);
	void setGelImage(const QCimg & image);

	void adjustSize();
	void setScaleFactor(double scale_factor);
	double getScaleFactor() const {
		return (imageLabel->getScaleFactor());
	}
	void scaleImage(double factor);

private:
	QGelImage *imageLabel;

};

#endif /* Q_GEL_IMAGE_SCROLL_H_ */

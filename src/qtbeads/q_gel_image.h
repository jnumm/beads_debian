/*
 * q_gel_image.h
 *
 *  Created on: 11 déc. 2009
 *      Author: olivier
 */

#ifndef Q_GEL_IMAGE_H_
#define Q_GEL_IMAGE_H_

#include <QLabel>
#include "QCimg.h"

class QGelImage: public QLabel {
Q_OBJECT
public:
	QGelImage();
	virtual ~QGelImage();

	void setQCimg(QCimg & image);

	void setScaleFactor(double scale_factor);
	void scaleImage(double factor);
	double getScaleFactor() const {
		return (_scaleFactor);
	}
	int getX(int real_x) const;
	int getY(int real_y) const;
	int getBeforeX() const;
	int getBeforeY() const;

	signals:
	void mouseMoved(QMouseEvent *event);
	void moveGelImage(QMouseEvent *event);
	void doubleClicked(QMouseEvent *event);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

private:
	double _scaleFactor;
	bool _mousePressed;

	int _before_x;
	int _before_y;

};

#endif /* Q_GEL_IMAGE_H_ */

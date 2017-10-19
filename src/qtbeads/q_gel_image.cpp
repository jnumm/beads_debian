/*
 * q_gel_image.cpp
 *
 *  Created on: 11 déc. 2009
 *      Author: olivier
 */

#include "q_gel_image.h"

QGelImage::QGelImage() {
	setMouseTracking(true);
	_mousePressed = false;
}

QGelImage::~QGelImage() {
}

void QGelImage::mouseMoveEvent(QMouseEvent *event) // Fonction protected appartenant à QWidget
{
	if (_mousePressed) {
		emit moveGelImage(event);
	} else {
		emit mouseMoved(event);
	}
}

int QGelImage::getBeforeX() const {
	return (_before_x);

}
int QGelImage::getBeforeY() const {
	return (_before_y);

}

void QGelImage::mouseDoubleClickEvent(QMouseEvent *event) {
	emit doubleClicked(event);
}

void QGelImage::mousePressEvent(QMouseEvent * ev) {
	_mousePressed = true;
}

void QGelImage::mouseReleaseEvent(QMouseEvent * ev) {
	_mousePressed = false;
}

void QGelImage::setQCimg(QCimg & image) {
	this->setPixmap(QPixmap::fromImage(image));
}

void QGelImage::setScaleFactor(double scale_factor) {
	_scaleFactor = scale_factor;
}

void QGelImage::scaleImage(double factor) {
	_scaleFactor *= factor;
	this->resize(_scaleFactor * this->pixmap()->size());
}

int QGelImage::getX(int real_x) const {
	return ((1 / _scaleFactor) * real_x);
}
int QGelImage::getY(int real_y) const {
	return ((1 / _scaleFactor) * real_y);
}

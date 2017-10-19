/*
 * q_gel_image_scroll.cpp
 *
 *  Created on: 3 janv. 2010
 *      Author: olivier
 */

#include "q_gel_image_scroll.h"

QGelImageScroll::QGelImageScroll() {
	imageLabel = new QGelImage();
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);
	setBackgroundRole(QPalette::Dark);
	setWidget(imageLabel);
}

QGelImageScroll::~QGelImageScroll() {
}

const QGelImage * QGelImageScroll::getGelImage() const {
	return (imageLabel);
}

void QGelImageScroll::setGelImage(const QImage & image) {
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setScaleFactor(1.0);
}

void QGelImageScroll::setGelImage(const QCimg & image) {
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setScaleFactor(1.0);
}

void QGelImageScroll::adjustSize() {
	imageLabel->adjustSize();
}

void QGelImageScroll::setScaleFactor(double scale_factor) {
	imageLabel->setScaleFactor(scale_factor);
}

void QGelImageScroll::scaleImage(double factor) {
	imageLabel->scaleImage(factor);
}

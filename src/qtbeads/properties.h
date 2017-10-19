/*
 * properties.h
 *
 *  Created on: 27 nov. 2009
 *      Author: olivier
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <QString>
#include <QFileInfo>
#include "../images/imageIntensity.h"

using namespace std;

class Properties {
public:
	Properties();
	virtual ~Properties();
	Properties(const Properties & src);

	const imageIntensity * get_p_working_image() const;
	bool gelImageIsLoaded() const {
		if (_p_working_gel_image == NULL) {
			return false;
		}
		return true;
	}


	void setGelImageFilename(const QString & filename);
	const QFileInfo & getGelImageFileInfo() const;
	void setParamFilename(const QString & filename);
	const QFileInfo & getParamFileInfo() const;
	bool isInverse() const {
		return (_inversed);
	}
	void setInverse(bool ok);

private:
	imageIntensity * _p_working_gel_image;
	QFileInfo * _p_param_file_info;
	QFileInfo * _p_original_gel_image_file_info;
	bool _inversed;
};

#endif /* PROPERTIES_H_ */

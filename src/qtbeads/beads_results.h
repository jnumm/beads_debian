/*
 * beads_results.h
 *
 *  Created on: 29 nov. 2009
 *      Author: olivier
 */

#ifndef BEADS_RESULTS_H_
#define BEADS_RESULTS_H_

#include "properties.h"

#include "../images/imageDetection.h"
#include "../images/imageNumber.h"

class BeadsResults {
public:
	BeadsResults(const Properties & initial_properties);
	virtual ~BeadsResults();
	void setDetection(detection * p_detection) {
		_p_detection = p_detection;
	}
	void setNumberImage(const imageNumber * p_number_image) {
		_p_number_image = p_number_image;
	}

	detection * getDetection() const {
		return (_p_detection);
	}

	void saveImageContour(const QFileInfo & filename) const;
	void saveProticDbMl(const QFileInfo & filename) const;
	void saveText(const QFileInfo & filename) const;
	void saveSvg(const QFileInfo & filename) const;
	void saveGnumeric(const QFileInfo & filename) const;

	int getSpotNumber(int x, int y) const;

private:
	const Properties _properties;
	detection * _p_detection;
	const imageNumber * _p_number_image;
};

#endif /* BEADS_RESULTS_H_ */

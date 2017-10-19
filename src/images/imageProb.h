#ifndef IMAGEPROB_H_
#define IMAGEPROB_H_

#include "imageIntensity.h"
#include "parameterProb.h"

/** stores probabilities about spot positions
 * 
 * 
 */

class imageProb: public imageIntensity {
public:
	imageProb(const char * & filename) :
		imageIntensity(filename) {
	}

	imageProb() {
	}

	~imageProb() {
	}

	void compute_from_image_intensity(const imageIntensity & coule,
			const parameterProb & parameter_prob);

	void mark_burned_pixels(imageCode & mark, int & x, int & y) const;

private:
	void rec_mark_burned_pixels(imageCode & mark, int x, int y, int & xmin,
			int & xmax, int & ymin, int & ymax, unsigned int & depth) const;
};

#endif /*IMAGEPROB_H_*/

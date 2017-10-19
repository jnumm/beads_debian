#include "imageDetection.h"

#include "../detection.h"

imageDetection::imageDetection(detection & my_detection) {
	imageDetection & detection_image = *this;
	resize (my_detection.get_gel_image_width(),my_detection.get_gel_image_height(),1,1);
	fill(0);

	detection::iterator it;
	for (it = my_detection.begin (); it != my_detection.end (); ++it) {
		detection_image((*it).second.get_x(), (*it).second.get_y())=65000;
	}
};

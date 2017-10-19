#include "imageCoule.h"

#include <iostream>

using namespace std;

void imageCoule::compute_from_direct(const imageDirect & direct) {
	imageCoule & image_dest = *this;
	image_dest.resize(direct.width(), direct.height(), 1, 1);
	image_dest.fill(0);
	// processing
	cimg_forXY(direct,x,y)
		{
			int xx = x;
			int yy = y;
			int i = 0;
			while ((i < 100) && (image_dest(xx, yy) == 0)
					&& direct.move_to_spot_center(xx, yy)) {
				i++;
			}
			// on ne fait rien en cas de plat (= case 100): xx et yy sont inchangés
			if (direct.is_burned_pixel(xx, yy)) {
				//
				image_dest(xx, yy) = _max_value;
			} else {
				if (image_dest(xx, yy) != _max_value) {
					image_dest(xx, yy) += 1;
				}
			}
		}
	/* in image coule:
	 *  0 means the beads was rolling
	 *  !=0 means N beads stopped here
	 *  65535 means the zone is saturated
	 */
	//save("/tmp/coule.tiff");
	//cout << "imageCoule::compute_from_direct end" << endl;
}
;


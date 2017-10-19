#include "imagePaths.h"

#include <iostream>

using namespace std;

void imagePaths::save(const QString & filename) {
	cimg_library::CImg<unsigned short int>::save(filename.toStdString().c_str());
}

void imagePaths::compute_from_direct_and_coule(const imageDirect & direct,
		const imageCoule & coule) {
	imagePaths & image_dest = *this;
	image_dest.resize(direct.width(), direct.height(), 1, 1);
	image_dest.fill(0);
	// processing
	//cout << "imagePaths::compute_from_direct_and_coule begin" << endl;
	cimg_forXY(coule,x,y)
		{
			if (direct.is_burned_pixel(x, y)) {
				image_dest(x, y) = _max_value;
			} else {
				int xx = x;
				int yy = y;
				while (coule(xx, yy) == 0) {
					//while the bead has not reached the end
					if (direct.move_to_spot_center(xx, yy)) {
						//draw path
						if (image_dest(xx, yy) != _max_value) {
							image_dest(xx, yy) += 1;
						}
					}
				}
			}
		}
	//image_dest.save("/tmp/path.tiff");
	//cout << "imagePaths::compute_from_direct_and_coule end" << endl;
}

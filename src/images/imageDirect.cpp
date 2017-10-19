#include "imageDirect.h"
#include "imagePaths.h"

#include <iostream>

using namespace std;

void imageDirect::save(const QString & filename) {
	cimg_library::CImg<unsigned short int>::save(filename.toStdString().c_str());
}

/** \brief compute the direction of the greatest slop at each pixel
 * 
 * the resulting image describes the directions with a code :
 * 0 - 7 are possible directions
 * if there is no slope, the value will be 1000
 * edges of the images directions are set to 100
 * 
 */

void imageDirect::compute_from_gel_image(const imageIntensity & gel_image,
		const parameterDirect & parameter_direct) {
	//	gel_image.save("/tmp/gel_image.tiff");

	//unsigned short int burned_threshold(imageDirect::_max_value);
	unsigned short int burned_threshold(
			parameter_direct.get_burned_pixel_threshold());
	//cerr << "imageDirect::compute_from_gel_image begin" << endl;

	imageDirect & image_direct = *this;
	image_direct.resize(gel_image.width(), gel_image.height(), 1, 1);
	//	cimg_library::CImg<float> grad(3,3,1,1);
	float grad[8];
	// processing
	// calcul des 8 gradients
	CImg_3x3(I,unsigned short int);
	int color_channel(0);
	cimg_for3x3 (gel_image,x,y,0,color_channel,I, unsigned short int)
		{

			grad[0] = Ipc - Inc;
			grad[1] = Icp - Icn;
			grad[2] = (Ipp - Inn) / 1.4;
			grad[3] = (Inp - Ipn) / 1.4;
			grad[4] = -grad[0];
			grad[5] = -grad[1];
			grad[6] = -grad[2];
			grad[7] = -grad[3];
			/*
			 grad[0]=Inc-Ipc;
			 grad[1]=Icn-Icp;
			 grad[2]=(Inn-Ipp)/1.4;
			 grad[3]=(Ipn-Inp)/1.4;
			 grad[4]=-grad[0];
			 grad[5]=-grad[1];
			 grad[6]=-grad[2];
			 grad[7]=-grad[3];
			 */
			// calcul de la direction de plus grande pente
			int j = 0;
			int mind = 100; // si plat, mind restera 100
			float mmm = 0;
			while (j < 8) {
				if (grad[j] > mmm) {
					mmm = grad[j];
					mind = j;
				}
				j++;
			}
			image_direct(x, y) = mind;
			// si saturé on transmet la valeur saturée. attention valeur arbitraire
			if (gel_image(x, y, color_channel) > burned_threshold)
				set_burned_pixel(x, y);
		}

	//le contour de l'image doit être à 100: ne rien faire
	cimg_for_borderXY(image_direct,x,y,1)
			image_direct(x, y) = 100;

	//cerr << "imageDirect::compute_from_gel_image end" << endl;

	//image_dest.save("/tmp/direct.tiff");
	//save("/tmp/direct.tiff");
	//cerr << "imageDirect::compute_from_gel_image end" << endl;

}
;

/** \brief move x and y to the next pixel toward the spot center
 * 
 * \param & x reference to x coordinate of the spot to move
 * \param & y reference to y coordinate of the spot to move
 * 
 * \result bool TRUE if the move is possible, FALSE otherwise
 *  
 */

bool imageDirect::move_to_spot_center(int & xx, int & yy) const {
	//moving bead according to directions toward the top
	switch (*(data(xx, yy))) {
	case 0:
		xx--;
		break; //move left
	case 1:
		yy--;
		break; //move up
	case 2: {
		xx--;
		yy--;
	}
		break; //move up left
	case 3: {
		xx++;
		yy--;
	}
		break; //move up right
	case 4:
		xx++;
		break; // move right
	case 5:
		yy++;
		break; // move down
	case 6: {
		xx++;
		yy++;
	}
		break; // move down right
	case 7: {
		xx--;
		yy++;
	}
		break; // move down left
	default:
		return (false);
	}
	return (true);
}

/** \brief get the number of confluents pointing to this pixel
 * 
 * \param x x coordinate of the spot
 * \param y y coordinate of the spot
 * \param paths reference on the paths
 * \param minflux the minimum flux to take confluent into account
 * \return the number of confluents
 */

int imageDirect::get_nb_confluent_for_pixel(int x, int y,
		const imagePaths & paths, int minflux) const {
	const imageDirect & direct = *this;
	int nb_confluent(0);
	if (direct(x + 1, y - 1) == 7 && paths(x + 1, y - 1) > minflux)
		nb_confluent++;
	if (direct(x + 1, y) == 0 && paths(x + 1, y) > minflux)
		nb_confluent++;
	if (direct(x + 1, y + 1) == 2 && paths(x + 1, y + 1) > minflux)
		nb_confluent++;
	if (direct(x, y + 1) == 1 && paths(x, y + 1) > minflux)
		nb_confluent++;
	if (direct(x - 1, y + 1) == 3 && paths(x - 1, y + 1) > minflux)
		nb_confluent++;
	if (direct(x - 1, y) == 4 && paths(x - 1, y) > minflux)
		nb_confluent++;
	if (direct(x - 1, y - 1) == 6 && paths(x - 1, y - 1) > minflux)
		nb_confluent++;
	if (direct(x, y - 1) == 5 && paths(x, y - 1) > minflux)
		nb_confluent++;

	return (nb_confluent);
}


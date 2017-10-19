#include <iostream>
#include "imageContours.h"
#include "imageNumber.h"

using namespace std;

void imageContours::compute_from_gel_image_and_numeros(
		const imageIntensity & image_depart, const imageNumber & image_numeros) {

	bool inverse;

	inverse = image_depart.get_is_inversed();

	imageContours & image_contours = *this;
	//img1 => image_detect
	//img2 => image_depart
	image_contours.resize(image_depart.width(), image_depart.height(), 1, 1); //mark
	//  cimg_library::CImg<unsigned short int> dest=img1;
	//  cimg_library::CImg<unsigned short int> mark=img2;
	//	cout << "Quantif- initialisation à zero de image_contours" << endl;

	// pour contour on remplit avec 1 car sinon confusion qnad image inversée
	image_contours.fill(1);
	// processing

	/*
	 * déjà fait par imageNumber::compute_from_gel_image
	 // un petit coup de median d'abord
	 printf("Quantif- lissage de l'image\n");
	 image_depart.blur_median(n=3);
	 */

	// on fait bouger les frontieres:
	// pour tout pixel frontiere entre 2 spots, on calcule la direction de plus grande
	// pente. Le pixel changera de numero si besoin.
	// non car comme ca on va éliminer tous les spots non maximum locaux (épaulements)


	// début contour
	//unsigned short int spot_color = 65535-inverse*65535; 
	unsigned short int spot_color = this->_max_value; //white

	//	printf("Quantif- contours\n");
	cimg_for_insideXY(image_numeros,x,y,1)
		{
			if (image_numeros(x, y) == 0) {
			}

			else if (image_numeros(x, y) > image_numeros(x + 1, y))
				image_contours(x, y) = spot_color;
			else if (image_numeros(x, y) > image_numeros(x, y + 1))
				image_contours(x, y) = spot_color;
			else if (image_numeros(x, y) > image_numeros(x - 1, y))
				image_contours(x, y) = spot_color;
			else if (image_numeros(x, y) > image_numeros(x, y - 1))
				image_contours(x, y) = spot_color;
			else if ((x + 1) == (image_contours.width() - 1)) {
				if (image_numeros(x, y) > 0) {
					image_contours(x, y) = spot_color;
				}
			} else if ((y + 1) == (image_contours.height() - 1)) {
				if (image_numeros(x, y) > 0) {
					image_contours(x, y) = spot_color;
				}
			} else if ((x - 1) == 0) {
				if (image_numeros(x, y) > 0) {
					image_contours(x, y) = spot_color;
				}
			} else if ((y - 1) == 0) {
				if (image_numeros(x, y) > 0) {
					image_contours(x, y) = spot_color;
				}
			}
		}

	// fin contour

	//	printf("Quantif- mask\n");
	//mark = image_depart masquée par mark
	cimg_for_insideXY(image_contours,x,y,1)
		{
			if (image_contours(x, y) == 1)
				image_contours(x, y) = image_depart(x, y);
		}

}
;

void imageContours::draw_contours(const detection & my_detection, bool inverse) {
	imageContours & image_contours = *this;

	// dessin des croix au centre des spots
	//unsigned short int stroke_color = 65535-inverse*65535;
	unsigned short int stroke_color = 0; //black 
	detection::const_iterator it;
	int xx, yy;
	for (it = my_detection.begin(); it != my_detection.end(); ++it) {
		const spot & the_spot = (*it).second;
		yy = (int) the_spot.get_ty();
		for (xx = std::max<int>((int) the_spot.get_tx() - 2, 0); xx < std::min<
				int>((int) the_spot.get_tx() + 3, image_contours.width()); xx++) {

			image_contours(xx, yy) = stroke_color;
		}
		xx = (int) the_spot.get_tx();
		for (yy = std::max<int>((int) the_spot.get_ty() - 2, 0); yy < std::min<
				int>((int) the_spot.get_ty() + 3, image_contours.height()); yy++) {
			image_contours(xx, yy) = stroke_color;
		}
	}

}
;

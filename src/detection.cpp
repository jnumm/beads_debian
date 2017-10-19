#include "detection.h"
#include "images/imageNumber.h"
#include "qtbeads/qtbeads_error.h"

#include <iostream>
using namespace std;

detection::detection() :
	spot_map() {
	_image_file_name = "";
	_gel_image_width = 0;
	_gel_image_height = 0;
}
;

detection::detection(const detection & src) :
	spot_map(src) {
	_image_file_name = src._image_file_name;
	_gel_image_width = src._gel_image_width;
	_gel_image_height = src._gel_image_height;
}

detection::~detection() {
}

spot & detection::get_spot(unsigned int spot_number) {

	spot_map::iterator it = this->find(spot_number);

	if (it == this->end()) {
		throw qtbeadsError(
				QObject::tr(
						"trying to get the spot number '%1' which does not exists in this detection").arg(spot_number));
	}
	else {
		return it->second;
	}

}

/** \brief creates a collection of spot from an imageDetection object
 * 
 * on imageDetection, each pixel with intensity > 0 means we have a "spot"
 * imageDetection is a way to store detected spots and this function gives a way to read this detection
 */

detection::detection(const imageDetection & image_detect) {
	spot_map & _spots = *this;
	unsigned int nspot;

	nspot = 0;

	cimg_for_insideXY(image_detect,x,y,1)
		{
			if (image_detect(x, y) > 0) {
				//on a un spot:
				_spots[++nspot].set_x(x);
				_spots[nspot].set_number(nspot);
				_spots[nspot].set_y(y);
			}
		}
}
;

/** \brief detect spots from imageProb
 * 
 * imageProb gives potential spot positions
 * given a threshold, this try to find spot positions
 */
void detection::detect_from_prob(const imageProb & image_prob,
		const parameterDetection & parameter_detection) {

	//cerr << "detection::detect_from_prob begin" << endl;
	//image_prob.save("/tmp/prob.tiff");

	float minproba = parameter_detection.get_minproba();
	spot_map & _spots = *this;

	_gel_image_width = image_prob.width();
	_gel_image_height = image_prob.height();

	unsigned int nspot;

	nspot = 0;

	imageCode mark;
	//  cimg_library::CImg<unsigned short int> mark=img1;
	//  cimg_library::CImg<unsigned short int> dest=img1;
	mark.resize(image_prob.width(), image_prob.height(), 1, 1);
	mark.fill(0);
	// processing
	//
	// on recherche un voisin plus haut dans une fenêtre 3 par 3, et on y va
	cimg_for_insideXY(image_prob,x,y,1)
		{
			int xx, yy;
			int atteint = 0;
			xx = x;
			yy = y;
			if (mark(xx, yy) == 0) {
				if (image_prob(xx, yy) == imageProb::_max_value) {
					//this pixel is burned :
					//cerr << "detection::detect_from_prob burned" << endl;
					// we have to find neighbors and group it into a spot
					nspot++;

					image_prob.mark_burned_pixels(mark, xx, yy);

					_spots[nspot].set_x(xx);
					_spots[nspot].set_number(nspot);
					_spots[nspot].set_y(yy);
					//cerr << "detection::detect_from_prob burned end" << endl;


				} else {
					if (image_prob(xx, yy) > minproba) {
						//find the maximum of intensity and make a spot with it

						//       	 printf("1 %d %d %d\n",xx,yy,atteint);
						atteint = image_prob.amas(xx, yy, mark);
						//       	 printf("2 %d %d %d\n",xx,yy,atteint);
						while (atteint == 0) {
							atteint = image_prob.amas(xx, yy, mark);
							//       	       printf("3 %d %d %d\n",xx,yy,atteint);
						}
						if (atteint == 1) {
							//on a un spot:
							_spots[++nspot].set_x(xx);
							_spots[nspot].set_number(nspot);
							_spots[nspot].set_y(yy);

							//				std::cout << "spot " << _spots[nspot].get_number() << " x "  << _spots[nspot].get_x() << " y "<< _spots[nspot].get_y() << std::endl;
						}
						//	 printf("4 %d %d %d\n",xx,yy,atteint);
					}
				}
			}
		}
	printf("%d spots detected\n", nspot);
}
;

/** \brief compute quantitations using original image and "contours"
 * 
 * on each spot, this will give :
 * - the minimum intensity (on which the background is estimated)
 * - the spot surface in pixels
 * - the volume
 * - the spot barycenter (tx and ty on each spot)
 * - the background
 * 
 * if a spot has a surface of 0, this is deleted
 */

void detection::quantify_spots_with_image(
		const cimg_library::CImg<int> & image_depart,
		const imageNumber & image_numeros) {
	detection & spot_array = *this;
	cout << "Computing volume, surface and background" << endl;

	qDebug() << "detection::quantify_spots_with_image coucou spot_array size "
			<< spot_array.size();

	// dans un premier temps on ne recherche que les min et max dans chaque spot (tmin et tmax)
	cimg_forXY(image_numeros,x,y)
		{
			//qDebug() << "detection::quantify_spots_with_image x " << x << " y "
			//		<< y;

			unsigned int spot_number = image_numeros(x, y);

			//qDebug() << "detection::quantify_spots_with_image image_numeros("
			//		<< x << "," << y << ") = " << image_numeros(x, y);

			if (spot_number > 0) {
				spot & current_detected_spot = this->get_spot(spot_number);
				//cout << image_numeros(x,y) << endl;
				// vol[image_numeros(x,y)] += vol[image_numeros(x,y)]
				//			spot_array[image_numeros(x,y)].set_vol(spot_array[image_numeros(x,y)].get_vol() + image_depart(x,y));
				//			spot_arrayimage_numeros(x,y)].set_area(spot_array[image_numeros(x,y)].get_area() + 1);

				if (image_depart(x, y) < (long int) current_detected_spot.get_tmin())
					current_detected_spot.set_tmin(image_depart(x, y));
				if (image_depart(x, y) > (long int) current_detected_spot.get_tmax())
					current_detected_spot.set_tmax(image_depart(x, y));
			}
		}

	qDebug() << "detection::quantify_spots_with_image coucou 1";
	// correction du minimum au cas ou on tombe sur une valeur extreme
	detection::iterator it;
	for (it = spot_array.begin(); it != spot_array.end(); ++it) {
		spot & the_spot = (*it).second;
		//		cout << " tmin avant correction: " <<  the_spot.get_tmin() << endl;
		//		the_spot.set_tmin((long unsigned int) (the_spot.get_tmin()+(((float) the_spot.get_tmax()-(float)the_spot.get_tmin())/(float)2)));
		the_spot.set_tmin((long unsigned int) ((float) the_spot.get_tmin()
				* 1.1));
		//		cout << " tmin après correction: " <<  the_spot.get_tmin() << endl;
	}

	qDebug() << "detection::quantify_spots_with_image coucou 2";
	// on peut mantenant calculer les volumes, surfaces, et barycentres, en ne prenant que
	// les valeurs supérieures au tmin.
	cimg_forXY(image_numeros,x,y)
		{
			if ((image_numeros(x, y) > 0) && (image_depart(x, y)
					> (long int) spot_array[image_numeros(x, y)].get_tmin())) {
				// vol[image_numeros(x,y)] += vol[image_numeros(x,y)]
				spot_array[image_numeros(x, y)].set_vol(
						spot_array[image_numeros(x, y)].get_vol()
								+ image_depart(x, y));
				spot_array[image_numeros(x, y)].set_area(
						spot_array[image_numeros(x, y)].get_area() + 1);
				spot_array[image_numeros(x, y)].set_tx(
						spot_array[image_numeros(x, y)].get_tx()
								+ ((image_depart(x, y)
										- spot_array[image_numeros(x, y)].get_tmin())
										* x));
				spot_array[image_numeros(x, y)].set_ty(
						spot_array[image_numeros(x, y)].get_ty()
								+ ((image_depart(x, y)
										- spot_array[image_numeros(x, y)].get_tmin())
										* y));
			}
		}
	qDebug() << "detection::quantify_spots_with_image coucou 3";

	//compute background and delete spots that would have a null surface or a background greater than volume :
	detection::iterator to_delete;
	for (it = spot_array.begin(); it != spot_array.end();) {
		spot & the_spot = (*it).second;
		the_spot.compute_background();
		if ((the_spot.get_area() == 0) || ((long int) the_spot.get_bckgnd()
				> the_spot.get_vol())) {
			//on efface les spots non quantifiés :
			to_delete = it;
			++it;
			//cout << " spot deleted " << endl;
			spot_array.erase(to_delete);
		} else {
			++it;
		}
	}
	qDebug() << "detection::quantify_spots_with_image coucou 4";

	//means (moyenne des x et y pondérée par les intensités) 
	cout << "Computing barycenter coordinates" << endl;

	for (it = spot_array.begin(); it != spot_array.end(); ++it) {
		spot & the_spot = (*it).second;
		the_spot.set_tx(the_spot.get_tx() / ((float) the_spot.get_vol()
				- (float) the_spot.get_bckgnd()));
		the_spot.set_ty(the_spot.get_ty() / ((float) the_spot.get_vol()
				- (float) the_spot.get_bckgnd()));
	}
}

/** \brief add edges coordinates on each spot of the collection
 * 
 * the imageNumeros represent the surface of each spot. With this image, we can store
 * the shape of this spot as "spot edges"
 * It is usefull to represent spots in SVG drawings
 */

void detection::store_spot_edges(const imageNumber & image_numeros) {
	qDebug() << "begin detection::store_spot_edges begin";
	//for each spot, scan imageNumber to find coordinates of edges
	detection & spot_array = *this;
	// début contour
	//for each spot, scan imageNumber to find coordinates of edges
	cimg_for_insideXY(image_numeros,x,y,1)
		{
			int spot_num = image_numeros(x, y);
			if (spot_num > 0) {
				spot & the_spot = spot_array[spot_num];
				if (spot_num != image_numeros(x + 1, y))
					the_spot.add_edge_point(x, y);
				else if (spot_num != image_numeros(x, y + 1))
					the_spot.add_edge_point(x, y);
				else if (spot_num != image_numeros(x - 1, y))
					the_spot.add_edge_point(x, y);
				else if (spot_num != image_numeros(x, y - 1))
					the_spot.add_edge_point(x, y);
				else if ((x + 1) == (image_numeros.width() - 1)) {
					the_spot.add_edge_point(x, y);
				} else if ((y + 1) == (image_numeros.height() - 1)) {
					the_spot.add_edge_point(x, y);
				} else if ((x - 1) == 0) {
					the_spot.add_edge_point(x, y);
				} else if ((y - 1) == 0) {
					the_spot.add_edge_point(x, y);
				}
			}
		}
	detection::iterator it;
	for (it = begin(); it != end(); ++it) {
		//int spot_num = (*it).first;
		spot & the_spot = (*it).second;
		the_spot.sort_edge_points();
		the_spot.eliminate_aligned_edge_points();
	}
	qDebug() << "begin detection::store_spot_edges end";
}


#include <iostream>
#include "imageNumber.h"

using namespace std;

void imageNumber::save(const QString & filename) {
	cimg_library::CImg<short int>::save(filename.toStdString().c_str());
}

/** \brief fusion of 2 detected spots
 * 
 * the image number is updated and the second spot is erased from the detection
 */
void imageNumber::fusion(int spot_number_1, int spot_number_2,
		detection & the_detection) {
	if (spot_number_1 == spot_number_2) {
		return;
	}
	if (spot_number_1 == -spot_number_2) {
		return;
	}
	imageNumber & image_numeros = *this;

	//cerr << "imageNumber::fusion " << spot_number_1 << " " << spot_number_2 << endl;
	cimg_for_insideXY(image_numeros,x,y,1)
		{
			if (image_numeros(x, y) == spot_number_2) {
				image_numeros(x, y) = spot_number_1;
			} else if (image_numeros(x, y) == -spot_number_2) {
				image_numeros(x, y) = -spot_number_1;
			}
		}
	if (spot_number_2 > 0) {
		the_detection.erase(spot_number_2);
	} else {
		the_detection.erase(-spot_number_2);
	}
}

void imageNumber::set_numbers_with_detection(detection & the_detection) {
	imageNumber & image_numeros = *this;

	qDebug()
			<< "imageNumber::set_numbers_with_detection(detection & the_detection) begin size"
			<< the_detection.size();
	image_numeros.fill(0);

	// préliminaire 1: on numérote les spots.
	// si un voisin du pixel a déjà été marqué, on le marque avec le meme numero (proc voisin)
	//sinon on le marque avec un nouveau numero.


	detection::iterator it, to_delete;
	int spot_num, x, y;
	for (it = the_detection.begin(); it != the_detection.end();) {
		spot_num = (*it).first;
		x = (*it).second.get_x();
		y = (*it).second.get_y();
		int spot_num_neigbhor = image_numeros.neighbor_number(x, y);
		if (spot_num_neigbhor == 0) {
			//the spot has no neighbors, the spot number is set to this pixel
			image_numeros(x, y) = spot_num;
			++it;
		} else {
			//the spot has a neighbor, the pixel takes the neighbors spot number and
			// the spot is deleted from the detection:
			image_numeros(x, y) = spot_num_neigbhor;
			//provisoirement on ne le marque pas du tout
			//			image_numeros(x,y)=spot_num;
			to_delete = it;
			it++;
			qDebug()
					<< "imageNumber::set_numbers_with_detection(detection & the_detection) erase "
					<< to_delete->first;
			the_detection.erase(to_delete);
		}
	}
	qDebug()
			<< "imageNumber::set_numbers_with_detection(detection & the_detection) end size "
			<< the_detection.size();
}

void imageNumber::enlarge_and_fusion(detection & my_detection, int enlarge,
		const imageDirect & direct) {
	imageNumber & image_numeros = *this;

	/* ici on détermine l'intervale de confiance pour la position de
	 * chaque spot. fait comme la 1ere passe de l'ancienne version.
	 * si en s'élargissant les zones de 2 spots se rencontrent, elles fusionnent: il n'y a plus qu'un spot
	 */

	for (int cpass = 0; cpass < enlarge; cpass++) {
		//cerr << "imageNumber::enlarge_and_fusion cpass: " << cpass << endl;
		cimg_for_insideXY(image_numeros,x,y,1)
			{
				if (direct.is_burned_pixel(x, y)) {
					//don't enlarge and fusion when the zone is burned
				} else {
					if (image_numeros(x, y) > 0) {
						if (image_numeros(x + 1, y - 1) == 0) {
							image_numeros(x + 1, y - 1) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y), image_numeros(x + 1, y
									- 1), my_detection);
						}
						if (image_numeros(x + 1, y) == 0) {
							image_numeros(x + 1, y) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y),
									image_numeros(x + 1, y), my_detection);
						}
						if (image_numeros(x + 1, y + 1) == 0) {
							image_numeros(x + 1, y + 1) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y), image_numeros(x + 1, y
									+ 1), my_detection);
						}
						if (image_numeros(x, y + 1) == 0) {
							image_numeros(x, y + 1) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y),
									image_numeros(x, y + 1), my_detection);
						}
						if (image_numeros(x - 1, y + 1) == 0) {
							image_numeros(x - 1, y + 1) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y), image_numeros(x - 1, y
									+ 1), my_detection);
						}
						if (image_numeros(x - 1, y) == 0) {
							image_numeros(x - 1, y) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y),
									image_numeros(x - 1, y), my_detection);
						}
						if (image_numeros(x - 1, y - 1) == 0) {
							image_numeros(x - 1, y - 1) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y), image_numeros(x - 1, y
									- 1), my_detection);
						}
						if (image_numeros(x, y - 1) == 0) {
							image_numeros(x, y - 1) = -image_numeros(x, y);
						} else {
							fusion(image_numeros(x, y),
									image_numeros(x, y - 1), my_detection);
						}
					}
				}
			}

		// 2eme phase: on ne touche qu'aux négatifs: on les met en positifs.

		cimg_for_insideXY(image_numeros,x,y,1)
			{
				if (image_numeros(x, y) < 0) {
					image_numeros(x, y) = -image_numeros(x, y);
				}
			}
		// fin cpass sans test sur image de départ
	}
}

void imageNumber::bouche_trou() {
	imageNumber & image_numeros = *this;
	//bouche-trou bas de gamme
	const int ncols = image_numeros.width();
	const int nrows = image_numeros.height();
	int nc, nr, ncdep, nnc;
	int couldep, coularr;
	for (nr = 1; nr < nrows - 1; nr++) {
		nc = 0;
		while (nc < ncols - 1) {
			while (nc < ncols - 1 && image_numeros(nc, nr) > 0) {
				nc++;
			}
			if (nc < ncols - 1) {
				ncdep = nc;
				couldep = image_numeros(nc - 1, nr);
				while (nc < ncols - 1 && image_numeros(nc, nr) == 0) {
					nc++;
				}
				if (nc < ncols - 1) {
					coularr = image_numeros(nc, nr);
					if (couldep == coularr) {
						for (nnc = ncdep; nnc < nc; nnc++) {
							image_numeros(nnc, nr) = couldep;
						}
					}
				}
			}
		}
	}
}

void imageNumber::enlarge(int enlarge) {
	imageNumber & image_numeros = *this;

	// --------------------------------------------------------------------------
	// partie à boucler en principe tant que ca bouge encore.

	// 1ere phase: on marque en négatif les voisins des surfaces déjà marquées,
	//avec la meme valeur, en négatif pour les distinguer des plus anciens,
	// et donc n'avancer que d'un pixel à la fois pour tout le monde
	// c'est important car sinon je suis obligé de calculer des distances quand
	// il y a conflit entre 2 spots pour l'occupation d'un pixel.
	// attention au signe de la différence dans img2: contre-intuitif car
	//ici fortes valeurs= zones claires=fond

	//attention, pour boucher d'entrée les trous qui sont dus à un départ
	//trop loin du max, on ne fait pas de test sur l'intensité dans l'image au
	//cours des 4 premières passes.

	/* ici on détermine l'intervale de confiance pour la position de
	 * chaque spot. fait comme la 1ere passe de l'ancienne version.
	 */

	for (int cpass = 0; cpass < enlarge; cpass++) {
		//		cout << "imageNumber::enlarge cpass: " << cpass << endl;
		cimg_for_insideXY(image_numeros,x,y,1)
			{
				if (image_numeros(x, y) > 0) {
					if (image_numeros(x + 1, y - 1) == 0) {
						image_numeros(x + 1, y - 1) = -image_numeros(x, y);
					}

					if (image_numeros(x + 1, y) == 0) {
						image_numeros(x + 1, y) = -image_numeros(x, y);
					}

					if (image_numeros(x + 1, y + 1) == 0) {
						image_numeros(x + 1, y + 1) = -image_numeros(x, y);
					}

					if (image_numeros(x, y + 1) == 0) {
						image_numeros(x, y + 1) = -image_numeros(x, y);
					}

					if (image_numeros(x - 1, y + 1) == 0) {
						image_numeros(x - 1, y + 1) = -image_numeros(x, y);
					}

					if (image_numeros(x - 1, y) == 0) {
						image_numeros(x - 1, y) = -image_numeros(x, y);
					}

					if (image_numeros(x - 1, y - 1) == 0) {
						image_numeros(x - 1, y - 1) = -image_numeros(x, y);
					}

					if (image_numeros(x, y - 1) == 0) {
						image_numeros(x, y - 1) = -image_numeros(x, y);
					}

				}
			}

		// 2eme phase: on ne touche qu'aux négatifs: on les met en positifs.

		cimg_for_insideXY(image_numeros,x,y,1)
			{
				if (image_numeros(x, y) < 0) {
					image_numeros(x, y) = -image_numeros(x, y);
				}
			}
		// fin cpass sans test sur image de départ
	}
}

void imageNumber::set_burned_zone_number(const imageDirect & direct,
		int number, int x, int y, unsigned int & depth) {

	depth++;
	if (depth > 1000) {
		depth--;
		return;
	}

	imageNumber & image_number = *this;
	//if the spot is burned
	if (direct.is_burned_pixel(x, y)) {
		if (image_number(x, y) == 0) {
			// set image_number of the whole burned area :
			image_number(x, y) = number;
			set_burned_zone_number(direct, number, x + 1, y, depth);
			set_burned_zone_number(direct, number, x, y + 1, depth);
			set_burned_zone_number(direct, number, x - 1, y, depth);
			set_burned_zone_number(direct, number, x, y - 1, depth);
		}
	}
	depth--;
}

void imageNumber::enlarge_burned_areas(const imageDirect & direct,
		detection & the_detection) {
	//imageNumber & image_number = *this;
	detection::iterator it;
	for (it = the_detection.begin(); it != the_detection.end(); ++it) {
		spot & the_spot = (*it).second;
		int x(the_spot.get_x()), y(the_spot.get_y());
		//if the spot is burned
		if (direct.is_burned_pixel(x, y)) {
			unsigned int depth(0);
			set_burned_zone_number(direct, the_spot.get_number(), x + 1, y,
					depth);
			set_burned_zone_number(direct, the_spot.get_number(), x, y + 1,
					depth);
			set_burned_zone_number(direct, the_spot.get_number(), x - 1, y,
					depth);
			set_burned_zone_number(direct, the_spot.get_number(), x, y - 1,
					depth);
		}

	}

}

/** set image numbers with the detection and directions
 * 
 * 1) attribute numbers from the detection (with the spot coordinates)
 * 2) enlarge burned areas
 * 3) enlarge spot areas with enlarge parameter
 * 4) with directions, drop beads and let them flow to the spot area. This will give the full spot area
 */

void imageNumber::compute_from_direct(const imageDirect & direct,
		detection & my_detection, const parameterNumber & parameter_number) {

	qDebug() << "imageNumber::compute_from_direct begin";

	int enlarge_param = parameter_number.get_enlarge();
	int enlarge_fusion_param = parameter_number.get_enlarge_fusion();

	// un petit coup de median d'abord
	// devenu inutile ici
	/*
	 cout << "Quantif- lissage de l'image" << endl;
	 int n;
	 gel_image.blur_median(n=3);
	 */

	imageNumber & image_numeros = *this;
	image_numeros.resize(direct.width(), direct.height(), 1, 1); //dest

	//	printf("Quantif- reperage spots\n");


	// 1) attribute numbers from the detection (with the spot coordinates)
	set_numbers_with_detection(my_detection);

	cout << "Quantif- spots left: " << my_detection.size() << endl;

	// 2) enlarge burned areas
	enlarge_burned_areas(direct, my_detection);

	// 3) enlarge spot areas with enlarge parameter
	enlarge_and_fusion(my_detection, enlarge_fusion_param, direct);
	enlarge(enlarge_param);

	// 4) with directions, drop beads and let them flow to the spot area. This will give the full spot area

	/* maintenant je coule à partir de direct, et le numéro trouvé à l'arrivée dans image_numeros
	 * est donné au point de départ. Si le point d'arrivée n'est pas affecté à un spot, je donne
	 * la valeur 0
	 */
	cimg_forXY(direct,x,y)
		{
			int xx = x;
			int yy = y;
			int i = 0;
			int curr;
			// on s'arrete des qu'on penetre dans une zone occupée par un carré de spot
			// attention ca risque de faire aller jusqu'à 100 pour bcp de spots du bruit de
			// fond
			while (i < 100 && image_numeros(xx, yy) == 0) {
				i++;
				curr = (int) direct(xx, yy);
				switch (curr) {
				case 0:
					xx--;
					break;
				case 1:
					yy--;
					break;
				case 2: {
					xx--;
					yy--;
				}
					break;
				case 3: {
					xx++;
					yy--;
				}
					break;
				case 4:
					xx++;
					break;
				case 5:
					yy++;
					break;
				case 6: {
					xx++;
					yy++;
				}
					break;
				case 7: {
					xx--;
					yy++;
				}
					break;
				}
			}
			// on ne fait rien en cas de plat (= case -1): xx et yy sont inchangés
			// est-ce judicieusement fait ?

			// on attribue au pixel de départ le numero du pixel d'arrivée.
			// si le pixel d'arrivée est négatif (déjà passé) ou nul (pas de spot là), on met -1
			if (image_numeros(xx, yy) > 0)
				image_numeros(x, y) = image_numeros(xx, yy);
			else
				image_numeros(x, y) = -1;
		}

	// juste par sécurité on supprime les pixels à -1 qui n'ont servi qu'à voir où on était
	// déjà passé.
	cimg_forXY(image_numeros,x,y)
		{
			if (image_numeros(x, y) < 0)
				image_numeros(x, y) = 0;
		}

	//	printf ("Quantif- bouche-trou\n");

	bouche_trou();
	qDebug() << "imageNumber::compute_from_direct end";

}

/** \brief look for a neighboring pixel with a number > 0
 *
 * if a neighbor pixel with a number is found, return the neighbor pixel number
 */
// procedure voisin
int imageNumber::neighbor_number(int x, int y) const {
	const imageNumber & image_numeros = *this;
	if (image_numeros(x + 1, y - 1) > 0) {
		return (image_numeros(x + 1, y - 1));
	} else if (image_numeros(x + 1, y) > 0) {
		return image_numeros(x + 1, y);
	} else if (image_numeros(x + 1, y + 1) > 0) {
		return image_numeros(x + 1, y + 1);
	} else if (image_numeros(x, y + 1) > 0) {
		return image_numeros(x, y + 1);
	} else if (image_numeros(x - 1, y + 1) > 0) {
		return image_numeros(x - 1, y + 1);
	} else if (image_numeros(x - 1, y) > 0) {
		return image_numeros(x - 1, y);
	} else if (image_numeros(x - 1, y - 1) > 0) {
		return image_numeros(x - 1, y - 1);
	} else if (image_numeros(x, y - 1) > 0) {
		return image_numeros(x, y - 1);
	} else
		return 0;
}

void imageNumber::expand_areas_down_the_slope(const imageIntensity & gel_image,
		int npass) {
	//int nspot=my_detection.size();
	imageNumber & image_numeros = *this;

	for (int cpass = 0; cpass < npass; cpass++) {
		//		cout << "imageNumber::expand_areas_down_the_slope " << cpass << endl;
		cimg_for_insideXY(image_numeros,x,y,1)
			{
				if (image_numeros(x, y) > 0) {
					if (image_numeros(x + 1, y - 1) == 0 && (gel_image(x + 1, y
							- 1) < gel_image(x, y))) {
						image_numeros(x + 1, y - 1) = -image_numeros(x, y);
					}
					if (image_numeros(x + 1, y) == 0 && (gel_image(x + 1, y)
							< gel_image(x, y))) {
						image_numeros(x + 1, y) = -image_numeros(x, y);
					}
					if (image_numeros(x + 1, y + 1) == 0 && (gel_image(x + 1, y
							+ 1) < gel_image(x, y))) {
						image_numeros(x + 1, y + 1) = -image_numeros(x, y);
					}
					if (image_numeros(x, y + 1) == 0 && (gel_image(x, y + 1)
							< gel_image(x, y))) {
						image_numeros(x, y + 1) = -image_numeros(x, y);
					}
					if (image_numeros(x - 1, y + 1) == 0 && (gel_image(x - 1, y
							+ 1) < gel_image(x, y))) {
						image_numeros(x - 1, y + 1) = -image_numeros(x, y);
					}
					if (image_numeros(x - 1, y) == 0 && (gel_image(x - 1, y)
							< gel_image(x, y))) {
						image_numeros(x - 1, y) = -image_numeros(x, y);
					}
					if (image_numeros(x - 1, y - 1) == 0 && (gel_image(x - 1, y
							- 1) < gel_image(x, y))) {
						image_numeros(x - 1, y - 1) = -image_numeros(x, y);
					}
					if (image_numeros(x, y - 1) == 0 && (gel_image(x, y - 1)
							< gel_image(x, y))) {
						image_numeros(x, y - 1) = -image_numeros(x, y);
					}
				}
			}

		// 2eme phase: on ne touche qu'aux négatifs: on les met en positifs.

		cimg_forXY(image_numeros,x,y)
			{
				if (image_numeros(x, y) < 0) {
					image_numeros(x, y) = -image_numeros(x, y);
				}
			}

		// fin npass
	}
	bouche_trou();

}


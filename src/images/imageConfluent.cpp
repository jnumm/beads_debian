#include <iostream>

// ---------------------------------------------------------------------------
// confluent
// ---------------------------------------------------------------------------
#include "imageConfluent.h"

using namespace std;

void imageConfluent::compute_from_direct_and_paths(const imageDirect & direct,
		const imagePaths & paths, const imageIntensity & gel_image,
		const parameterConfluent & parameter_confl) {

	imageConfluent & confluent = *this;

	int minflux = parameter_confl.get_minflux();
	int minpath = parameter_confl.get_minpath();
	int winconfl = parameter_confl.get_winconfl();
	int minbeads = parameter_confl.get_minbeads();
	int intmax = parameter_confl.get_intmax();
	int minpct = parameter_confl.get_minpct();

	confluent.resize(direct.width(), direct.height(), 1, 1);
	confluent.fill(0);
	// processing
	int confl, is_max;
	//	cimg_forXY(direct,x,y) {
	cimg_for_insideXY(direct,x,y,1)
		{
			is_max = 0;

			/* on est plus indulgent dans le cas où plus de 2 chemins convergent.
			 * A la limite il n'y a plus besoin d'indiquer le nb de confluents rechercé */
			confl = direct.get_nb_confluent_for_pixel(x, y, paths, (int) (minflux
					* 0.8));
			if (confl == 2)
				confl = direct.get_nb_confluent_for_pixel(x, y, paths, minflux);

			/*
			 if (direct(x+1,y-1)==7 && paths(x+1,y-1)>flux) confl++;
			 if (direct(x+1,y)==0 && paths(x+1,y)>flux) confl++;
			 if (direct(x+1,y+1)==2 && paths(x+1,y+1)>flux) confl++;
			 if (direct(x,y+1)==1 && paths(x,y+1)>flux) confl++;
			 if (direct(x-1,y+1)==3 && paths(x-1,y+1)>flux) confl++;
			 if (direct(x-1,y)==4 && paths(x-1,y)>flux) confl++;
			 if (direct(x-1,y-1)==6 && paths(x-1,y-1)>flux) confl++;
			 if (direct(x,y-1)==5 && paths(x,y-1)>flux) confl++;
			 */

			// si on est sur un confluent, on va chercher le maximum local vers où il va en suivant la
			// direction indiquée par direct. On peut alors éliminer le confluent dans le cas où le
			// ne concernait qu'une petite surface par rapport au maximum local.

			if (confl >= minpath) {
				int xx = x;
				int yy = y;
				int i = 0;
				//int curr;
				unsigned short int oldpix = paths(x, y) - 1;
				unsigned short int newpix = paths(x, y);
				unsigned short int pixconfl = paths(x, y);
				int distance = 0;
				// on s'arrete des qu'on penetre dans une zone occupée par un carré de spot
				// attention ca risque de faire aller jusqu'à 100 pour bcp de spots du bruit de
				// fond
				while (i < 100 && newpix > oldpix) {
					i++;
					if (direct.move_to_spot_center(xx, yy)) {
						//move ok
					}
					/*
					 curr=(int)direct(xx,yy);
					 switch (curr) {
					 case 0: xx--; break;
					 case 1: yy--; break;
					 case 2: {xx--; yy--;} break;
					 case 3: {xx++; yy--;} break;
					 case 4: xx++; break;
					 case 5: yy++; break;
					 case 6: {xx++; yy++;} break;
					 case 7: {xx--; yy++;} break;
					 }
					 */
					distance++;
					oldpix = newpix;
					newpix = paths(xx, yy);
				}
				// on ne fait rien en cas de plat (= case -1): xx et yy sont inchangés
				// est-ce judicieusement fait ?

				if (float(pixconfl) * 100 / newpix < minpct)
					confl = 0;

			}

			/* dans le même temps je prends le maximum local */
			if (paths(x, y) > minbeads) {
				if ((paths(x + 1, y - 1) < paths(x, y)) && (paths(x + 1, y)
						< paths(x, y)) && (paths(x + 1, y + 1) < paths(x, y))
						&& (paths(x, y + 1) < paths(x, y)) && (paths(x - 1, y
						+ 1) < paths(x, y)) && (paths(x - 1, y) < paths(x, y))
						&& (paths(x - 1, y - 1) < paths(x, y)) && (paths(x, y
						- 1) < paths(x, y)))
					is_max = 1;
			}

			// on enleve les confluents sur les spots intenses:la plupart du temps c'est artefactuel.
			if (confl >= minpath && gel_image(x, y) < intmax)
				confluent(x, y) = winconfl;
			//		if (is_max==1) confluent(x,y)=winconfl+paths(x,y);
			if (is_max == 1)
				confluent(x, y) = paths(x, y);
		}

	// transmission valeurs saturées
	cimg_for_insideXY(direct,x,y,1)
		{
			if (direct.is_burned_pixel(x, y))
				confluent(x, y) = _max_value;
		}

	//save("/tmp/confluent.tiff");
	//cerr << "imageConfluent::compute_from_direct_and_paths end" << endl;

}


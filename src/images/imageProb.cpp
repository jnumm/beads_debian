#include "imageProb.h"
#include <iostream>
#include <vector>
using namespace std;

class kernel_prob {
public:

	kernel_prob(float sx, float sy) {
		generate(sx, sy);
	}

	~kernel_prob() {
	}

	void update(float sx, float sy) {
		if ((sx != _sx) || (sy != _sy)) {
			//cerr << "kernel_prob::update " << sx << " " << sy << endl;
			generate(sx, sy);
		}
	}

	void generate(float sx, float sy) {
		_ncols = 51;
		_nrows = 51;
		_kern.resize(_ncols, vector<float> (_nrows, 0));
		int nc0 = _ncols / 2;
		int nr0 = _nrows / 2;
		//int nc;
		//int nr;
		for (int nc = 0; nc < _ncols; nc++) {
			for (int nr = 0; nr < _nrows; nr++) {
				float fnc = nc - nc0;
				float fnr = nr - nr0;
				float valx = (fnc * fnc) / (2 * sx * sx);
				float valy = (fnr * fnr) / (2 * sy * sy);
				_kern[nc][nr] = std::exp(-valx - valy);
			}
		}
		_sx = sx;
		_sy = sy;
	}

	int _ncols;
	int _nrows;
	vector<vector<float> > _kern;

private:
	float _sx;
	float _sy;
};

void imageProb::compute_from_image_intensity(const imageIntensity & coule,
		const parameterProb & parameter_prob) {
	imageProb & image_prob = *this;

	float threshold = parameter_prob.get_threshold();
	float sx = parameter_prob.get_sx();
	float sy = parameter_prob.get_sy();
	/*
	 * Pour traiter les pixels des bords sans faire de cas particulier,
	 * création d'une image prob plus grande que l'image d'origine.
	 * 20 pixels de plus dans chaque dimension, elle dépasse de 10 dans
	 * les 4 directions. Cette bordure est ensuite enlevée par crop en
	 * fin de programme.
	 */
	unsigned int margin(50);
	unsigned int half_margin(margin / 2);
	image_prob.resize(coule.width() + margin, coule.height() + margin, 1, 1);
	image_prob.fill(0);
	//    image_dest.blur(3.0); attention c'est pas pareil, voir util convol
	// generation du kernel
	//21 devrait être aussi en parametre

	kernel_prob kern(sx, sy);

	// processing
	cimg_forXY(coule,x,y)
		{
			float ratio = (float) y / (float) coule.height();
			//the kernel might be updated depending on the position on the gel :
			kern.update(parameter_prob.get_sx_with_gel_ratio_position(ratio),
					parameter_prob.get_sy_with_gel_ratio_position(ratio));
			if (coule(x, y) != _max_value) {
				if (coule(x, y) > threshold) {
					// printf(" %d %d %f\n", x, y, img1(x,y));
					int x0 = x - (kern._ncols - 1) / 2;
					int y0 = y - (kern._nrows - 1) / 2;
					for (int nc = 0; nc < kern._ncols; nc++) {
						for (int nr = 0; nr < kern._nrows; nr++) {
							image_prob(x0 + nc + half_margin, y0 + nr
									+ half_margin)
									+= (unsigned short int) (kern._kern[nc][nr]
											* coule(x, y));
						}
					}
				}
			} else {
				image_prob(x + half_margin, y + half_margin) = _max_value;
			}
		}
	image_prob.crop(half_margin, half_margin, coule.width() + half_margin,
			coule.height() + half_margin, false);
	cimg_for_borderXY(image_prob,x,y,1)
			image_prob(x, y) = 0;
}

void imageProb::mark_burned_pixels(imageCode & mark, int & cx, int & cy) const {
	//cerr << "imageProb::mark_burned_pixels begin" << endl;
	//cerr << "cx "<< cx << "cy " << cy << endl;
	int xmin(cx), xmax(cx), ymin(cy), ymax(cy);
	unsigned int depth(0);
	rec_mark_burned_pixels(mark, cx, cy, xmin, xmax, ymin, ymax, depth);

	cx = (xmin + xmax) / 2;
	cy = (ymin + ymax) / 2;
	//cerr << "imageProb::mark_burned_pixels end" << endl;
	//cerr << "cx "<< cx << "cy " << cy << endl;
}

void imageProb::rec_mark_burned_pixels(imageCode & mark, int x, int y,
		int & xmin, int & xmax, int & ymin, int & ymax, unsigned int & depth) const {
	//cerr << "imageProb::rec_mark_burned_pixels begin " << x << " " << y << endl;
	if ((x >= width()) || (y >= height()) || (x < 0) || (y < 0)) {
		return;
	}
	depth++;
	if (depth > 1000) {
		depth--;
		return;
	}
	const imageProb & image_prob = *this;
	if (mark(x, y) == 0) {
		if (image_prob(x, y) == imageProb::_max_value) {
			mark(x, y) = 1;
			if (x < xmin)
				xmin = x;
			if (y < ymin)
				ymin = y;
			if (x > xmax)
				xmax = x;
			if (y > ymax)
				ymax = y;

			rec_mark_burned_pixels(mark, x + 1, y, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x, y + 1, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x - 1, y, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x, y - 1, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x - 1, y + 1, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x + 1, y + 1, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x + 1, y - 1, xmin, xmax, ymin, ymax,
					depth);
			rec_mark_burned_pixels(mark, x - 1, y - 1, xmin, xmax, ymin, ymax,
					depth);
		}
	}
	depth--;
	return;
	//cerr << "imageProb::rec_mark_burned_pixels end" << endl;
}


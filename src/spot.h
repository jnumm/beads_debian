#ifndef SPOT_
#define SPOT_

#include <QString>
#include <deque>

using namespace std;
/** \brief object to handle spot properties
 * 
 * a spot on a 2-DE gel electrophoresis image has :
 * - a number
 * - coordinates (x, y)
 * - barycenter (tx, ty)
 * - volume, background, area
 */

class imageNumber;

class spot {
public:
	spot();
	spot(const spot & src);
	~spot();

	void set_picked_num(const QString & picked_num);
	const QString & get_picked_num() const {
		return (_picked_num);
	}

	void set_number(int number) {
		_number = number;
	}

	const int get_number() const {
		return (_number);
	}

	void set_vol(long int vol) {
		_vol = vol;
	}

	const int get_x() const {
		return ((int) _tx);
	}

	void set_x(int x) {
		_tx = x;
	}

	const int get_y() const {
		return ((int) _ty);
	}

	void set_y(int y) {
		_ty = y;
	}

	const float get_tx() const {
		return (_tx);
	}

	void set_tx(float x) {
		_tx = x;
	}

	const float get_ty() const {
		return (_ty);
	}

	const void set_ty(float y) {
		_ty = y;
	}

	const long int get_vol() const {
		return (_vol);
	}

	void set_area(unsigned long int surface) {
		_area = surface;
	}

	const unsigned long int get_area() const {
		return (_area);
	}

	void set_tmin(unsigned long int tmin) {
		_tmin = tmin;
	}

	const unsigned long int get_tmin() const {
		return (_tmin);
	}

	void set_tmax(unsigned long int tmax) {
		_tmax = tmax;
	}

	const unsigned long int get_tmax() const {
		return (_tmax);
	}

	void compute_background();
	const unsigned long int get_bckgnd() const {
		return (_bckgnd);
	}

	void add_edge_point(int x, int y);

	void sort_edge_points();
	//void look_for_edges(const imageNumber & image_numeros,int begin_x, int begin_y);

	unsigned int get_nb_edge_points() const {
		return (_edge_x.size());
	}

	int get_x_coord_of_edge_point(int i) const {
		return _edge_x[i];
	}
	int get_y_coord_of_edge_point(int i) const {
		return _edge_y[i];
	}

	void eliminate_aligned_edge_points();

	bool is_quantified() const {
		if (_area > 0) {
			return (true);
		}
		return (false);
	}

private:
	//void rec_look_for_edges(const imageNumber & image_numeros,int current_x, int current_y);
	bool is_in_edge(int x, int y);
	bool check_alignment(unsigned int i);

	int _number;
	QString _picked_num;

	long int _vol;
	unsigned long int _tmin;
	unsigned long int _tmax;
	unsigned long int _area;
	unsigned long int _bckgnd;
	float _tx;
	float _ty;

	deque<int> _edge_x;
	deque<int> _edge_y;

};

#endif /*SPOT_*/

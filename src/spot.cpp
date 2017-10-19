#include <iostream>
#include <list>
#include "spot.h"

#include "images/imageNumber.h"

using namespace std;

bool pixel_is_neighbour(int x1, int y1, int x2, int y2);
float distance_between_2_points(int x1, int y1, int x2, int y2);

float distance_between_2_points(int x1, int y1, int x2, int y2) {
	float result = (float) (x1 - x2) * (float) (x1 - x2);
	result += (float) (y1 - y2) * (float) (y1 - y2);

	result = sqrt(result);
	//cout << "spot::distance_between_2_points " << result << " x1 " << x1 << " y1 " << y1 << " x2 " << x2 << " y2 " << y2 << endl;

	return (result);
}

bool pixel_is_neighbour(int x1, int y1, int x2, int y2) {
	if ((abs(x1 - x2) <= 1) && (abs(y1 - y2) <= 1)) {
		return true;
	}
	return (false);
}

spot::spot() {
	_vol = 0;
	_tx = 0;
	_ty = 0;
	_tmin = 65535;
	_tmax = 0;
	_area = 0;
	_number = 0;
	_bckgnd = 0;

	_edge_x.resize(0);
	_edge_y.resize(0);
}

spot::spot(const spot & src) {
	_vol = src._vol;
	_tx = src._tx;
	_ty = src._ty;
	_tmin = src._tmin;
	_tmax = src._tmax;
	_area = src._area;
	_number = src._number;
	_bckgnd = src._bckgnd;

	_edge_x = src._edge_x;
	_edge_y = src._edge_y;

	_picked_num = src._picked_num;

}

spot::~spot() {
	_edge_x.resize(0);
	_edge_y.resize(0);
}

void spot::set_picked_num(const QString & picked_num) {
	_picked_num = picked_num;
}
void spot::compute_background() {
	_bckgnd = _area * _tmin;
}

bool spot::is_in_edge(int x, int y) {
	bool is_present = false;
	unsigned int i;
	for (i = 0; i < _edge_x.size(); i++) {
		if ((x == _edge_x[i]) && (y == _edge_y[i])) {
			is_present = true;
			break;
		}
	}
	return (is_present);
}

void spot::add_edge_point(int x, int y) {

	//cout << "spot::add_edge_coordinate x " << x << " y " << y << " on " << _number << endl;
	//check that this coordinate is not already inserted:
	if (is_in_edge(x, y))
		return;

	if (_edge_x.size() > 1) {
		float distance_front = distance_between_2_points(_edge_x.front(),
				_edge_y.front(), x, y);
		float distance_back = distance_between_2_points(_edge_x.back(),
				_edge_y.back(), x, y);
		if (distance_back < distance_front) {
			//cout << "push_back spot::add_edge_coordinate distance_front " << distance_front << " distance_back " << distance_back << endl;
			_edge_x.push_back(x);
			_edge_y.push_back(y);
		} else {
			//cout << "push_front spot::add_edge_coordinate distance_front " << distance_front << " distance_back " << distance_back << endl;
			_edge_x.push_front(x);
			_edge_y.push_front(y);
		}
	} else {
		_edge_x.push_back(x);
		_edge_y.push_back(y);
	}

	//cout << "end spot::add_edge_coordinate x " << x << " y " << y << " on " << _number << endl;
	//cin >> x;

}

void spot::sort_edge_points() {

	//cout << "spot::sort_edge_points " << endl;
	unsigned int size(_edge_x.size());
	if (size < 3) {
		return;
	}

	deque<int> new_x_left;
	deque<int> new_y_left;
	/*
	 unsigned int i;
	 list<int> new_x_right;
	 list<int> new_y_right;
	 list<int>::iterator ix_list, iy_list;

	 //new_x_left.push_back(_edge_x[0]);
	 //new_y_left.push_back(_edge_y[0]);
	 for (i = 0; i < size; i++) {
	 new_x_right.push_back(_edge_x[i]);
	 new_y_right.push_back(_edge_y[i]);
	 }

	 bool find=false;

	 while ((new_x_right.size() > 0) and (new_x_left.size() >= 0)) {
	 ix_list = new_x_right.begin();
	 iy_list = new_y_right.begin();

	 if (new_x_left.size() == 0) {
	 new_x_left.push_back(*ix_list);
	 new_y_left.push_back(*iy_list);
	 new_x_right.erase(ix_list);
	 new_y_right.erase(iy_list);
	 ix_list++;
	 iy_list++;
	 } else {
	 find = false;

	 while ((find == false) and (ix_list != new_x_right.end())) {
	 if (pixel_is_neighbour(new_x_left.back(), new_y_left.back(),
	 *ix_list, *iy_list)) {
	 find = true;
	 new_x_left.push_back(*ix_list);
	 new_y_left.push_back(*iy_list);
	 new_x_right.erase(ix_list);
	 new_y_right.erase(iy_list);
	 }
	 ix_list++;
	 iy_list++;
	 }

	 if (find == false) {
	 new_x_left.pop_front();
	 new_y_left.pop_front();
	 }

	 }
	 }

	 _edge_x.resize(new_x_left.size());
	 _edge_y.resize(new_y_left.size());
	 for (i = 0; i < new_x_left.size(); i++) {
	 _edge_x[i] = new_x_left[i];
	 _edge_y[i] = new_y_left[i];
	 }
	 */

	unsigned int i, finish;
	int min_ipos(0);
	float distance, d_tmp;
	deque<int> new_x;
	deque<int> new_y;

	new_x.push_back(_edge_x[0]);
	new_y.push_back(_edge_y[0]);
	_edge_x[0] = -1;

	finish = size;
	while (finish > 0) {
		distance = 5000;
		for (i = 1; i < size; i++) {
			if (_edge_x[i] < 0)
				continue;
			d_tmp = distance_between_2_points(_edge_x[i], _edge_y[i],
					new_x.back(), new_y.back());
			if (d_tmp < distance) {
				distance = d_tmp;
				min_ipos = i;
			}
		}
		if (distance < 10) {
			new_x.push_back(_edge_x[min_ipos]);
			new_y.push_back(_edge_y[min_ipos]);
		}
		finish--;
		_edge_x[min_ipos] = -1;
	}
	_edge_x = new_x;
	_edge_y = new_y;

	//cout << "end spot::sort_edge_points " << endl;
}

void spot::eliminate_aligned_edge_points() {

	if (_edge_x.size() < 3) {
		return;
	}
	unsigned int i;
	deque<int> new_x;
	deque<int> new_y;

	new_x.push_back(_edge_x[0]);
	new_y.push_back(_edge_y[0]);

	for (i = 2; i < _edge_x.size(); i++) {
		if (check_alignment(i)) {
		} else {
			new_x.push_back(_edge_x[i - 1]);
			new_y.push_back(_edge_y[i - 1]);
		}
	}
	new_x.push_back(_edge_x[_edge_x.size() - 1]);
	new_y.push_back(_edge_y[_edge_y.size() - 1]);

	_edge_x = new_x;
	_edge_y = new_y;

}

/** \brief check alignment of the last 3 edges points
 *
 * if spots are aligned
 */
bool spot::check_alignment(unsigned int pos) {
	bool remove = false;
	if (pos < 3) {
		return (remove);
	}
	int x1 = _edge_x[pos - 2];
	int y1 = _edge_y[pos - 2];
	int x2 = _edge_x[pos - 1];
	int y2 = _edge_y[pos - 1];
	int x3 = _edge_x[pos];
	int y3 = _edge_y[pos];

	if ((x1 == x2) && (x2 == x3)) {
		//aligned
		if ((y1 < y2) && (y2 < y3)) {
			//			cout << "t1" << endl;
			remove = true;
		}
		if ((y3 < y2) && (y2 < y1)) {
			//			cout << "t2" << endl;
			remove = true;
		}
	} else if ((y1 == y2) && (y2 == y3)) {
		//aligned
		if ((x1 < x2) && (x2 < x3)) {
			//			cout << "t3" << endl;
			remove = true;
		}
		if ((x3 < x2) && (x2 < x1)) {
			//			cout << "t4" << endl;
			remove = true;
		}
	} else if ((x1 - x2) == 0) {
	} else if ((y1 - y2) == 0) {
	} else {

		float ratio_x = ((float) (x1 - x3)) / ((float) (x1 - x2));
		float ratio_y = ((float) (y1 - y3)) / ((float) (y1 - y2));

		if (ratio_x != ratio_y) {
			//not aligned
		} else if (ratio_x < 1) {
			//aligned but going back => keep the second point
		} else {
			//aligned and going forward => no need to keep second point:
			remove = true;
			//			cout << "spot::check_alignment remove ratio_x " <<  ratio_x << " ratio_y " << ratio_y << " in " << _number << endl;
		}
	}
	//	if (remove) {
	//		cout << "spot::check_alignment remove x " <<  x2 << " y " << y2 << " in " << _number << endl;
	//		cout << x1 << " " << x2 << " " << x3 << endl;
	//		cout << y1 << " " << y2 << " " << y3 << endl;
	//
	//	}
	return (remove);

}

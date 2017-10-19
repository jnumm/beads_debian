#ifndef PARAMETERCONFLUENT_H_
#define PARAMETERCONFLUENT_H_

/** \brief handle parameters for imageConfluent
 *
 */

class parameterConfluent {
public:
	parameterConfluent();

	~parameterConfluent() {
	}

	void set_minflux(int flux) {
		_minflux = flux;
	}

	void set_minpath(int flux) {
		_minpath = flux;
	}

	void set_winconfl(int flux) {
		_winconfl = flux;
	}

	void set_minbeads(int flux) {
		_minbeads = flux;
	}

	void set_intmax(int flux) {
		_intmax = flux;
	}

	void set_minpct(int flux) {
		_minpct = flux;
	}

	const int get_minflux() const {
		return (_minflux);
	}

	const int get_minpath() const {
		return (_minpath);
	}

	const int get_winconfl() const {
		return (_winconfl);
	}

	const int get_minbeads() const {
		return (_minbeads);
	}

	const int get_intmax() const {
		return (_intmax);
	}

	const int get_minpct() const {
		return (_minpct);
	}

private:
	int _minflux;
	int _minpath;
	int _winconfl;
	int _minbeads;
	int _intmax;
	int _minpct;
};

#endif /*PARAMETERCONFLUENT_H_*/

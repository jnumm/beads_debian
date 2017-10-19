#ifndef PARAMETERNUMBER_H_
#define PARAMETERNUMBER_H_

/** \brief handle parameters for imageNumber
 * quantification parameters
 */

class parameterNumber {
public:
	parameterNumber();

	~parameterNumber() {
	}

	void set_enlarge(int flux) {
		_enlarge = flux;
	}

	void set_enlarge_fusion(int flux) {
		_enlarge_fusion = flux;
	}

	void set_npass(int flux) {
		_npass = flux;
	}

	const int get_enlarge() const {
		return (_enlarge);
	}

	const int get_enlarge_fusion() const {
		return (_enlarge_fusion);
	}

	const int get_npass() const {
		return (_npass);
	}

private:
	int _enlarge;
	int _enlarge_fusion;
	int _npass;
};

#endif /*PARAMETERNUMBER_H_*/

#ifndef PARAMETERPROB_H_
#define PARAMETERPROB_H_

/** \brief handle parameters for imageProb
 *
 */

class parameterProb {
public:
	parameterProb();
	~parameterProb() {
	}

	void set_threshold(float flux) {
		_threshold = flux;
	}

	void set_sx(float flux) {
		_sx = flux;
	}

	void set_sy(float flux) {
		_sy = flux;
	}

	void set_sx_bottom(float flux) {
		_sx_bottom = flux;
	}

	void set_sy_bottom(float flux) {
		_sy_bottom = flux;
	}

	const float get_threshold() const {
		return (_threshold);
	}

	const float get_sx() const {
		return (_sx);
	}

	const float get_sy() const {
		return (_sy);
	}

	const float get_sx_bottom() const {
		return (_sx_bottom);
	}

	const float get_sy_bottom() const {
		return (_sy_bottom);
	}

	const float get_sx_with_gel_ratio_position(float ratio_position) const;
	const float get_sy_with_gel_ratio_position(float ratio_position) const;

private:
	float _threshold;
	float _sx;
	float _sy;

	float _sx_bottom;
	float _sy_bottom;
};

#endif /*PARAMETERPROB_H_*/

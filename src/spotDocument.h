#ifndef SPOTDOCUMENT_H_
#define SPOTDOCUMENT_H_

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "detection.h"
#include "spot.h"
#include "config.h"

class spotDocument {
public:
	spotDocument();

	~spotDocument();

	void set_gel_image_file_name(const QString & filename) {
		_gel_image_file_name = filename;
	}
	const QString & get_gel_image_file_name() const {
		return (_gel_image_file_name);
	}

	virtual void open(const QString & filename);
	virtual void close();

	virtual void write_detection(detection & the_detection);

	virtual void write_spot(const spot &);

protected:

	QString _gel_image_file_name;

	QFile * _output_file;
	QTextStream * _output_stream;
};

#endif /*SPOTDOCUMENT_H_*/

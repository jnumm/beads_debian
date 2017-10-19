#include "spotDocument.h"
#include <iostream>
#include <QFileInfo>

spotDocument::spotDocument() {
	_output_file = NULL;
	_output_stream = NULL;
}

spotDocument::~spotDocument() {
	if (_output_stream != NULL) {
		delete (_output_stream);
	}
	if (_output_file != NULL) {
		delete (_output_file);
	}
}

void spotDocument::open(const QString & filename) {
	QString output_filename(filename);
	QFileInfo fileinfo(filename);
	QString ext = fileinfo.suffix();
	if (ext != "txt") {
		output_filename = output_filename + ".txt";
	}

	_output_file = new QFile(output_filename);

	if (_output_file->exists()) {
		cerr << "WARNING : " << _output_file->fileName().toStdString()
				<< " output file already exists" << endl;
	}
	if (_output_file->open(QIODevice::WriteOnly)) {
		_output_stream = new QTextStream(_output_file);
		_output_stream ->setLocale(QLocale::system());
	} else {
		//throw exception
	}

	*_output_stream << "Spot Volume x y min surface background vol-bckgnd\n";

}

void spotDocument::write_spot(const spot & thespot) {
	//std::ostringstream tag;
	*_output_stream << thespot.get_number() << " ";
	*_output_stream << thespot.get_vol() << " ";
	*_output_stream << thespot.get_tx() << " ";
	*_output_stream << thespot.get_ty() << " ";
	*_output_stream << thespot.get_tmin() << " ";
	*_output_stream << thespot.get_area() << " ";
	*_output_stream << thespot.get_bckgnd() << " ";
	*_output_stream << (thespot.get_vol() - thespot.get_bckgnd()) << "\n";

	//return (tag.str().c_str());
}

void spotDocument::close() {
	qDebug() << "spotDocument::close begin";
	if (_output_stream != NULL) {
		_output_stream->flush();
		delete (_output_stream);
	}
	if (_output_file != NULL) {
		_output_file->close();
		delete (_output_file);
	}
	_output_file = NULL;
	_output_stream = NULL;
	qDebug() << "spotDocument::close end";
}

void spotDocument::write_detection(detection & the_detection) {

	detection::iterator it;
	for (it = the_detection.begin(); it != the_detection.end(); ++it) {
		spot & the_spot = (*it).second;
		write_spot(the_spot);
	}
}


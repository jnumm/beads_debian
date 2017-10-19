/*
 * properties.cpp
 *
 *  Created on: 27 nov. 2009
 *      Author: olivier
 */

#include "properties.h"
#include "qtbeads_error.h"
#include <QDebug>
#include <string>
#include <iostream>
#include <fstream>

void create_default_parameter_file(const string filename) {
	ofstream param_stream;
	param_stream.open(filename.c_str());
	param_stream << "#" << filename << endl;
	param_stream << "# beads configuration file" << endl;
	param_stream << "# default parameters, optimized for silver staining"
			<< endl;
	param_stream << endl;
	param_stream << "################################" << endl;
	param_stream << "# parameters for DIRECTIONS    #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "burned_pixel_threshold = 63000" << endl;
	param_stream << endl;
	param_stream << "################################" << endl;
	param_stream << "# parameters for SELECT        #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "minflux = 100" << endl;
	param_stream << "minpath = 2" << endl;
	param_stream << "winconfl = 300" << endl;
	param_stream << "minbeads = 200" << endl;
	param_stream << "confluent_intmax = 60000" << endl;
	param_stream << "confluent_minpct = 1" << endl;
	param_stream << endl;
	param_stream << "################################" << endl;
	param_stream << "# parameters for PROBABILITIES #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "prob_threshold = 10" << endl;
	param_stream << "sx = 3" << endl;
	param_stream << "sy = 3" << endl;
	param_stream << "sx_bottom = 5" << endl;
	param_stream << "sy_bottom = 5" << endl;
	param_stream << endl;

	param_stream << "################################" << endl;
	param_stream << "# parameters for detections #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "minproba  = 400" << endl;
	param_stream << endl;

	param_stream << "################################" << endl;
	param_stream << "# parameters for quantification #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "quantification_enlarge_fusion = 3" << endl;
	param_stream << "npass = 10" << endl;
	param_stream << endl;
	param_stream.close();
}

Properties::Properties() {
	_p_param_file_info = NULL;
	_p_original_gel_image_file_info = NULL;
	_p_working_gel_image = NULL;
	_inversed = false;

	//create default parameter file :
	QFileInfo * newfile = new QFileInfo("beads.conf");
	if (newfile->exists()) {
	} else {
		create_default_parameter_file(newfile->filePath().toStdString());
	}
	if (newfile->exists()) {
	} else {
		throw qtbeadsError(
				QObject::tr(
						"unable to create default parameter file for beads in ").append(
						newfile->filePath()));
	}
	_p_param_file_info = newfile;

}

Properties::Properties(const Properties & src) {
	_p_param_file_info = NULL;
	_p_original_gel_image_file_info = NULL;

	qDebug() << "Properties::Properties(const Properties & src) begin ";
	if (_p_param_file_info != NULL) {
		qDebug()
				<< "Properties::Properties(const Properties & src) delete (_p_param_file_info) "
				<< _p_param_file_info;
		delete (_p_param_file_info);
	}
	if (_p_original_gel_image_file_info != NULL) {
		qDebug()
				<< "Properties::Properties(const Properties & src) delete (_p_original_gel_image_file_info) "
				<< _p_original_gel_image_file_info;
		delete (_p_original_gel_image_file_info);
	}
	_p_param_file_info = new QFileInfo(src.getParamFileInfo());
	_p_original_gel_image_file_info = new QFileInfo(src.getGelImageFileInfo());
	_p_working_gel_image = new imageIntensity(*(src.get_p_working_image()));
	_inversed = src.isInverse();

	qDebug() << "Properties::Properties(const Properties & src) end ";

}

Properties::~Properties() {
	if (_p_param_file_info != NULL) {
		delete (_p_param_file_info);
	}
	if (_p_original_gel_image_file_info != NULL) {
		delete (_p_original_gel_image_file_info);
	}
	_p_param_file_info = NULL;
	_p_original_gel_image_file_info = NULL;
	if (_p_working_gel_image == NULL) {
		delete _p_working_gel_image;
	}
}

const imageIntensity * Properties::get_p_working_image() const {

	if (_p_working_gel_image == NULL) {
		throw qtbeadsError(QObject::tr(
						"no gel image loaded... please open a gel image first"));
	}
	return _p_working_gel_image;

}

const QFileInfo & Properties::getGelImageFileInfo() const {
	qDebug() << "QFileInfo & Properties::getGelImageFileInfo() begin";
	if (_p_original_gel_image_file_info == NULL) {
		throw qtbeadsError(QObject::tr(
				"no gel image loaded... please open a gel image first"));
	}
	qDebug() << "QFileInfo & Properties::getGelImageFileInfo() end";
	return (*_p_original_gel_image_file_info);
}

const QFileInfo & Properties::getParamFileInfo() const {
	if (_p_param_file_info == NULL) {
		throw qtbeadsError(QObject::tr(
				"no ParamFileInfo... please set the parameter file first"));
		/*
		 //create default parameter file :
		 QFileInfo * newfile = new QFileInfo("beads.conf");
		 _p_param_file_info = newfile;
		 if (newfile->exists()) {
		 } else {
		 create_default_parameter_file(newfile->filePath().toStdString());
		 }
		 */
	}
	return (*_p_param_file_info);

}

void Properties::setGelImageFilename(const QString & filename) {
	QFileInfo * newfile = new QFileInfo(filename);
	if (newfile->exists()) {
		if (_p_original_gel_image_file_info != NULL) {
			delete _p_original_gel_image_file_info;
			/*
			throw qtbeadsError(
					QObject::tr(
							"A gel image file is already associated to these properties : ").append(
							getGelImageFileInfo().filePath()));
						*/
		}
		_p_original_gel_image_file_info = newfile;
		if (_p_working_gel_image != NULL) {
			delete _p_working_gel_image;
		}
		imageIntensity * p_gel_image_loaded = new imageIntensity(
				getGelImageFileInfo().filePath());
		if (isInverse()) {
			qDebug() << "Properties::setGelImageFilename inverse " << isInverse() ;
			//gel_image.image_inverse();
			_p_working_gel_image = p_gel_image_loaded->new_image_inversed();
			delete (p_gel_image_loaded);
		} else {
			qDebug() << "Properties::setGelImageFilename not inverse " << isInverse() ;
			_p_working_gel_image = p_gel_image_loaded;
		}

	} else {
		throw qtbeadsError(QObject::tr(
				"This gel image filename does not exists : ").append(filename));
	}
}

void Properties::setParamFilename(const QString & filename) {
	QFileInfo * newfile = new QFileInfo(filename);
	if (newfile->exists()) {
		_p_param_file_info = newfile;
	}
}

void Properties::setInverse(bool ok) {
	_inversed = ok;

	qDebug() << "Properties::setInverse " << ok ;
	if (_p_working_gel_image != NULL) {
		qDebug() << "Properties::setInverse _p_working_gel_image != NULL"  ;
		if (isInverse() != _p_working_gel_image->get_is_inversed()) {
			imageIntensity * new_working_image =
					_p_working_gel_image->new_image_inversed();
			delete (_p_working_gel_image);
			_p_working_gel_image = new_working_image;
		}
		qDebug() << "Properties::setInverse _p_working_gel_image->inv " << _p_working_gel_image->get_is_inversed() ;
	}
	qDebug() << "Properties::setInverse end " ;

}


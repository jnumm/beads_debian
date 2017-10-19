/*
 * beads_results.cpp
 *
 *  Created on: 29 nov. 2009
 *      Author: olivier
 */

#include "../spotPROTICdbDocument.h"
#include "../spotSvgDocument.h"
#include "../spot_document_gnumeric.h"

#include "beads_results.h"
#include "../images/imageContours.h"

BeadsResults::BeadsResults(const Properties & initial_properties) :
	_properties(initial_properties) {

}

BeadsResults::~BeadsResults() {
}

void BeadsResults::saveImageContour(const QFileInfo & filename) const {
	imageContours contours;
	contours.compute_from_gel_image_and_numeros(
			*_properties.get_p_working_image(), *_p_number_image);
	contours.draw_contours(*_p_detection, _properties.isInverse());
	const imageIntensity * image_inversed = contours.new_image_inversed();

	image_inversed->save(filename.filePath());
	//QCimg image(*image_inversed);
	delete (image_inversed);
}

void BeadsResults::saveProticDbMl(const QFileInfo & filename) const {
	qDebug() << "BeadsResults::saveProticDbMl begin";
	detection detection(*_p_detection);

	qDebug()
			<< "BeadsResults::saveProticDbMl   detection.quantify_spots_with_image begin";
	detection.quantify_spots_with_image(*_properties.get_p_working_image(),
			*_p_number_image);
	qDebug()
			<< "BeadsResults::saveProticDbMl   detection.quantify_spots_with_image end";

	spotPROTICdbDocument proticdbml_file;
	proticdbml_file.set_gel_image_file_name(
			_properties.getGelImageFileInfo().filePath());
	proticdbml_file.open(filename.filePath());
	proticdbml_file.write_detection(detection);
	proticdbml_file.close();
	qDebug() << "BeadsResults::saveProticDbMl end";

}

void BeadsResults::saveText(const QFileInfo & filename) const {
	detection detection(*_p_detection);

	detection.quantify_spots_with_image(*_properties.get_p_working_image(),
			*_p_number_image);

	spotDocument fspot;
	fspot.open(filename.filePath());
	fspot.write_detection(detection);
	fspot.close();
}

void BeadsResults::saveGnumeric(const QFileInfo & filename) const {
	detection detection(*_p_detection);

	detection.quantify_spots_with_image(*_properties.get_p_working_image(), *_p_number_image);

	SpotDocumentGnumeric fspot;
	fspot.open(filename.filePath());
	fspot.write_detection(detection);
	fspot.close();
}

void BeadsResults::saveSvg(const QFileInfo & filename) const {
	qDebug() << "BeadsResults::saveSvg(const QFileInfo & filename) begin";
	detection detection(*_p_detection);
	detection.store_spot_edges(*_p_number_image);
	spotSvgDocument svg_file;
	svg_file.set_gel_image_file_name(
			_properties.getGelImageFileInfo().filePath());
	svg_file.set_css_class("detection", "fill:blue;fill-opacity:1;stroke:none");
	svg_file.set_css_class("contour",
			"stroke-linejoin:round;stroke:#000000;stroke-opacity:1;fill:none");

	qDebug() << "BeadsResults::saveSvg(const QFileInfo & filename) embed";
	svg_file.embed_gel_image(_properties.getGelImageFileInfo().filePath());
	//svg_file.embed_gel_image();

	svg_file.open(filename.filePath());
	svg_file.write_detection(detection, "detection");
	svg_file.draw_spot_numbers(detection, "detection");

	qDebug() << "BeadsResults::saveSvg(const QFileInfo & filename) quantify";
	detection.quantify_spots_with_image(*_properties.get_p_working_image(), *_p_number_image);
	svg_file.write_detection(detection, "quantif");
	svg_file.draw_contours(detection, "contour");

	qDebug() << "BeadsResults::saveSvg(const QFileInfo & filename) close";
	svg_file.close();
	qDebug() << "BeadsResults::saveSvg(const QFileInfo & filename) end";
}

int BeadsResults::getSpotNumber(int x, int y) const {
	const imageNumber & image_numeros = *_p_number_image;
	if (image_numeros(x, y) > 0)
		return image_numeros(x, y);
	return (-1);
}

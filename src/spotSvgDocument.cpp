#include "spotSvgDocument.h"
using namespace std;
#include <QFileInfo>
#include <iostream>
#include <fstream>
#include "encode/base64.h"

spotSvgDocument::spotSvgDocument() {
	_css_classes["quantif"] = "fill:#ff0000;fill-opacity:1;stroke:none";
	_css_classes["contour"] = "stroke:#000000;stroke-opacity:1;fill:none";

	_embed_gel_image = false;
	_gel_image_writed = false;
}

void spotSvgDocument::set_css_class(const QString & css_class_name,
		const QString & css_style) {
	_css_classes[css_class_name] = css_style;
}

void spotSvgDocument::open(const QString & filename) {
	QString output_filename(filename);
	QFileInfo fileinfo(filename);
	QString ext = fileinfo.suffix();
	if (ext != "svg") {
		output_filename = output_filename + ".svg";
	}

	_output_file = new QFile(output_filename);
	_output_stream = NULL;

	if (_output_file->exists()) {
		cerr << "WARNING : " << _output_file->fileName().toStdString()
				<< " svg file already exists" << endl;
	}
	if (_output_file->open(QIODevice::WriteOnly)) {

		_p_xml_stream = new QXmlStreamWriter(_output_file);
	} else {
		//throw exception
	}

	/*
	 * <?xml version="1.0" standalone="no"?>
	 <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
	 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">

	 <svg width="100%" height="100%" version="1.1"
	 xmlns="http://www.w3.org/2000/svg">

	 <circle cx="100" cy="50" r="40" stroke="black"
	 stroke-width="2" fill="red"/>

	 </svg>
	 *
	 */
	_p_xml_stream->setAutoFormatting(true);
	_p_xml_stream->writeStartDocument();
	/*
	 * <svg version="1.1"
	 baseProfile="full"
	 xmlns="http://www.w3.org/2000/svg"
	 xmlns:xlink="http://www.w3.org/1999/xlink"
	 xmlns:ev="http://www.w3.org/2001/xml-events">
	 *
	 */
	_p_xml_stream->writeStartElement("svg");
	_p_xml_stream->writeAttribute("baseProfile", "full");
	_p_xml_stream->writeDefaultNamespace("http://www.w3.org/2000/svg");
	_p_xml_stream->writeNamespace("http://www.w3.org/1999/xlink", "xlink");
	_p_xml_stream->writeNamespace("http://www.w3.org/2001/xml-events", "ev");
	//*this << "<svg ";
	_p_xml_stream->writeAttribute("width", "100%");
	//*this << " width=\"100%\"";
	_p_xml_stream->writeAttribute("height", "100%");
	//*this << " height=\"100%\"";
	_p_xml_stream->writeAttribute("version", "1.1");
	//*this << " version=\"1.1\"";

	//définition des styles (syntaxe CSS):
	_p_xml_stream->writeStartElement("defs");
	//*this << "<defs>" << std::endl;
	_p_xml_stream->writeStartElement("style");
	_p_xml_stream->writeAttribute("type", "text/css");

	QString style_css;
	//*this << "\t<style type=\"text/css\"><![CDATA[" << std::endl;
	std::map<QString, QString>::iterator it;
	for (it = _css_classes.begin(); it != _css_classes.end(); ++it) {
		style_css += "\t\t." + (*it).first + " {" + (*it).second + " }\n";
		//*this << "\t\t." << (*it).first << " {" << (*it).second << " }"
		//		<< std::endl;
	}
	_p_xml_stream->writeCDATA(style_css);
	_p_xml_stream->writeEndElement();

	//*this << "]]></style>" << std::endl;
	_p_xml_stream->writeEndElement();
	//*this << "</defs>" << std::endl;

	_p_xml_stream->writeStartElement("g");
	//*this << "\t<g>";

	/*
	 *this << "\t\t<g transform=\"translate(+1,+1)\">";
	 *this << "\t\t<image y=\"102.40313\" x=\"75.97953\"";
	 *this << " height=\"564\"";
	 *this << " width=\"516\"";
	 *this << " xlink:href=\"" << get_gel_image_file_name() << "\" />\n";
	 */

}

void spotSvgDocument::close() {
	qDebug() << "spotSvgDocument::close begin";
	//*this << "\t\t</g>" << endl;
	_p_xml_stream->writeEndElement();
	//*this << "\t</g>" << endl;
	_p_xml_stream->writeEndDocument();
	//*this << "</svg>" << std::endl;
	//std::ofstream::close();

	if (_p_xml_stream != NULL) {
		delete (_p_xml_stream);
	}
	if (_output_file != NULL) {
		_output_file->close();
		delete (_output_file);
	}
	if (_output_stream != NULL) {
		delete (_output_stream);
	}
	_output_file = NULL;
	_output_stream = NULL;
	_p_xml_stream = NULL;
	qDebug() << "spotSvgDocument::close end";

}

void spotSvgDocument::write_spot_number(const spot & thespot,
		const QString & cssclass) {

	_p_xml_stream->writeStartElement("text");
	//*this << "\t\t<text";
	_p_xml_stream->writeAttribute("class", cssclass);
	//*this << " class=\"" << cssclass << "\"";
	_p_xml_stream->writeAttribute("x", QString::number(thespot.get_x()));
	//*this << " x=\"" << thespot.get_x() << "\"";
	_p_xml_stream->writeAttribute("y", QString::number(thespot.get_y()));
	//*this << " y=\"" << thespot.get_y() << "\"";
	_p_xml_stream->writeAttribute("dx", "-10");
	//*this << " dx=\"-10\"";
	_p_xml_stream->writeAttribute("dy", "-10");
	//*this << " dy=\"-10\"";
	_p_xml_stream->writeCharacters(QString::number(thespot.get_number()));
	_p_xml_stream->writeEndElement();
	//*this << ">" << thespot.get_number() << "</text>" << std::endl;
}

void spotSvgDocument::write_spot(const spot & thespot, const QString & cssclass) {
	//style = "fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:1pt;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1";
	/*
	 <circle cx="100" cy="50" r="40" stroke="black"
	 stroke-width="2" fill="red"/>
	 */
	//std::ostringstream tag;
	//*this << "<point x=\"" << thespot.get_x() << "\" y=\"" << thespot.get_y() << "\"></rect>\n";

	_p_xml_stream->writeStartElement("circle");
	//*this << "\t\t<circle";
	_p_xml_stream->writeAttribute("class", cssclass);
	//*this << " class=\"" << cssclass << "\"";
	_p_xml_stream->writeAttribute("cx", QString::number(thespot.get_x()));
	//*this << " cx=\"" << thespot.get_x() << "\"";
	_p_xml_stream->writeAttribute("cy", QString::number(thespot.get_y()));
	//*this << " cy=\"" << thespot.get_y() << "\"";
	_p_xml_stream->writeAttribute("r", "1");
	//*this << " r=\"1\"";
	_p_xml_stream->writeEndElement();
	//*this << "/>" << std::endl;

	//return (tag.str().c_str());
}

void spotSvgDocument::write_spot_contours(const spot & thespot,
		const QString & cssclass) {

	unsigned int nb_edge_points = thespot.get_nb_edge_points();

	if (nb_edge_points == 0)
		return;
	/*
	 * <path
	 style="fill:none;fill-opacity:0.75;fill-rule:evenodd;stroke:#000000;stroke-width:0.25pt;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1"
	 d="M 154.28571,383.79075 L 297.14286,298.07647 L 437.14286,415.21933 L 294.28571,518.07647 L 154.28571,383.79075 z "
	 id="path2160" />
	 */
	_p_xml_stream->writeComment("spot " + QString::number(thespot.get_number()));
	//*this << "<!-- spot " << thespot.get_number() << "-->";
	_p_xml_stream->writeStartElement("path");
	//*this << "\t\t<path";
	_p_xml_stream->writeAttribute("class", cssclass);
	//*this << " class=\"" << cssclass << "\"" << std::endl;

	QString dpath = "M ";
	//*this << " d=\"M ";

	unsigned int i;
	for (i = 0; i < nb_edge_points; i++) {
		if (i > 0) {
			dpath += " L ";
			//*this << " L ";
		}
		dpath += QString::number(thespot.get_x_coord_of_edge_point(i)) + ","
				+ QString::number(thespot.get_y_coord_of_edge_point(i));
		//*this << thespot.get_x_coord_of_edge_point(i) << ","
		//		<< thespot.get_y_coord_of_edge_point(i);
	}
	dpath += " L " + QString::number(thespot.get_x_coord_of_edge_point(0))
			+ "," + QString::number(thespot.get_y_coord_of_edge_point(0))
			+ " z ";
	//*this << " L " << thespot.get_x_coord_of_edge_point(0) << ","
	//		<< thespot.get_y_coord_of_edge_point(0) << " z ";

	_p_xml_stream->writeAttribute("d", dpath);

	_p_xml_stream->writeEndElement();
	//*this << "\"" << std::endl;
	//*this << "/>" << std::endl;

	//return (tag.str().c_str());
}

void spotSvgDocument::write_gel_image() {

	if (this->_embed_gel_image) {
		//embed the gel image file using base64 encoding
		_p_xml_stream->writeStartElement("image");
		//*this << "\t\t<image";
		_p_xml_stream->writeAttribute("x", "0");
		//*this << " x=\"0\"";
		_p_xml_stream->writeAttribute("y", "0");
		//*this << " y=\"0\"";
		_p_xml_stream->writeAttribute("height", QString::number(
				this->_gel_image_height));
		//*this << " height=\"" << this->_gel_image_height << "\"";
		_p_xml_stream->writeAttribute("width", QString::number(
				this->_gel_image_width));
		//*this << " width=\"" << this->_gel_image_width << "\"";
		_p_xml_stream->writeAttribute("http://www.w3.org/1999/xlink", "href",
				"data:image/jpg;base64,\n" + this->_encoded_gel_image + "\n");
		//*this << " xlink:href=\"data:image/jpg;base64," << endl
		//		<< this->_encoded_gel_image << endl << "\"";
		_p_xml_stream->writeEndElement();
		//*this << "/>" << std::endl;
	} else {
		//the gel image file is only a reference on the gel image (URL)

		_p_xml_stream->writeStartElement("image");
		//*this << "\t\t<image";
		_p_xml_stream->writeAttribute("x", "0");
		//*this << " x=\"0\"";
		_p_xml_stream->writeAttribute("y", "0");
		//*this << " y=\"0\"";
		_p_xml_stream->writeAttribute("height", QString::number(
				this->_gel_image_height));
		//*this << " height=\"" << _gel_image_height << "\"";
		_p_xml_stream->writeAttribute("width", QString::number(
				this->_gel_image_width));
		//*this << " width=\"" << _gel_image_width << "\"";
		_p_xml_stream->writeAttribute("http://www.w3.org/1999/xlink", "href",
				"./" + get_gel_image_file_name());
		//*this << " xlink:href=\"./" << get_gel_image_file_name() << "\"";
		_p_xml_stream->writeEndElement();
		//*this << "/>" << std::endl;
	}
}

void spotSvgDocument::write_detection(const detection & the_detection,
		const QString & cssclass) {

	_gel_image_height = the_detection.get_gel_image_height();
	_gel_image_width = the_detection.get_gel_image_width();

	if (_gel_image_writed == false) {
		this->write_gel_image();
		_gel_image_writed = true;
	}
	//écrit tous les spots dans un group
	_p_xml_stream->writeStartElement("g");
	//*this << "\t\t<g>";
	detection::const_iterator it;
	for (it = the_detection.begin(); it != the_detection.end(); ++it) {
		const spot & the_spot = (*it).second;
		write_spot(the_spot, cssclass);
	}
	_p_xml_stream->writeEndElement();
	//*this << "\t\t</g>";

}

void spotSvgDocument::draw_contours(const detection & the_detection,
		const QString & cssclass) {
	_gel_image_height = the_detection.get_gel_image_height();
	_gel_image_width = the_detection.get_gel_image_width();
	if (_gel_image_writed == false) {
		this->write_gel_image();
		_gel_image_writed = true;
	}
	//dessine les bords des spots en SVG
	_p_xml_stream->writeStartElement("g");
	//*this << "\t\t<g>";
	detection::const_iterator it;
	for (it = the_detection.begin(); it != the_detection.end(); ++it) {
		const spot & the_spot = (*it).second;
		write_spot_contours(the_spot, cssclass);
	}
	_p_xml_stream->writeEndElement();
	//*this << "\t\t</g>";
}

void spotSvgDocument::draw_spot_numbers(detection & my_detection,
		const QString & cssclass) {
	_gel_image_height = my_detection.get_gel_image_height();
	_gel_image_width = my_detection.get_gel_image_width();
	if (_gel_image_writed == false) {
		this->write_gel_image();
		_gel_image_writed = true;
	}
	//dessine les étiquettes des spots en SVG
	_p_xml_stream->writeStartElement("g");
	//*this << "\t\t<g>";
	detection::iterator it;
	for (it = my_detection.begin(); it != my_detection.end(); ++it) {
		spot & the_spot = (*it).second;
		write_spot_number(the_spot, cssclass);
	}
	_p_xml_stream->writeEndElement();
	//*this << "\t\t</g>";
}

void spotSvgDocument::embed_gel_image(const QString & gel_image_file_name) {
	const imageIntensity gel_image(gel_image_file_name);
	embed_gel_image(gel_image);
}

void spotSvgDocument::embed_gel_image(const imageIntensity & gel_image) {
	qDebug() << "spotSvgDocument::embed_gel_image begin";
	//gel_image.save("tmp.tiff");
	gel_image.save("tmp.jpg");

	this->_gel_image_width = gel_image.width();
	this->_gel_image_height = gel_image.height();

	ifstream file("tmp.jpg", ios::binary | ios::ate);
	// Since we are at the end, we can get the size
	//file.seekg(0,ifstream::end);
	long size = file.tellg();
	//cout << size;
	file.seekg(0, ios::beg); //Seek back to the beginning
	char* unsorted;
	//Size input and read the file into it
	unsorted = new char[size];
	file.read(unsorted, size);
	file.close();
	unlink("tmp.jpg");
	// Change size so it is the number of floats instead of
	// the number of characters
	//size = size / 4;

	//const std::string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung" ;
	qDebug() << "spotSvgDocument::embed_gel_image base64_encode";
	this->_encoded_gel_image = base64_encode((const unsigned char *) unsorted,
			size).c_str();

	this->_embed_gel_image = true;
	qDebug() << "spotSvgDocument::embed_gel_image end";

}


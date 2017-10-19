#include <iostream>
#include <QFileInfo>

#include "spotPROTICdbDocument.h"

void spotPROTICdbDocument::open(const QString & filename) {
	QString output_filename(filename);
	QFileInfo fileinfo(filename);
	QString ext = fileinfo.suffix();
	if (ext != "xml") {
		output_filename = output_filename + ".xml";
	}

	_output_file = new QFile(output_filename);
	_output_stream = NULL;

	if (_output_file->exists()) {
		cerr << "WARNING : " << _output_file->fileName().toStdString()
				<< " xml output file already exists" << endl;
	}
	if (_output_file->open(QIODevice::WriteOnly)) {

		_p_xml_stream = new QXmlStreamWriter(_output_file);
	} else {
		//throw exception
	}

	_p_xml_stream->setAutoFormatting(true);
	_p_xml_stream->writeStartDocument();
	_p_xml_stream->writeStartElement("PROTICdb");
	// xmlns="http://www.w3.org/1999/xhtml" xml:lang="en"
	_p_xml_stream->writeNamespace("http://www.w3.org/2001/XMLSchema-instance",
			"xsi");
	_p_xml_stream->writeAttribute("http://www.w3.org/2001/XMLSchema-instance",
			"noNamespaceSchemaLocation",
			"http://uln/~langella/proticdev/doc/xml_schema/proticdbml/proticdbml.xsd");
	_p_xml_stream->writeAttribute("version", "1.0");
	_p_xml_stream->writeAttribute("type", "2Danalyses");
	_p_xml_stream->writeStartElement("detectMeth");
	_p_xml_stream->writeAttribute("name", "");
	_p_xml_stream->writeAttribute("id", "M1");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("scalingMeth");
	_p_xml_stream->writeAttribute("name", "");
	_p_xml_stream->writeAttribute("id", "M2");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("project");
	_p_xml_stream->writeAttribute("id", "p1");
	_p_xml_stream->writeAttribute("name", "");
	_p_xml_stream->writeStartElement("gelImages");
	_p_xml_stream->writeStartElement("gelImage");
	_p_xml_stream->writeAttribute("id", "I1");
	QFileInfo file_info(get_gel_image_file_name());
	_p_xml_stream->writeAttribute("name", file_info.baseName());
	_p_xml_stream->writeEndElement();
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("matchSet");
	_p_xml_stream->writeAttribute("name", "");

}

void spotPROTICdbDocument::close() {
	qDebug() << "spotPROTICdbDocument::close begin";
	std::vector<const spot *>::const_iterator it;

	_p_xml_stream->writeEndElement(); //matchset

	_p_xml_stream->writeStartElement("detectionList");
	_p_xml_stream->writeAttribute("master_id", "D1");
	_p_xml_stream->writeStartElement("detection");
	_p_xml_stream->writeAttribute("id", "D1");
	_p_xml_stream->writeAttribute("image_id", "I1");
	_p_xml_stream->writeAttribute("detect_meth_id", "M1");
	_p_xml_stream->writeAttribute("type", "native");
	for (it = _all_spots.begin(); it != _all_spots.end(); ++it) {
		const spot & thespot(**it);

		//std::ostringstream tag;
		_p_xml_stream->writeStartElement("spotGel");
		_p_xml_stream->writeAttribute("id", "sg" + QString::number(
				thespot.get_number()));
		_p_xml_stream->writeAttribute("num", QString::number(
				thespot.get_number()));
		//*this << "                <spotGel id=\"s" << thespot.get_number()
		//		<< "\" num=\"" << thespot.get_number();
		_p_xml_stream->writeAttribute("spot_id", "s" + QString::number(
				thespot.get_number()));

		if (!thespot.get_picked_num().isEmpty()) {
			qDebug()
					<< "spotPROTICdbDocument::close() get_picked_num not empty";
			_p_xml_stream->writeAttribute("manual_num",
					thespot.get_picked_num());
			//*this << "\" manual_num=\"" << thespot.get_picked_num();


			//write sample before closing project tag <samples>:
			_samples.push_back(&thespot);
			//_samples << "<sample id=\"ms" << thespot.get_number() << "\"";
			//_samples << " name=\"" << thespot.get_picked_num() << "\"";
			//_samples << " spotgel_id=\"s" << thespot.get_number() << "\"";
			//_samples << " taxa=\"\">" << endl;
			//_samples << "</sample>" << endl;
		} else {
			//qDebug() << "spotPROTICdbDocument::write_spot(const spot & thespot) get_picked_num empty";
		}
		//*this << "\">" << std::endl;
		_p_xml_stream->writeStartElement("x");
		_p_xml_stream->writeCharacters(QString::number(thespot.get_x()));
		_p_xml_stream->writeEndElement();
		//*this << "                    <x>" << thespot.get_x() << "</x>"
		//		<< std::endl;
		_p_xml_stream->writeStartElement("y");
		_p_xml_stream->writeCharacters(QString::number(thespot.get_y()));
		_p_xml_stream->writeEndElement();
		//*this << "                    <y>" << thespot.get_y() << "</y>"
		//		<< std::endl;
		if (thespot.is_quantified()) {
			_p_xml_stream->writeStartElement("sc");
			_p_xml_stream->writeAttribute("sc_meth_id", "M2");
			//*this << "                    <sc sc_meth_id=\"M2\">" << std::endl;
			_p_xml_stream->writeStartElement("vol");
			_p_xml_stream->writeCharacters(QString::number(thespot.get_vol()));
			_p_xml_stream->writeEndElement();
			//*this << "                        <vol>" << thespot.get_vol()
			//		<< "</vol>" << std::endl;
			//*this << "                        <area>" << thespot.get_area() << "</area>" << std::endl;
			_p_xml_stream->writeStartElement("ud");
			_p_xml_stream->writeAttribute("acc", "PROTICdbO:0000056");
			_p_xml_stream->writeAttribute("value", QString::number(
					thespot.get_area()));
			_p_xml_stream->writeEndElement();
			//*this
			//		<< "                        <ud acc=\"PROTICdbO:0000056\" value=\""
			//		<< thespot.get_area() << "\"/>" << std::endl;
			_p_xml_stream->writeStartElement("ud");
			_p_xml_stream->writeAttribute("acc", "PROTICdbO:0000277");
			_p_xml_stream->writeAttribute("value", QString::number(
					thespot.get_bckgnd()));
			_p_xml_stream->writeEndElement();
			//*this
			//		<< "                        <ud acc=\"PROTICdbO:0000277\" value=\""
			//		<< thespot.get_bckgnd() << "\"/>" << std::endl;
			_p_xml_stream->writeEndElement();
			//*this << "                    </sc>" << std::endl;
		}
		_p_xml_stream->writeEndElement();

	}

	_p_xml_stream->writeEndElement();
	//*this << "            </detection>" << std::endl;
	_p_xml_stream->writeEndElement();
	//*this << "        </detectionList>" << std::endl;


	_p_xml_stream->writeStartElement("samples");

	for (it = _samples.begin(); it != _samples.end(); ++it) {
		const spot & thespot(**it);
		_p_xml_stream->writeStartElement("sample");
		_p_xml_stream->writeAttribute("id", "ms" + QString::number(
				thespot.get_number()));
		//_samples << "<sample id=\"ms" << thespot.get_number() << "\"";
		_p_xml_stream->writeAttribute("name", thespot.get_picked_num());
		//_samples << " name=\"" << thespot.get_picked_num() << "\"";
		_p_xml_stream->writeAttribute("spotgel_id", "sg" + QString::number(
				thespot.get_number()));
		//_samples << " spotgel_id=\"s" << thespot.get_number() << "\"";
		_p_xml_stream->writeAttribute("taxa", "");
		//_samples << " taxa=\"\">" << endl;
		_p_xml_stream->writeEndElement();
		//_samples << "</sample>" << endl;
	}

	_p_xml_stream->writeEndElement();
	//	_samples << "</samples>" << endl;

	//*this << _samples.str() << std::endl;
	//cout << mystr;

	//*this << "    </project>" << std::endl;
	//*this << "</PROTICdb>" << std::endl;
	//std::ofstream::close();
	_p_xml_stream->writeEndDocument();

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
	qDebug() << "spotPROTICdbDocument::close end";

}

void spotPROTICdbDocument::write_spot(const spot & thespot) {
	qDebug() << "spotPROTICdbDocument::write_spot(const spot & thespot) begin";

	this->_all_spots.push_back(&thespot);

	_p_xml_stream->writeStartElement("spot");
	_p_xml_stream->writeAttribute("id", "s" + QString::number(
			thespot.get_number()));
	_p_xml_stream->writeAttribute("num", QString::number(thespot.get_number()));
	/* <matchSet name="">
	 <spot num="1" id="ms1"/>
	 </matchSet>*/
	_p_xml_stream->writeEndElement();

	//*this << "                </spotGel>" << std::endl;

	//return (tag.str().c_str());
}


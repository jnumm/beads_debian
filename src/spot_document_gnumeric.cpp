/*
 * spot_document_gnumeric.cpp
 *
 *  Created on: 12 mars 2010
 *      Author: olivier
 */

#include <iostream>
#include <QFileInfo>

#include "spot_document_gnumeric.h"

SpotDocumentGnumeric::SpotDocumentGnumeric() {
	irow = 0;
}

SpotDocumentGnumeric::~SpotDocumentGnumeric() {
	irow = 0;
}

void SpotDocumentGnumeric::open(const QString & filename) {
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

	irow = 0;
	_output_file = new QFile(filename);
	_output_stream = NULL;

	if (_output_file->exists()) {
		cerr << "WARNING : " << _output_file->fileName().toStdString()
				<< " gnumeric file already exists" << endl;
	}
	if (_output_file->open(QIODevice::WriteOnly)) {

		_p_xml_stream = new QXmlStreamWriter(_output_file);
	} else {
		//throw exception
	}

	_p_xml_stream->setAutoFormatting(true);
	_p_xml_stream->writeStartDocument();
	_p_xml_stream->writeNamespace("http://www.gnumeric.org/v10.dtd", "gnm");
	_p_xml_stream->writeNamespace("http://www.w3.org/2001/XMLSchema-instance",
			"xsi");
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Workbook");
	_p_xml_stream->writeAttribute("http://www.w3.org/2001/XMLSchema-instance",
			"schemaLocation", "http://www.gnumeric.org/v9.xsd");
	//<gnm:Version Epoch="1" Major="9" Minor="9" Full="1.9.9"/>
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Version");
	_p_xml_stream->writeAttribute("Epoch", "1");
	_p_xml_stream->writeAttribute("Major", "9");
	_p_xml_stream->writeAttribute("Minor", "9");
	_p_xml_stream->writeAttribute("Full", "1.9.9");
	_p_xml_stream->writeEndElement();
	//<gnm:Attributes>
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Attributes");
	//<gnm:Attribute>
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Attribute");
	//<gnm:type>4</gnm:type>
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "type",
			"4");
	//<gnm:name>WorkbookView::show_horizontal_scrollbar</gnm:name>
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"WorkbookView::show_horizontal_scrollbar");
	//<gnm:value>TRUE</gnm:value>
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "value",
			"TRUE");
	_p_xml_stream->writeEndElement();

	/*
	 *     <gnm:Attribute>
	 <gnm:type>4</gnm:type>
	 <gnm:name>WorkbookView::show_vertical_scrollbar</gnm:name>
	 <gnm:value>TRUE</gnm:value>
	 </gnm:Attribute>
	 */
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Attribute");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "type",
			"4");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"WorkbookView::show_vertical_scrollbar");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "value",
			"TRUE");
	_p_xml_stream->writeEndElement();

	/*
	 <gnm:Attribute>
	 <gnm:type>4</gnm:type>
	 <gnm:name>WorkbookView::show_notebook_tabs</gnm:name>
	 <gnm:value>TRUE</gnm:value>
	 </gnm:Attribute>
	 */
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Attribute");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "type",
			"4");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"WorkbookView::show_notebook_tabs");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "value",
			"TRUE");
	_p_xml_stream->writeEndElement();
	/*
	 <gnm:Attribute>
	 <gnm:type>4</gnm:type>
	 <gnm:name>WorkbookView::do_auto_completion</gnm:name>
	 <gnm:value>TRUE</gnm:value>
	 </gnm:Attribute>
	 */
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Attribute");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "type",
			"4");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"WorkbookView::do_auto_completion");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "value",
			"TRUE");
	_p_xml_stream->writeEndElement();
	/*
	 <gnm:Attribute>
	 <gnm:type>4</gnm:type>
	 <gnm:name>WorkbookView::is_protected</gnm:name>
	 <gnm:value>FALSE</gnm:value>
	 </gnm:Attribute>
	 *
	 */
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Attribute");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "type",
			"4");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"WorkbookView::is_protected");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "value",
			"FALSE");
	_p_xml_stream->writeEndElement();
	//gnm:Attributes
	_p_xml_stream->writeEndElement();

	/*
	 *   <office:document-meta xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
	 *   xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
	 *   xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
	 *   xmlns:ooo="http://openoffice.org/2004/office" office:version="1.1">
	 <office:meta/>
	 </office:document-meta>
	 *
	 */
	_p_xml_stream->writeNamespace(
			"urn:oasis:names:tc:opendocument:xmlns:office:1.0", "office");
	_p_xml_stream->writeNamespace("http://www.w3.org/1999/xlink", "xlink");
	_p_xml_stream->writeNamespace("http://purl.org/dc/elements/1.1/", "dc");
	_p_xml_stream->writeNamespace(
			"urn:oasis:names:tc:opendocument:xmlns:meta:1.0", "meta");
	_p_xml_stream->writeNamespace("http://openoffice.org/2004/office", "ooo");
	_p_xml_stream->writeStartElement(
			"urn:oasis:names:tc:opendocument:xmlns:office:1.0", "document-meta");
	_p_xml_stream->writeAttribute(
			"urn:oasis:names:tc:opendocument:xmlns:office:1.0", "version",
			"1.1");
	_p_xml_stream->writeStartElement(
			"urn:oasis:names:tc:opendocument:xmlns:office:1.0", "meta");
	_p_xml_stream->writeEndElement();
	_p_xml_stream->writeEndElement();

	//  <gnm:Calculation ManualRecalc="0" EnableIteration="1" MaxIterations="100"
	//IterationTolerance="0.001" FloatRadix="2" FloatDigits="53"/>
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Calculation");
	_p_xml_stream->writeAttribute("ManualRecalc", "0");
	_p_xml_stream->writeAttribute("EnableIteration", "1");
	_p_xml_stream->writeAttribute("MaxIterations", "100");
	_p_xml_stream->writeAttribute("IterationTolerance", "0.001");
	_p_xml_stream->writeAttribute("FloatRadix", "2");
	_p_xml_stream->writeAttribute("FloatDigits", "53");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Summary");
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Item");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"biology softwares");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd",
			"val-string", "http://pappso.inra.fr/");
	_p_xml_stream->writeEndElement();
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Item");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "name",
			"author");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd",
			"val-string", "Olivier Langella <Olivier.Langella@moulon.inra.fr>");
	_p_xml_stream->writeEndElement();
	_p_xml_stream->writeEndElement();
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"SheetNameIndex");
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd",
			"SheetName", "beads");
	_p_xml_stream->writeEndElement();
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd",
			"Sheets");

	//QString sheetname("Matrix 1");
	//oGnumericSheet(xml_stream, sheetname);
	/*fichier << "<gmr:Sheet ";
	 fichier
	 << "DisplayFormulas=\"false\" HideZero=\"false\" HideGrid=\"false\" HideColHeader=\"false\" HideRowHeader=\"false\" DisplayOutlines=\"true\" OutlineSymbolsBelow=\"true\" OutlineSymbolsRight=\"true\">"
	 << endl;*/
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Sheet");
	_p_xml_stream->writeAttribute("DisplayFormulas", "false");
	_p_xml_stream->writeAttribute("HideZero", "false");
	_p_xml_stream->writeAttribute("HideGrid", "false");
	_p_xml_stream->writeAttribute("HideColHeader", "false");
	_p_xml_stream->writeAttribute("HideRowHeader", "false");
	_p_xml_stream->writeAttribute("DisplayOutlines", "true");
	_p_xml_stream->writeAttribute("OutlineSymbolsBelow", "true");
	_p_xml_stream->writeAttribute("OutlineSymbolsRight", "true");

	/*_p_xml_stream->writeAttribute("xml", "lang", "en");
	 _p_xml_stream->writeStartElement("body");
	 _p_xml_stream->writeStartElement("table");
	 _p_xml_stream->writeStartElement("tbody");*/

	// 	fichier.imbue();
	//	fichier << "<gmr:Name>" << sheetname << "</gmr:Name>" << endl;
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "Name",
			"beads");

	//fichier << "<gmr:MaxCol>" << (GetNC() + 2) << "</gmr:MaxCol>" << endl;
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd",
			"MaxCol", "0");
	//fichier << "<gmr:MaxRow>" << (GetNL() + 5 + _titre.size())
	//		<< "</gmr:MaxRow>" << endl;
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd",
			"MaxRow", "0");
	//fichier << "<gmr:Zoom>1.000000</gmr:Zoom>" << endl;
	_p_xml_stream->writeTextElement("http://www.gnumeric.org/v10.dtd", "Zoom",
			"1.000000");

	//fichier << "<gmr:Names/>" << endl;
	_p_xml_stream->writeEmptyElement("http://www.gnumeric.org/v10.dtd", "Names");

	//fichier << "<gmr:Cells>" << endl;
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cells");

	//*_output_stream << "Spot Volume x y min surface background vol-bckgnd\n";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "0");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("Spot");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "1");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("Volume");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "2");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("x");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "3");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("y");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "4");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("min");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "5");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("surface");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "6");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("background");
	_p_xml_stream->writeEndElement();

	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", "0");
	_p_xml_stream->writeAttribute("Col", "7");
	_p_xml_stream->writeAttribute("ValueType", "60");
	_p_xml_stream->writeCharacters("vol-bckgnd");
	_p_xml_stream->writeEndElement();

	irow++;

}

void SpotDocumentGnumeric::write_spot(const spot & thespot) {
	//std::ostringstream tag;
	//*_output_stream << thespot.get_number() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "0");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_number()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << thespot.get_vol() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "1");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_vol()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << thespot.get_tx() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "2");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_tx()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << thespot.get_ty() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "3");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_ty()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << thespot.get_tmin() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "4");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_tmin()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << thespot.get_area() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "5");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_area()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << thespot.get_bckgnd() << " ";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "6");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_bckgnd()));
	_p_xml_stream->writeEndElement();

	//*_output_stream << (thespot.get_vol() - thespot.get_bckgnd()) << "\n";
	_p_xml_stream->writeStartElement("http://www.gnumeric.org/v10.dtd", "Cell");
	_p_xml_stream->writeAttribute("Row", QString::number(irow));
	_p_xml_stream->writeAttribute("Col", "7");
	_p_xml_stream->writeAttribute("ValueType", "40");
	_p_xml_stream->writeCharacters(QString::number(thespot.get_vol()
			- thespot.get_bckgnd()));
	_p_xml_stream->writeEndElement();

	irow++;
	//return (tag.str().c_str());
}

void SpotDocumentGnumeric::close() {
	qDebug() << "SpotDocumentGnumeric::close begin";
	_p_xml_stream->writeEndDocument();
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
	qDebug() << "SpotDocumentGnumeric::close end";
}

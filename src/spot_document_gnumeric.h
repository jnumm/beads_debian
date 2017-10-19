/*
 * spot_document_gnumeric.h
 *
 *  Created on: 12 mars 2010
 *      Author: olivier
 */

#ifndef SPOT_DOCUMENT_GNUMERIC_H_
#define SPOT_DOCUMENT_GNUMERIC_H_

#include <QXmlStreamWriter>

#include "spotDocument.h"

class SpotDocumentGnumeric: public spotDocument {
public:
	SpotDocumentGnumeric();
	virtual ~SpotDocumentGnumeric();

	virtual void open(const QString & filename);
	virtual void close();

	//virtual void write_detection(detection & the_detection);
	virtual void write_spot(const spot &);

private:
	int irow;
	QXmlStreamWriter * _p_xml_stream;

};

#endif /* SPOT_DOCUMENT_GNUMERIC_H_ */

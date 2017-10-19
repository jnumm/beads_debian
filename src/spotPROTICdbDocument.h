#ifndef SPOTPROTICDBDOCUMENT_H_
#define SPOTPROTICDBDOCUMENT_H_

#include <QXmlStreamWriter>

#include "spotDocument.h"

class spotPROTICdbDocument: public spotDocument {
public:
	spotPROTICdbDocument() {
	}

	~spotPROTICdbDocument() {
	}

	void open(const QString & filename);
	virtual void close();
	void write_spot(const spot &);

protected:
	std::vector<const spot *> _all_spots;
	std::vector<const spot *> _samples;

	QXmlStreamWriter * _p_xml_stream;

};

#endif /*SPOTPROTICDBDOCUMENT_H_*/

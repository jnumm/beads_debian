#ifndef SPOTSVGDOCUMENT_H_
#define SPOTSVGDOCUMENT_H_

#include <QXmlStreamWriter>

#include <map>
#include "spotDocument.h"

class spotSvgDocument: public spotDocument {
public:
	spotSvgDocument();
	~spotSvgDocument() {
	}

	void open(const QString & filename);
	virtual void close();

	void embed_gel_image() {
		embed_gel_image(get_gel_image_file_name());
	}

	void embed_gel_image(const imageIntensity & gel_image);
	void embed_gel_image(const QString & gel_image_file_name);

	void write_detection(const detection & the_detection,
			const QString & cssclass);
	void
	draw_contours(const detection & my_detection, const QString & cssclass);
	void draw_spot_numbers(detection & my_detection, const QString & cssclass);

	void set_css_class(const QString & css_class_name,
			const QString & css_style);

protected:

	QXmlStreamWriter * _p_xml_stream;

private:

	void write_gel_image();

	void write_spot(const spot & thespot, const QString & cssclass);
	void
	write_spot_contours(const spot & thespot, const QString & cssclass);
	void write_spot_number(const spot & thespot, const QString & cssclass);

	std::map<QString, QString> _css_classes;

	QString _encoded_gel_image;
	int _gel_image_width;
	int _gel_image_height;

	bool _embed_gel_image;

	bool _gel_image_writed;

};

/*
 * image could be embedded in the svg file using base 64 encoding :

 <image width="962.000000" height="725.000000" xlink:href="data:image/jpg;base64,
 iVBORw0KGgoAAAANSUhEUgAAA8IAAALVCAIAAACELc9tAAFNr0lEQVR42uy9B3Qb1523zX2zb+LE
 AAAAAABAowEAAAAAAAiD/w9XlqS2KZ32NgAAAABJRU5ErkJggg==
 " />
 * 
 * */

#endif /*SPOTSVGDOCUMENT_H_*/

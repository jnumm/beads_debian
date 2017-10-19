/*
 * qtbeads_error.h
 *
 *  Created on: 27 nov. 2009
 *      Author: olivier
 */

#ifndef QTBEADS_ERROR_H_
#define QTBEADS_ERROR_H_

#include <exception>
#include <QString>

class qtbeadsError: public std::exception {
public:
	qtbeadsError(QString message = "") throw () :
		_message(message) {
	}

	virtual const QString & getMessage() const throw () {
		return _message;
	}

	virtual const char* what() const throw () {
		return _message.toStdString().c_str();
	}

	virtual ~qtbeadsError() throw () {
	}

private:
	QString _message; //Description de l'erreur
};

#endif /* QTBEADS_ERROR_H_ */

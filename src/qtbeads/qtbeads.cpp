#include <QDebug>
#include <QApplication>
#include "../config.h"
#include "main_window.h"

int main(int argc, char * * argv) {
	QApplication a(argc, argv);
	QLocale::setDefault(QLocale::system());

	MainWindow w;
	w.show();

	return a.exec();
}

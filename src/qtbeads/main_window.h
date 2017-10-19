/*
 * main_windows.h
 *
 *  Created on: 27 nov. 2009
 *      Author: langella
 *
 *      http://doc.trolltech.com/4.2/widgets-imageviewer.html
 */

#ifndef MAIN_WINDOWS_H_
#define MAIN_WINDOWS_H_

#include <QPrinter>
#include <QMainWindow>
#include <QScrollBar>

#include "properties.h"
#include "beads_results.h"
#include "qtbeads_error.h"
#include "q_gel_image_scroll.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
//class QScrollBar;

using namespace std;

class MainWindow: public QMainWindow {
Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();

private slots :
	void openFile();
	void openParamFile();
	void saveFile();
	void saveStepsDirectory();
	void runDetection();
	void runDetection(QDir & directory_to_save_all_steps);
	void inverseGelImage(bool checked);

	void print();
	void zoomIn();
	void zoomOut();
	void normalSize();
	void fitToWindow();
	void about();
	void moveOnImage(QMouseEvent * event);
	void doubleClickOnImage(QMouseEvent * event);
	void moveGelImage(QMouseEvent * event);

private:
	void createActions();
	void createMenus();
	void updateActions();
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void runDetection(QString directory_to_save_all_steps);

	QLabel *positionLabel;
	QGelImageScroll *scrollArea;

	QPrinter printer;

	QAction *openAct;
	QAction *openParamFileAct;
	QAction *saveAct;
	QAction *saveStepsAct;
	QAction *printAct;
	QAction *exitAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
	QAction *runDetectionAct;
	QAction *inverseAct;

	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *detectionMenu;
	QMenu *helpMenu;

	Properties _properties;
	BeadsResults * _p_results;

	int _before_x;
	int _before_y;

	bool _moving_gel_image;

};

#endif /* MAIN_WINDOWS_H_ */

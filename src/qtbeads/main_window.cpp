/*
 * main_windows.cpp
 *
 *  Created on: 27 nov. 2009
 *      Author: langella
 */

#include <QtGui>
#include <iostream>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include "main_window.h"
#include "QCimg.h"
#include "../config.h"

#include "../images/imageCoule.h"
#include "../images/imageProb.h"
#include "../images/imagePaths.h"
#include "../images/imageDetection.h"
#include "../images/imageConfluent.h"
#include "../images/imageContours.h"
#include "../images/imageDeNovo.h"
#include "../parameters.h"

bool copy_file(QString inputfile, QString outputfile) {
	QFile input(inputfile);
	if (!input.open(QIODevice::ReadOnly)) {
		// error
		return false;
	}
	QFile output(outputfile);
	if (!output.open(QIODevice::WriteOnly)) {
		// error
		return false;
	}
	QTextStream inputstream(&input);
	QTextStream outputstream(&output);
	outputstream << inputstream.readAll();
	output.close();
	input.close();
	return true;
}

MainWindow::MainWindow() {
	_moving_gel_image = false;
	QFileInfo beads_icon;
	beads_icon.setFile("/usr/share/beads/beads_icon.svg");
	if (beads_icon.exists()) {
		setWindowIcon(QIcon(beads_icon.filePath()));
	}
	beads_icon.setFile("/usr/local/share/beads/beads_icon.svg");
	if (beads_icon.exists()) {
		setWindowIcon(QIcon(beads_icon.filePath()));
	}

	_p_results = NULL;
	setMenuBar(menuBar());
	setStatusBar(QMainWindow::statusBar());

	scrollArea = new QGelImageScroll;
	setCentralWidget(scrollArea);

	positionLabel = new QLabel;
	positionLabel->setText("0, 0");
	statusBar()->addWidget(positionLabel);

	createActions();
	createMenus();

	setWindowTitle(tr("Beads ").append(BEADS_VERSION));
	resize(500, 400);

connect(scrollArea->getGelImage(), SIGNAL(mouseMoved(QMouseEvent *)), this, SLOT(
				moveOnImage(QMouseEvent *)));
connect(scrollArea->getGelImage(), SIGNAL(doubleClicked(QMouseEvent *)), this, SLOT(
				doubleClickOnImage(QMouseEvent *)));
//connect(imageLabel, SIGNAL(moveGelImage(QMouseEvent *)), this, SLOT(
//				moveGelImage(QMouseEvent *)));

}

MainWindow::~MainWindow() {
}

void MainWindow::moveOnImage(QMouseEvent * event) {
	int x(scrollArea->getGelImage()->getX(event->x()));
	int y(scrollArea->getGelImage()->getY(event->y()));
	_before_x = x;
	_before_y = y;

	QString message(QString::number(x) + ", " + QString::number(y));

	if ((_p_results != NULL) && (_p_results->getSpotNumber(x, y) != -1)) {
		int spot_num(_p_results->getSpotNumber(x, y));
		message += " spot : " + QString::number(spot_num);

		if (_p_results->getDetection() != NULL) {
			QString
					picked_num(
							_p_results->getDetection()->find(spot_num)->second.get_picked_num());
			if (picked_num != "") {
				message += " picked num : " + picked_num;
			}
		}
	}
	positionLabel->setText(message);

}

void MainWindow::doubleClickOnImage(QMouseEvent * event) {
	int x(scrollArea->getGelImage()->getX(event->x()));
	int y(scrollArea->getGelImage()->getY(event->y()));

	if ((_p_results != NULL) && (_p_results->getSpotNumber(x, y) != -1)) {
		int spot_num(_p_results->getSpotNumber(x, y));
		bool ok(false);
		QString
				old_picked_num(
						_p_results->getDetection()->find(spot_num)->second.get_picked_num());

		QString picked_num = QInputDialog::getText(this, tr(
				"edit spot %1 picked num").arg(spot_num),
				tr("spot picked num:"), QLineEdit::Normal, old_picked_num, &ok);
		if (ok) {
			_p_results->getDetection()->find(spot_num)->second.set_picked_num(
					picked_num);
		}
	}

}

void MainWindow::moveGelImage(QMouseEvent * event) {

	if (_moving_gel_image) {

	} else {
		_moving_gel_image = true;
		int dx(event->x() - _before_x);
		int dy(event->y() - _before_y);

		_before_x = event->x();
		_before_y = event->y();

		qDebug() << event->x() << " " << event->y() << endl;
		//scrollArea->widget()->move(event->x(), event->y());
		scrollArea->horizontalScrollBar()->setSliderPosition(
				scrollArea->horizontalScrollBar()->value() - dx);
		scrollArea->verticalScrollBar()->setSliderPosition(
				scrollArea->verticalScrollBar()->value() - dy);
		_moving_gel_image = false;
	}

}

void MainWindow::openFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr(
			"Open qtbeads file"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		QImage image(fileName);
		if (image.isNull()) {
			QMessageBox::information(this, tr("qtbeads"),
					tr("Cannot load %1.").arg(fileName));
			return;
		}
		this->setCursor(Qt::BusyCursor);
		_properties.setGelImageFilename(fileName);
		scrollArea->setGelImage(image);

		printAct->setEnabled(true);
		fitToWindowAct->setEnabled(true);
		runDetectionAct->setEnabled(true);
		inverseAct->setEnabled(true);
		updateActions();
		this->setCursor(Qt::ArrowCursor);

		if (!fitToWindowAct->isChecked())
			scrollArea->adjustSize();
	}
}

void MainWindow::openParamFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr(
			"Open qtbeads parameter file"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		_properties.setParamFilename(fileName);
	}
}

//saveAct

void MainWindow::saveFile() {
	QString
			fileName =
					QFileDialog::getSaveFileName(
							this,
							tr("Save File"),
							QDir::currentPath().append("/untitled.png"),
							tr(
									"Images *.png *.jpg (*.png *.jpg);;Text *.txt (*.txt);;Gnumeric *.gnumeric (*.gnumeric);;PROTICdbML *.xml (*.xml);;Scalable Vector Graphic *.svg (*.svg)"));

	if (!fileName.isEmpty()) {
		this->setCursor(Qt::BusyCursor);
		QFileInfo to_save(fileName);
		if ((to_save.suffix() == "PNG") || (to_save.suffix() == "png")
				|| (to_save.suffix() == "jpg") || (to_save.suffix() == "JPG")) {
			_p_results->saveImageContour(to_save);
		}
		if ((to_save.suffix() == "XML") || (to_save.suffix() == "xml")) {
			_p_results->saveProticDbMl(to_save);
		}
		if ((to_save.suffix() == "txt") || (to_save.suffix() == "TXT")) {
			_p_results->saveText(to_save);
		}
		if ((to_save.suffix() == "svg") || (to_save.suffix() == "SVG")) {
			_p_results->saveSvg(to_save);
		}
		if (to_save.suffix() == "gnumeric") {
			_p_results->saveGnumeric(to_save);
		}
		this->setCursor(Qt::ArrowCursor);
	}
}

void MainWindow::saveStepsDirectory() {

	QFileDialog * choose_directory = new QFileDialog();
	choose_directory->setFileMode(QFileDialog::Directory);

	if (choose_directory->exec() == QDialog::Accepted) {
		this->setCursor(Qt::BusyCursor);
		QDir directory(choose_directory->selectedFiles().value(0));
		qDebug() << "MainWindow::saveStepsDirectory() " << directory.path();
		this->runDetection(directory);
		this->setCursor(Qt::ArrowCursor);
	}
	delete (choose_directory);
}

void MainWindow::print() {
	Q_ASSERT(scrollArea->getGelImage()->pixmap());
	QPrintDialog dialog(&printer, this);
	if (dialog.exec()) {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = scrollArea->getGelImage()->pixmap()->size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(scrollArea->getGelImage()->pixmap()->rect());
		painter.drawPixmap(0, 0, *scrollArea->getGelImage()->pixmap());
	}
}

void MainWindow::zoomIn() {
	scaleImage(1.25);
}

void MainWindow::zoomOut() {
	scaleImage(0.8);
}

void MainWindow::normalSize() {
	scrollArea->adjustSize();
	scrollArea->setScaleFactor(1.0);

}

void MainWindow::fitToWindow() {
	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow) {
		normalSize();
	}
	updateActions();
}

void MainWindow::about() {
	QMessageBox::about(
			this,
			tr("About QTbeads %1").arg(BEADS_VERSION),
			tr(
					"<div><a href=\"http://pappso.inra.fr/bioinfo/beads/index.php\">QTbeads</a> %1 is a 2-DE electrophoresis gel image spot detection software developped by <a href=\"http://pappso.inra.fr/\">PAPPSO</a></div><div>beads is freely available under the terms of the <a href=\"http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.html\">CeCILL</a> licence</div>").arg(
					BEADS_VERSION));
}

void MainWindow::createActions() {

	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

	saveAct = new QAction(tr("&Save..."), this);
	//openAct->setShortcut(tr("Ctrl+O"));
	saveAct->setEnabled(false);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

	saveStepsAct = new QAction(tr("Save s&teps..."), this);
	//openAct->setShortcut(tr("Ctrl+O"));
	saveStepsAct->setEnabled(false);
	connect(saveStepsAct, SIGNAL(triggered()), this, SLOT(saveStepsDirectory()));

	printAct = new QAction(tr("&Print..."), this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setEnabled(false);
	connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+S"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	runDetectionAct = new QAction(tr("&Run"), this);
	runDetectionAct->setEnabled(false);
	connect(runDetectionAct, SIGNAL(triggered()), this, SLOT(runDetection()));

	inverseAct = new QAction(tr("Inverse"), this);
	inverseAct->setEnabled(false);
	inverseAct->setCheckable(true);
	inverseAct->setChecked(true);
	inverseGelImage(true);
	connect(inverseAct, SIGNAL(triggered(bool)), this, SLOT(inverseGelImage(bool)));

	openParamFileAct = new QAction(tr("Parameters"), this);
	connect(openParamFileAct, SIGNAL(triggered()), this, SLOT(openParamFile()));

	//aboutQtAct = new QAction(tr("About &Qt"), this);
	//connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveStepsAct);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(normalSizeAct);
	//viewMenu->addSeparator();
	//viewMenu->addAction(fitToWindowAct);

	detectionMenu = new QMenu(tr("&Detection"), this);

	detectionMenu->addAction(openParamFileAct);
	detectionMenu->addAction(inverseAct);
	detectionMenu->addSeparator();
	detectionMenu->addAction(runDetectionAct);

	/*
	 QMenu * menuDetection = new QMenu(tr("Detection"));
	 menubar->addMenu(menuDetection);
	 menuDetection->addAction(tr("Run"), this, SLOT(runDetection()));
	 */

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);
	//helpMenu->addAction(aboutQtAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(detectionMenu);
	menuBar()->addMenu(helpMenu);
}

void MainWindow::updateActions() {
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void MainWindow::scaleImage(double factor) {
	Q_ASSERT(scrollArea->getGelImage()->pixmap());
	scrollArea->scaleImage(factor);

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scrollArea->getScaleFactor() < 3.0);
	zoomOutAct->setEnabled(scrollArea->getScaleFactor() > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor) {
	scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1)
			* scrollBar->pageStep() / 2)));
}

void MainWindow::inverseGelImage(bool checked) {
	qDebug() << "MainWindow::inverseGelImage (bool checked) begin " << checked;

	_properties.setInverse(checked);

	if (_p_results == NULL) {
		if (_properties.gelImageIsLoaded()) {
			const imageIntensity * p_gel_image = _properties.get_p_working_image()->new_image_inversed();
			QCimg image(
					*p_gel_image);
			scrollArea->setGelImage(image);
			delete p_gel_image;
		}
	}
	qDebug() << "MainWindow::inverseGelImage (bool checked) end "
			<< _properties.isInverse();
}

void MainWindow::runDetection() {
	this->runDetection(QString(""));
}

void MainWindow::runDetection(QDir & directory_to_save_all_steps) {
	this->runDetection(directory_to_save_all_steps.path());
}

void MainWindow::runDetection(QString directory_to_save_all_steps) {
	qDebug()
			<< "MainWindow::runDetection (QString directory_to_save_all_steps) begin"
			<< directory_to_save_all_steps;
	try {
		qDebug() << "MainWindow::runDetection set cursor ";
		this->setCursor(Qt::BusyCursor);
		parameters beads_params(
				_properties.getParamFileInfo().filePath().toStdString());
		qDebug() << "Loading parameter file "
				<< _properties.getParamFileInfo().fileName();

		if (!directory_to_save_all_steps.isEmpty()) {
			copy_file(_properties.getParamFileInfo().filePath(),
					directory_to_save_all_steps + "/parameters.conf");
		}

		const imageIntensity * p_gel_image = _properties.get_p_working_image();
		qDebug() << "MainWindow::runDetection p_gel_image->get_is_inversed() "
				<< p_gel_image->get_is_inversed();
		qDebug() << "MainWindow::runDetection _properties.isInverse() "
				<< _properties.isInverse();

		qDebug() << "Computing bead directions " << endl;
		imageDirect directions;
		const parameterDirect & parameter_direct =
				beads_params.get_parameter_direction();
		directions.compute_from_gel_image(*p_gel_image, parameter_direct);

		if (!directory_to_save_all_steps.isEmpty()) {
			directions.save(directory_to_save_all_steps + "/DIRECTIONS.png");
		}

		qDebug() << "Beads are rolling" << endl;
		imageCoule rolling;
		rolling.compute_from_direct(directions);
		if (!directory_to_save_all_steps.isEmpty()) {
			rolling.save(directory_to_save_all_steps + "/BEADS.png");
		}

		//using confluent method :
		qDebug() << "Computing bead paths" << endl;
		imagePaths path;
		path.compute_from_direct_and_coule(directions, rolling);

		if (!directory_to_save_all_steps.isEmpty()) {
			path.save(directory_to_save_all_steps + "/PATHS.png");
		}

		qDebug() << "Detecting merging paths and arrival positions" << endl;
		const parameterConfluent & parameter_confl =
				beads_params.get_parameter_confluent();
		qDebug() << " minflux : " << parameter_confl.get_minflux() << endl;
		qDebug() << " minpath : " << parameter_confl.get_minpath() << endl;
		qDebug() << " winconfl : " << parameter_confl.get_winconfl() << endl;
		qDebug() << " minbeads : " << parameter_confl.get_minbeads() << endl;

		imageConfluent confluent;
		confluent.compute_from_direct_and_paths(directions, path, *p_gel_image,
				parameter_confl);
		if (!directory_to_save_all_steps.isEmpty()) {
			confluent.save(directory_to_save_all_steps + "/SELECT.png");
		}

		qDebug() << "Computing probabilites of spot positions" << endl;
		const parameterProb & parameter_prob =
				beads_params.get_parameter_prob();
		qDebug() << " threshold : " << parameter_prob.get_threshold() << endl;
		qDebug() << " sx : " << parameter_prob.get_sx() << endl;
		qDebug() << " sy : " << parameter_prob.get_sy() << endl;

		imageProb dest_image_prob;

		dest_image_prob.compute_from_image_intensity(confluent, parameter_prob);
		if (!directory_to_save_all_steps.isEmpty()) {
			dest_image_prob.save(directory_to_save_all_steps
					+ "/PROBABILITIES.png");
		}

		qDebug() << "Detecting spots" << endl;
		const parameterDetection & parameter_detection =
				beads_params.get_parameter_detection();
		qDebug() << " minproba : " << parameter_detection.get_minproba()
				<< endl;

		detection * p_the_detection_from_prob = new detection();
		p_the_detection_from_prob->detect_from_prob(dest_image_prob,
				parameter_detection);

		//imageDetection img_detect(*p_the_detection_from_prob);

		qDebug() << "Quantification" << endl;
		const parameterNumber & parameter_number =
				beads_params.get_parameter_number();

		imageNumber * p_numbers = new imageNumber();
		//quantification des spots:
		p_numbers->compute_from_direct(directions, *p_the_detection_from_prob,
				parameter_number);

		int npass = parameter_number.get_npass();
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "image numbers expand_areas_down_the_slope before";

		p_numbers->expand_areas_down_the_slope(*p_gel_image, npass);
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "image numbers expand_areas_down_the_slope after";

		if (!directory_to_save_all_steps.isEmpty()) {
			p_numbers->save(directory_to_save_all_steps + "/NUMBERS.png");
		}

		imageContours contours;
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "compute_from_gel_image_and_numeros before";
		contours.compute_from_gel_image_and_numeros(*p_gel_image, *p_numbers);
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "compute_from_gel_image_and_numeros after";
		contours.draw_contours(*p_the_detection_from_prob,
				_properties.isInverse());
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "draw_contours finished";
		const imageIntensity * image_inversed = contours.new_image_inversed();
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "new_image_inversed finished";

		if (!directory_to_save_all_steps.isEmpty()) {
			image_inversed->save(directory_to_save_all_steps + "/CONTOURS.png");
		}
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "coucou";

		if (directory_to_save_all_steps.isEmpty()) {
			QCimg image(*image_inversed);
			scrollArea->setGelImage(image);

			if (_p_results != NULL) {
				delete (_p_results);
			}
			_p_results = new BeadsResults(_properties);
			//_p_results->setGelImage(p_gel_image);
			_p_results->setDetection(p_the_detection_from_prob);
			_p_results->setNumberImage(p_numbers);
		}
		qDebug()
				<< "MainWindow::runDetection (QString directory_to_save_all_steps) "
				<< "deleting (image_inversed)";
		delete (image_inversed);

		saveAct->setEnabled(true);
		saveStepsAct->setEnabled(true);
		this->setCursor(Qt::ArrowCursor);
	} catch (qtbeadsError& e) {

		qDebug() << "an error occured " << e.getMessage();
		QMessageBox::critical(this, tr("qtbeads"),
				tr("Detection aborted : ").append(e.getMessage()));
		/*
		 QMessageBox::about(
		 this,
		 tr("About QTbeads %1").arg(BEADS_VERSION),
		 tr(
		 "<div><a href=\"http://pappso.inra.fr/bioinfo/beads/index.php\">QTbeads</a> %1 is a 2-DE electrophoresis gel image spot detection software developped by <a href=\"http://pappso.inra.fr/\">PAPPSO</a></div><div>beads is freely available under the terms of the <a href=\"http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.html\">CeCILL</a> licence</div>").arg(
		 BEADS_VERSION));*/
	}
}

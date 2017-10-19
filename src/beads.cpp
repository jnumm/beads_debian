#include "beads.h"
#include <vector>
#include <iostream>
#include "config.h"

#include "images/imageCoule.h"
#include "images/imageProb.h"
#include "images/imagePaths.h"
#include "images/imageDetection.h"
#include "images/imageConfluent.h"
#include "images/imageContours.h"
#include "images/imageDeNovo.h"
#include "spot.h"
#include "detection.h"
#include "spotSvgDocument.h"
#include "spotPROTICdbDocument.h"

#include "parameters.h"

#include <QFileInfo>

using namespace std;

// The undef below is necessary when using a non-standard compiler.
#ifdef cimg_use_visualcpp6
#define std
#endif

void create_default_parameter_file(const string filename) {
	ofstream param_stream;
	param_stream.open(filename.c_str());
	param_stream << "#" << filename << endl;
	param_stream << "# beads configuration file" << endl;
	param_stream << "# default parameters, optimized for silver staining"
			<< endl;
	param_stream << endl;
	param_stream << "################################" << endl;
	param_stream << "# parameters for DIRECTIONS    #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "burned_pixel_threshold = 63000" << endl;
	param_stream << endl;
	param_stream << "################################" << endl;
	param_stream << "# parameters for SELECT        #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "minflux = 100" << endl;
	param_stream << "minpath = 2" << endl;
	param_stream << "winconfl = 300" << endl;
	param_stream << "minbeads = 200" << endl;
	param_stream << "confluent_intmax = 60000" << endl;
	param_stream << "confluent_minpct = 1" << endl;
	param_stream << endl;
	param_stream << "################################" << endl;
	param_stream << "# parameters for PROBABILITIES #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "prob_threshold = 10" << endl;
	param_stream << "sx = 3" << endl;
	param_stream << "sy = 3" << endl;
	param_stream << "sx_bottom = 5" << endl;
	param_stream << "sy_bottom = 5" << endl;
	param_stream << endl;

	param_stream << "################################" << endl;
	param_stream << "# parameters for detections #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "minproba  = 400" << endl;
	param_stream << endl;

	param_stream << "################################" << endl;
	param_stream << "# parameters for quantification #" << endl;
	param_stream << "################################" << endl;
	param_stream << endl;
	param_stream << "quantification_enlarge_fusion = 3" << endl;
	param_stream << "npass = 10" << endl;
	param_stream << endl;
	param_stream.close();
}

bool exist(const string filename) {
	ifstream in;
	in.open(filename.c_str());
	if (in.fail()) {
		in.close();
		return false;
	}
	in.close();
	return true;
}

// Main procedure
//---------------
int main(int argc, char * * argv) {

	cout << "beads " << BEADS_VERSION << endl;

	const bool save_steps =
			cimg_option("--steps",false,"save each step of the beads process");

	//argument for parameter file :
	string parameter_filename =
			cimg_option("--param","","configuration file for beads");
	if (parameter_filename == "") {
		parameter_filename
				= cimg_option("-p","beads.conf","configuration file for beads");
	}
	if (parameter_filename == "beads.conf") {
		if (!exist(parameter_filename)) {
			cerr << "ERROR : parameter file not found" << endl;
			cerr
					<< "creating an empty parameter file in the current directory named \""
					<< parameter_filename << "\"" << endl;
			create_default_parameter_file(parameter_filename);
		}
	}
	qDebug() << " main(int argc, char * * argv) loading parameter_filename " << parameter_filename.c_str();
	parameters beads_params(parameter_filename);

	//argument to inverse or not source gel image :
	const bool
			inverse =
					cimg_option("--inverse",true,"Inverse intensity (true if spot intensity lower than background)");

	// Read and check command line parameters
	const std::string sub_function(
			cimg_option("-sub","nosub","function to apply"));

	if (sub_function == "total") {
#ifndef MINGW32 //deactivate the sub total for mingw32 version
		cimg_usage("Spot detection and quantification");
		const char * file_i1 =
				cimg_option("-i1","ap-petit.TIFF","Input gel image");
		const char * file_odir =
				cimg_option("-odir","direct.TIFF","Output Direct Image");
		const char * file_oc =
				cimg_option("-oc","coule.TIFF","Output Bead Image");
		const char * file_op =
				cimg_option("-op","prob.TIFF","Output Prob Image");
		const char * file_odet =
				cimg_option("-odet","detect.TIFF","Output Detect Image");
		const char * file_o1 =
				cimg_option("-o1","numeros.TIFF","Image of spot numbers");
		const char * file_o2 =
				cimg_option("-o2","contours.TIFF","Image of contours");
		const char * file_paths =
				cimg_option("-opth","paths.TIFF","Bead paths");
		const char * file_confl =
				cimg_option("-ocfl","confluents.TIFF","Image of merging paths");
		const bool visu = cimg_option("-visu",false,"Visualization mode");
		const string meth =
				cimg_option("-methode","confluent","surface or merging paths");
		const bool svg_output = cimg_option("-svg",false,"SVG output");

		//cout << "inverse" << inverse << endl;
		QString source_filename(file_i1);
		QFileInfo source_fileinfo(source_filename);
		source_filename = source_fileinfo.baseName();

		// Init images
		// gestion image inversée: les programmes direct et quantif sont concus tels que
		// les hautes intensités sont au centre des spots.
		// si besoin on inverse avant analyse et apres analyse (ie dans contours)
		printf("Loading image\n");
		imageIntensity gel_image_loaded(file_i1);

		imageIntensity * p_gel_image;
		if (inverse) {
			printf("Inversing image intensity (negative image)\n");
			//gel_image.image_inverse();
			p_gel_image = gel_image_loaded.new_image_inversed();
			//delete (p_gel_image_loaded);
		} else {
			p_gel_image = &gel_image_loaded;
		}

		printf("Computing directions\n");
		imageDirect dest;
		const parameterDirect & parameter_direct =
				beads_params.get_parameter_direction();
		dest.compute_from_gel_image(*p_gel_image, parameter_direct);
		printf("Writing direction image\n");
		if (file_odir)
			dest.save(file_odir);
		// cimg_library::CImgList<unsigned short int> liste(img1,dest);
		if (visu)
			dest.display("Direction");
		printf("Beads are rolling...\n");
		imageCoule coule;
		coule.compute_from_direct(dest);
		//coule(dest, img1);
		// Save and exit: on fait pas de visu pour le moment
		printf("Writing image of beads\n");
		if (file_oc)
			coule.save(file_oc);

		// definition des maintenant de l'image des proba
		imageProb dest_image_prob;

		if (meth == "confluent") {
			printf("Computing bead paths\n");
			imagePaths parcours;
			parcours.compute_from_direct_and_coule(dest, coule);
			printf("Writing paths \n");
			if (file_paths)
				parcours.save(file_paths);

			cout << "Looking for merging paths" << endl;
			const parameterConfluent & parameter_confl =
					beads_params.get_parameter_confluent();

			cout << " minflux : " << parameter_confl.get_minflux() << endl;
			cout << " minpath : " << parameter_confl.get_minpath() << endl;
			cout << " winconfl : " << parameter_confl.get_winconfl() << endl;
			cout << " min number of beads : " << parameter_confl.get_minbeads()
					<< endl;
			cout << " max intensity for confluent detection : "
					<< parameter_confl.get_intmax() << endl;
			cout << " min percentage of spot for confluent detection : "
					<< parameter_confl.get_minpct() << endl;

			imageConfluent confluent;
			//			confluent.compute_from_direct_and_paths (dest, parcours, flux);
			confluent.compute_from_direct_and_paths(dest, parcours,
					*p_gel_image, parameter_confl);

			//confluent.dilate(5);

			printf("Writing merging positions and final positions \n");
			if (file_confl)
				confluent.save(file_confl);

			cout << "Probabilities of spot positions" << endl;
			const parameterProb & parameter_prob =
					beads_params.get_parameter_prob();
			cout << " threshold : " << parameter_prob.get_threshold() << endl;
			cout << " sx : " << parameter_prob.get_sx() << endl;
			cout << " sy : " << parameter_prob.get_sy() << endl;

			dest_image_prob.compute_from_image_intensity(confluent,
					parameter_prob);

		} else {
			cout << "Probabilities of spot positions" << endl;
			const parameterProb & parameter_prob =
					beads_params.get_parameter_prob();
			cout << " threshold : " << parameter_prob.get_threshold() << endl;
			cout << " sx : " << parameter_prob.get_sx() << endl;
			cout << " sy : " << parameter_prob.get_sy() << endl;

			//			imageProb dest_image_prob;

			dest_image_prob.compute_from_image_intensity(coule, parameter_prob);
		}
		// Save and exit
		printf("Writing image of probabilities \n");
		if (file_op)
			dest_image_prob.save(file_op);

		printf("Spot detection\n");
		const parameterDetection & parameter_detection =
				beads_params.get_parameter_detection();
		cout << " minproba : " << parameter_detection.get_minproba() << endl;

		printf("Creating spot list \n");
		detection the_detection_from_prob;
		the_detection_from_prob.detect_from_prob(dest_image_prob,
				parameter_detection);

		imageDetection img_detect(the_detection_from_prob);
		//the_detection.write_detection_image(img1);

		//detect (dest, img1, threshold);
		// Save and exit
		printf("Writing detection image \n");
		if (file_odet)
			img_detect.save(file_odet);

		printf("Quantification\n");
		const parameterNumber & parameter_number =
				beads_params.get_parameter_number();

		imageNumber numeros;
		numeros.compute_from_direct(dest, the_detection_from_prob,
				parameter_number);

		int npass = parameter_number.get_npass();
		numeros.expand_areas_down_the_slope(*p_gel_image, npass);

		//   		printf("Computing spot volume and background\n");
		the_detection_from_prob.quantify_spots_with_image(*p_gel_image, numeros);
		cout << "Writing image of spot numbers" << endl;
		if (file_o1)
			numeros.save(file_o1);

		cout << "Computing spot contours" << endl;
		imageContours contours;
		contours.compute_from_gel_image_and_numeros(*p_gel_image, numeros);

		contours.draw_contours(the_detection_from_prob, inverse);
		// Save
		if (file_o2) {
			printf("Writing original image with spot contours\n");
			if (inverse) {
				contours.save_inversed(file_o2);
			} else {
				contours.save(file_o2);
			}
		}

		/*****************************************************
		 //write results in several file formats:
		 * ****************************************************/
		// the PROTICdbML file contains all detected spots with quantitations in XML format
		// the text tabulated file contains spots coordinates and quantitations
		// the SVG output is a drawing of detected spots viewable with any SVG compatible software
		//  (example : http://www.inkscape.org)
		cout << "Writing PROTICdbML xml spot file" << endl;
		spotPROTICdbDocument proticdbml_file;
		proticdbml_file.set_gel_image_file_name(file_i1);
		proticdbml_file.open(source_filename);
		proticdbml_file.write_detection(the_detection_from_prob);
		proticdbml_file.close();

		// ecriture fichier de spots
		cout << "Writing text tabulated spot file" << endl;

		spotDocument fspot;
		fspot.open(source_filename);
		fspot.write_detection(the_detection_from_prob);
		fspot.close();

		if (svg_output) {
			cout << "Computing SVG file" << endl;
			the_detection_from_prob.store_spot_edges(numeros);
			spotSvgDocument svg_file;
			svg_file.set_gel_image_file_name(file_i1);
			//On peut créer et modifier les styles utilisés pour dessiner les contours ou les spots détectés ici:
			svg_file.set_css_class("detection",
					"fill:blue;fill-opacity:1;stroke:none");
			svg_file.set_css_class("contour",
					"stroke-linejoin:round;stroke:#000000;stroke-opacity:1;fill:none");

			svg_file.open(source_filename);
			svg_file.write_detection(the_detection_from_prob, "detection");
			svg_file.draw_spot_numbers(the_detection_from_prob, "detection");
			svg_file.write_detection(the_detection_from_prob, "quantif");
			svg_file.draw_contours(the_detection_from_prob, "contour");
			svg_file.close();
		}

		cout << "Done" << endl;
#endif
	}

	else if (sub_function == "denovo") {
		imageDeNovo image;
		QString output_filename =
				cimg_option("-o","denovo.tiff","output DeNovo image file");
		const QString file_denovo_spots =
				cimg_option("-denovospots","denovo.txt","Parameter file");

		if (image.build_image_with_spot_file(file_denovo_spots)) {
			cout
					<< "create an artificial gel image with spots described in file : "
					<< file_denovo_spots.toStdString() << endl;
		} else {
			cerr
					<< "please use argument -denovospots to provide a de novo spot file."
					<< endl;
		}
		//if (inverse) {
		image.save_inversed(output_filename);
		//}
		//else {
		//	image.save(output_filename.c_str());
		//}
	} else {
		// no sub function : detect spots and write ouput
		QString input_filename = cimg_option("-i","","input gel image file");
		QString output_filename =
				cimg_option("-o","output.xml","output data file");

		if (input_filename == "") {
			cerr << "please use argument -i to provide a gel image file."
					<< endl;
			return 1;
		}

		QString source_filename(input_filename);
		QFileInfo source_fileinfo(source_filename);

		QFileInfo output_fileinfo(output_filename);

		source_filename = source_fileinfo.baseName();

		QString output_extension = output_fileinfo.suffix();

		qDebug() << "main output_extension : " << output_extension;

		cout << "Loading input image file " << input_filename.toStdString()
				<< endl;
		imageIntensity gel_image_loaded(input_filename);

		imageIntensity * p_gel_image;
		if (inverse) {
			printf("Inversing image intensity (negative image)\n");
			//gel_image.image_inverse();
			p_gel_image = gel_image_loaded.new_image_inversed();
			//delete (p_gel_image_loaded);
		} else {
			p_gel_image = &gel_image_loaded;
		}

		QString tmp_filename;
		if (save_steps) {
			tmp_filename = source_filename + "_working_on.tif";
			cout << "saving working gel image in "
					<< tmp_filename.toStdString() << endl;
			p_gel_image->save(tmp_filename);
		}

		cout << "Computing bead directions " << endl;
		imageDirect directions;
		const parameterDirect & parameter_direct =
				beads_params.get_parameter_direction();
		directions.compute_from_gel_image(*p_gel_image, parameter_direct);

		if (save_steps) {
			tmp_filename = source_filename + "_DIRECTIONS.png";
			cout << "saving directions image in " << tmp_filename.toStdString()
					<< endl;
			directions.save(tmp_filename);
		}

		cout << "Beads are rolling" << endl;
		imageCoule rolling;
		rolling.compute_from_direct(directions);

		if (save_steps) {
			tmp_filename = source_filename + "_BEADS.png";
			rolling.save(tmp_filename);
		}

		//using confluent method :
		cout << "Computing bead paths" << endl;
		imagePaths path;
		path.compute_from_direct_and_coule(directions, rolling);

		if (save_steps) {
			tmp_filename = source_filename + "_PATHS.png";
			cout << "saving path image in " << tmp_filename.toStdString()
					<< endl;
			path.save(tmp_filename);
		}

		cout << "Detecting merging paths and arrival positions" << endl;
		const parameterConfluent & parameter_confl =
				beads_params.get_parameter_confluent();
		cout << " minflux : " << parameter_confl.get_minflux() << endl;
		cout << " minpath : " << parameter_confl.get_minpath() << endl;
		cout << " winconfl : " << parameter_confl.get_winconfl() << endl;
		cout << " minbeads : " << parameter_confl.get_minbeads() << endl;

		imageConfluent confluent;
		confluent.compute_from_direct_and_paths(directions, path, *p_gel_image,
				parameter_confl);

		if (save_steps) {
			tmp_filename = source_filename + "_SELECT.png";
			cout << "saving confluent image in " << tmp_filename.toStdString()
					<< endl;
			confluent.save(tmp_filename);
		}

		cout << "Computing probabilites of spot positions" << endl;
		const parameterProb & parameter_prob =
				beads_params.get_parameter_prob();
		cout << " threshold : " << parameter_prob.get_threshold() << endl;
		cout << " sx : " << parameter_prob.get_sx() << endl;
		cout << " sy : " << parameter_prob.get_sy() << endl;

		imageProb dest_image_prob;

		dest_image_prob.compute_from_image_intensity(confluent, parameter_prob);

		if (save_steps) {
			tmp_filename = source_filename + "_PROBABILITIES.png";
			cout << "saving prob image in " << tmp_filename.toStdString()
					<< endl;
			dest_image_prob.save(tmp_filename);
		}

		cout << "Detecting spots" << endl;
		const parameterDetection & parameter_detection =
				beads_params.get_parameter_detection();
		cout << " minproba : " << parameter_detection.get_minproba() << endl;

		detection the_detection_from_prob;
		the_detection_from_prob.detect_from_prob(dest_image_prob,
				parameter_detection);

		//imageDetection img_detect(the_detection_from_prob);

		cout << "Quantification" << endl;
		const parameterNumber & parameter_number =
				beads_params.get_parameter_number();

		imageNumber numbers;
		//quantification des spots:
		numbers.compute_from_direct(directions, the_detection_from_prob,
				parameter_number);

		int npass = parameter_number.get_npass();
		numbers.expand_areas_down_the_slope(*p_gel_image, npass);

		if (save_steps) {
			tmp_filename = source_filename + "_NUMBERS.png";
			cout << "saving numbers image in " << tmp_filename.toStdString()
					<< endl;
			numbers.save(tmp_filename);
		}

		/*****************************************************
		 //write results
		 * ****************************************************/
		if (output_extension == "xml") {
			the_detection_from_prob.quantify_spots_with_image(*p_gel_image,
					numbers);
			// the PROTICdbML file contains all detected spots with quantitations in XML format
			// the text tabulated file contains spots coordinates and quantitations
			// the SVG output is a drawing of detected spots viewable with any SVG compatible software
			//  (example : http://www.inkscape.org)
			cout << "Writing PROTICdbML xml spot file "
					<< output_filename.toStdString() << endl;
			spotPROTICdbDocument proticdbml_file;
			proticdbml_file.set_gel_image_file_name(input_filename);
			proticdbml_file.open(output_filename);
			proticdbml_file.write_detection(the_detection_from_prob);
			proticdbml_file.close();
		}
		if (output_extension == "txt") {
			the_detection_from_prob.quantify_spots_with_image(*p_gel_image,
					numbers);
			cout << "Writing text tabulated spot file "
					<< output_filename.toStdString() << endl;

			spotDocument fspot;
			fspot.open(output_filename);
			fspot.write_detection(the_detection_from_prob);
			fspot.close();
		}
		if (output_extension == "jpg") {
			imageContours contours;
			contours.compute_from_gel_image_and_numeros(*p_gel_image, numbers);
			contours.draw_contours(the_detection_from_prob, inverse);
			cout << "Writing jpeg file of spot contours"
					<< output_filename.toStdString() << endl;
			cout << "Writing jpeg file " << output_filename.toStdString()
					<< endl;
			contours.save_inversed(output_filename);
		}
		if (output_extension == "jpg2") {
			//draw spot barycenters instead of detected spot centers
			imageContours contours;
			contours.compute_from_gel_image_and_numeros(*p_gel_image, numbers);
			the_detection_from_prob.quantify_spots_with_image(*p_gel_image,
					numbers);
			contours.draw_contours(the_detection_from_prob, inverse);
			cout << "Writing jpeg file of spot contours"
					<< output_filename.toStdString() << endl;
			cout << "Writing jpeg file " << output_filename.toStdString()
					<< endl;
			contours.save_inversed(output_filename);
		}
		if ((output_extension == "tiff") || (output_extension == "tif")) {
			imageContours contours;
			contours.compute_from_gel_image_and_numeros(*p_gel_image, numbers);
			contours.draw_contours(the_detection_from_prob, inverse);
			cout << "Write tiff file of spot contours"
					<< output_filename.toStdString() << endl;
			cout << "Writing tiff file " << output_filename.toStdString()
					<< endl;
			contours.save_inversed(output_filename);
		}
		if (output_extension == "svg") {
			cout << "Writing SVG file of spot contours"
					<< output_filename.toStdString() << endl;
			the_detection_from_prob.store_spot_edges(numbers);
			spotSvgDocument svg_file;
			svg_file.set_gel_image_file_name(input_filename);
			svg_file.set_css_class("detection",
					"fill:blue;fill-opacity:1;stroke:none");
			svg_file.set_css_class("contour",
					"stroke-linejoin:round;stroke:#000000;stroke-opacity:1;fill:none");

			svg_file.embed_gel_image(gel_image_loaded);
			//svg_file.embed_gel_image();

			svg_file.open(output_filename);
			svg_file.write_detection(the_detection_from_prob, "detection");
			svg_file.draw_spot_numbers(the_detection_from_prob, "detection");

			the_detection_from_prob.quantify_spots_with_image(*p_gel_image,
					numbers);
			svg_file.write_detection(the_detection_from_prob, "quantif");
			svg_file.draw_contours(the_detection_from_prob, "contour");

			svg_file.close();
		}
	}
	return 0;
}


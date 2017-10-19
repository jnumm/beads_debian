#ifndef IMAGECONFLUENT_H_
#define IMAGECONFLUENT_H_


#include "imageIntensity.h"
#include "imageDirect.h"
#include "imagePaths.h"
#include "parameterConfluent.h"


//Identification des confluents
class imageConfluent : public imageIntensity {
public:
	imageConfluent(const char * & filename) : imageIntensity(filename){
	};
	imageConfluent() {
	};
	~imageConfluent() {
	};

	void compute_from_direct_and_paths (const imageDirect & direct, const imagePaths & paths, const imageIntensity & gel_image, const parameterConfluent & parameter_confl);

private:
};

#endif /*IMAGECONFLUENTS_H_*/

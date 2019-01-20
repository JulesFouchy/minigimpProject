#ifndef IMAGE_TRANSFORMATIONS_H__
	#define IMAGE_TRANSFORMATIONS_H__

	#include "image.h"
	#include "colorSpaces.h"

	#ifndef COLOR_CODES__
		#define COLOR_CODES__
		#define RED 0
		#define GREEN 1
		#define BLUE 2
	#endif



	void changeLuminosity( Image* im , float param ) ;

	void changeContrast( Image* im , float param ) ;

	void invertColours( Image* im ) ;

	void cubicMitigateColourChannel( Image* im , int colour , float coefXcube , float coefX ) ;

	void sepia( Image* im , float redCoefXcube , float redCoefX , float greenCoefXcube , float greenCoefX , float blueCoefXcube , float blueCoefX ) ;

	void darkenTheBlacks( Image* im , float power , float factor ) ;

	void brightenTheWhites( Image* im , float power , float factor ) ;

	void convertToGrayScale( Image* im ) ;

	void changeSaturation( Image* im , float percentage ) ;

	void verticalMirror( Image* im , int dir ) ;

	float map( float x , float a , float b , float newA , float newB ) ;

	void vignetting( Image* im , float whRatio , float maxValue , float innerRadiusRatio ) ;

	void applyConvolution( Image** imPtrPtr , float kernel[] , int kernelSize ) ;

	void blur( Image **im , int kernelSize ) ;

	void contrastViaConvolution( Image **im ) ;

	void sharpen( Image **im , float x ) ;

	void edges( Image **im , float x ) ;

	RGBcolour temperatureColour( float Temperature ) ;

	void changeColourTemperature( Image *im , float temperature , float alpha ) ;

#endif
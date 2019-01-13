#ifndef IMAGE_H__
	#define IMAGE_H__
	#include "image.h"
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

void vignetting( Image* im , float horizontalRadius , float verticalRadius ) ;

void applyConvolution( Image** imPtrPtr , float kernel[] , int kernelSize ) ;
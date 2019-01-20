#ifndef IMAGE_TRANSFORMATIONS_H__
	#define IMAGE_TRANSFORMATIONS_H__

	#include "image.h"
	#include "colorSpaces.h"
	#include "histogram.h"

	#ifndef COLOR_CODES__
		#define COLOR_CODES__
		#define RED 0
		#define GREEN 1
		#define BLUE 2
	#endif



void changeLuminosity( Image* im , float param ) ;

void changeContrast( Image* im , float param ) ;

void invertColours( Image* im ) ;

void cubicTransformOfColourChannel( Image* im , int colour , float coefXcube , float coefX ) ;

void changeColourChannels( Image* im , float redCoefXcube , float redCoefX , float greenCoefXcube , float greenCoefX , float blueCoefXcube , float blueCoefX ) ;

void affectOnlyBlacks( Image* im , float power , float factor , float dir ) ;

void affectOnlyWhites( Image* im , float power , float factor , float dir ) ;

void convertToGrayScale( Image* im ) ;

void changeSaturation( Image* im , float percentage ) ;

void verticalMirror( Image* im , int dir ) ;

void vignetting( Image* im , float whRatio , float gradientSpeed , float innerRadiusRatio , float centerXratio , float centerYratio ) ;

void applyConvolution( Image** imPtrPtr , float kernel[] , int kernelSize ) ;

void blur( Image **im , int kernelSize ) ;

void blurEllipticGradient( Image** im , float whRatio , float gradientSpeed , float innerRadiusRatio , float centerXratio , float centerYratio ) ;

void contrastViaConvolution( Image **im ) ;

void sharpen( Image **im , float x ) ;

void edges( Image **im , float x ) ;

int toOddInteger( float x ) ;

RGBcolour temperatureColour( float Temperature ) ;

void changeColourTemperature( Image *im , float temperature , float alpha ) ;

void threshhold( Image* im , int R0 , int G0 , int B0 , int R1 , int G1 , int B1 , int threshhold ) ;

void threshholdUsingMedian( Image* im , int R0 , int G0 , int B0 , int R1 , int G1 , int B1 ) ;

#endif
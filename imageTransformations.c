#ifndef IMAGE_TRANSFORMATIONS_H__
	#define IMAGE_TRANSFORMATIONS_H__
	#include "imageTransformations.h"
#endif

#include <math.h>
#define PI 3.141592653589793238462643

#define RED 0
#define GREEN 1
#define BLUE 2

void changeLuminosity( Image* im , float param ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			im->luts[c][k] += param ;
		}
	}
}

void changeContrast( Image* im , float param ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			int currentValue = im->luts[c][k] ;
			int nextValue = currentValue + sin( currentValue*2*PI/255 - PI )*param ;
			im->luts[c][k] = nextValue ;
		}
	}
}

void invertColours( Image* im ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){			
			im->luts[c][k] = 255 - im->luts[c][k] ;
		}
	}
}

//Decreases the values of the color channel, with strongest effect on intermediate values and no effect on pure black and pure white.
//I would suggest you go there to see the curve and play around with the coefficients : https://www.geogebra.org/classic/yfggz7nu
//See the report for more details on the chosen function.
void cubicMitigateColourChannel( Image* im , int colour , float coefXcube , float coefX ){
	for( int k = 0 ; k < 256 ; ++k ){
		float x = im->luts[colour][k] ;
		im->luts[colour][k] = ( coefXcube * pow(x/255,3) + coefX * x/255 ) * 255 / ( coefXcube + coefX ) ;
	}
}

void sepia( Image* im , float redCoefXcube , float redCoefX , float greenCoefXcube , float greenCoefX , float blueCoefXcube , float blueCoefX ){
	convertToGrayScale( im ) ;
	cubicMitigateColourChannel( im , RED , redCoefXcube , redCoefX ) ;
	cubicMitigateColourChannel( im , GREEN , greenCoefXcube , greenCoefX ) ;
	cubicMitigateColourChannel( im , BLUE  , blueCoefXcube , blueCoefX ) ;
}

void darkenTheBlacks( Image* im , float power , float factor ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			int currentValue = im->luts[c][k] ;
			int nextValue = currentValue * (1 - exp(-factor*pow((float)currentValue/255,power) ) ) ;
			im->luts[c][k] = nextValue ;
		}
	}
}

void brightenTheWhites( Image* im , float power , float factor ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			int currentValue = im->luts[c][k] ;
			int nextValue = currentValue * (1 + exp(-factor*pow((255-(double)currentValue)/255,power) ) ) ;
			im->luts[c][k] = nextValue ;
		}
	}
}

//Using values found here : https://en.wikipedia.org/wiki/Grayscale#Colorimetric_(perceptual_luminance-preserving)_conversion_to_grayscale
void convertToGrayScale( Image* im ){
	for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		int grayValue = 0.2126 * getPixel( im , i , j , 0 ) + 0.7152 * getPixel( im , i , j , 1 ) + 0.0722 * getPixel( im , i , j , 2 ) ;
    		for( int c = 0 ; c < 3 ; ++c ){
    			setPixel( im , i , j , c , grayValue ) ;
    		}
    	}
    }
}

//Takes a barycenter of the pixel colour and it's grayscale conversion.
//The "percentage" is the barycentric weight given to the colour, so 0 will convert it to grayscale, 1 has no effect, and values greater than 1 will increase saturation.
void changeSaturation( Image* im , float percentage ){
	for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		int grayValue = 0.2126 * getPixel( im , i , j , 0 ) + 0.7152 * getPixel( im , i , j , 1 ) + 0.0722 * getPixel( im , i , j , 2 ) ;
    		for( int c = 0 ; c < 3 ; ++c ){
    			setPixel( im , i , j , c , toUnsignedChar( (1-percentage)*grayValue + percentage*getPixel(im,i,j,c) ) ) ;
    		}
    	}
    }
}

#ifndef IMAGE_TRANSFORMATIONS_H__
	#define IMAGE_TRANSFORMATIONS_H__
	#include "imageTransformations.h"
#endif

#include <stdlib.h>
#include <stdio.h>

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

// dir is either 1 or -1 and indicates if the right part is copied onto the left one (1) or vice-versa (-1)
void verticalMirror( Image* im , int dir ){
	for( int i = 0 ; i < im->height ; ++i ){
		for( int j = 0 ; j < im->width/2 ; ++j ){
			int actualJ = j*dir + (im->width-1)*(1-dir)/2 ; // either j or width-1-j depending on the value of dir
			for( int c = 0 ; c < 3 ; ++c ){
				setPixel( im , i , actualJ , c , getPixel( im , i , im->width-1-actualJ , c ) ) ;
			}
		}
	}
}

//Applies to x the linear function that maps the interval [a,b] into [newA,newB].
float map( float x , float a , float b , float newA , float newB ){
	return (x-b)*newA/(a-b) + (x-a)*newB/(b-a) ;
}

void vignetting( Image* im , float whRatio , float maxValue , float innerRadiusRatio ){
	for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		float dist = sqrt( pow((j-im->width/2),2) + pow((i-im->height/2)*whRatio,2) )  ;
    		if( dist > innerRadiusRatio*im->width ){
	    		for( int c = 0 ; c < 3 ; ++c ){
	    			setPixel( im , i , j , c , toUnsignedChar(getPixel(im,i,j,c)-map(dist,innerRadiusRatio*im->width,im->width,0,maxValue)) ) ;
	    		}
	    	}
    	}
    }
}

void applyConvolution( Image** im , float kernel[] , int kernelSize ){
	//Create the new image
	Image* newImPtr = malloc(sizeof(Image) + ((*im)->width)*((*im)->height)*3*sizeof(unsigned char)) ;
	newImPtr->width = (*im)->width ;
	newImPtr->height = (*im)->height ;
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			newImPtr->luts[c][k] = (*im)->luts[c][k] ;
		}
	}
	//Loop over the pixels
	for( int i = 0 ; i < (*im)->height ; ++i ){
    	for( int j = 0 ; j < (*im)->width ; ++j ) {
    		for( int c = 0 ; c < 3 ; ++c ){
    			//Loop over the neighbours of the pixel
    			float newValue = 0 ;
    			for( int i_ = 0 ; i_ < kernelSize ; ++i_ ){
    				for( int j_ = 0 ; j_ < kernelSize ; ++j_ ){
    					int neighbourI =  i + i_ - kernelSize/2 ;
    					int neighbourJ =  j + j_ - kernelSize/2 ;
    					if( neighbourI >= 0 && neighbourI < (*im)->height && neighbourJ >= 0 && neighbourJ < (*im)->width ){
    						newValue += kernel[j_+i_*kernelSize]*getPixel( *im , neighbourI , neighbourJ , c ) ;
    					}
    				}
    			}
    			setPixel( newImPtr , i , j , c , toUnsignedChar(newValue) ) ;
    		}
    	}
    }
	*im = newImPtr ;
}
#include <stdlib.h>
#include <math.h>

#include "histogram.h"

#define PI 3.141592653589793238462643

void calculateHistogram( Image *im ){
	//Initialize
	for( int k = 0 ; k < 256 ; k++ ){
		for( int c = 0 ; c < 3 ; c++ ){
			im->histos[c][k] = 0 ;
		}
	}
	//Calculate
	for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		for( int c = 0 ; c < 3 ; ++c ){
    			im->histos[c][getPixel( im , i , j , c )] += 1 ;
    		}
    	}
    }
}

void calculateAverageHistogram( Image *im ){
	//Initialize
	for( int k = 0 ; k < 256 ; k++ ){
		im->avgHisto[k] = 0 ;
	}
	//Calculate
	for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		int grayValue = 0.2126 * getPixel( im , i , j , 0 ) + 0.7152 * getPixel( im , i , j , 1 ) + 0.0722 * getPixel( im , i , j , 2 ) ;
    		im->avgHisto[grayValue] += 1 ;
    	}
    }
}

int maxOfHistos( int histos[3][256] ){
	int max = 0 ;
	for( int k = 0 ; k < 256 ; k++ ){
		for( int c = 0 ; c < 3 ; c++ ){
			if( histos[c][k] > max ){
				max = histos[c][k] ;
			}
		}
	}
	return max ;
}

int roundfl( float x ){
	if( x - (int) x < 0.5){
		return (int) x ;
	}
	else{
		return (int) x + 1 ;
	}
}

/*void saveHistogramBaton( Image *im , int width , int height ){
	Image* histo = newImage( width , height ) ;
	int max = maxOfHistos( im->histos ) ;
	float jScaleCoef = 255.0 / ( width - 1 ) ;
	float iScaleCoef = ( max - 1 ) / ( (float) height - 1 ) ;

	for( int j = 0 ; j < histo->width ; ++j ) {
    	int pixelValue = roundfl(j * jScaleCoef) ;
		for( int i = 0 ; i < histo->height ; ++i ){
    		for( int c = 0 ; c < 3 ; ++c ){
    			if( (height - i) * iScaleCoef < im->histos[c][pixelValue] ){
    				setPixel( histo , i , j , c , 255 ) ;
    			}
    			else{
    				setPixel( histo , i , j , c , 0 ) ;
    			}
    		}
    	}
    }

    saveImagePPM( "histogram.ppm" , histo ) ;

    free( histo ) ;
}*/

void saveHistogram( Image *im , int width , int height ){
	Image* histo = newImage( width , height ) ;
	int max = maxOfHistos( im->histos ) ;
	float jScaleCoef = 255.0 / ( width - 1 ) ;
	float iScaleCoef = ( max - 1 ) / ( (float) height - 1 ) ;

	for( int j = 0 ; j < histo->width ; ++j ) {
    	int pixelValue = j * jScaleCoef ;
    	float percentage = j * jScaleCoef - pixelValue ;
		for( int i = 0 ; i < histo->height ; ++i ){
    		for( int c = 0 ; c < 3 ; ++c ){
    			if( (height - i) * iScaleCoef < im->histos[c][pixelValue]*(1-percentage) + im->histos[c][pixelValue+1]*percentage ){
    				setPixel( histo , i , j , c , 255 ) ;
    			}
    			else{
    				setPixel( histo , i , j , c , 0 ) ;
    			}
    		}
    	}
    }

    saveImagePPM( "histogram.ppm" , histo ) ;

    free( histo ) ;
}

int calculateMedian( Image* im ){
	calculateAverageHistogram( im ) ;
	int nbPixels = im->width * im->height ;
	int pixelCount = 0 ;
	int medianPixel = 0 ;
	while( pixelCount < nbPixels/2 ){
		pixelCount += im->avgHisto[medianPixel] ;
		medianPixel += 1 ;
	}
	return medianPixel ;
}
#include <stdlib.h>
#include <math.h>

#include "imageTransformations.h"

#define PI 3.141592653589793238462643

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

//Changes the values of the colour channel, with strongest effect on intermediate values and no effect on 0 and 255.
//I would suggest you go there to see the curve and play around with the coefficients : https://www.geogebra.org/classic/yfggz7nu
//See the report for more details on the chosen function.
void cubicTransformOfColourChannel( Image* im , int colour , float coefXcube , float coefX ){
	for( int k = 0 ; k < 256 ; ++k ){
		float x = im->luts[colour][k] ;
		im->luts[colour][k] = ( coefXcube * pow(x/255,3) + coefX * x/255 ) * 255 / ( coefXcube + coefX ) ;
	}
}

void changeColourChannels( Image* im , float redCoefXcube , float redCoefX , float greenCoefXcube , float greenCoefX , float blueCoefXcube , float blueCoefX ){
	convertToGrayScale( im ) ;
	cubicTransformOfColourChannel( im , RED , redCoefXcube , redCoefX ) ;
	cubicTransformOfColourChannel( im , GREEN , greenCoefXcube , greenCoefX ) ;
	cubicTransformOfColourChannel( im , BLUE  , blueCoefXcube , blueCoefX ) ;
}

//dir is 1 or -1 and indicates if we increase or decrease the blacks
void affectOnlyBlacks( Image* im , float power , float factor , float dir ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			int currentValue = im->luts[c][k] ;
			//is is very important to check that currentValue>0 beacause the exponential function we use goes wild for x<0
			int nextValue = currentValue<0 ? currentValue : currentValue + currentValue*dir*exp(-factor*pow(currentValue/255.0,power) ) ;
			im->luts[c][k] = nextValue ;
		}
	}
}

//dir is 1 or -1 and indicates if we increase or decrease the whites
void affectOnlyWhites( Image* im , float power , float factor , float dir ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			int currentValue = im->luts[c][k] ;
			//is is very important to check that currentValue<255 beacause the exponential function we use goes wild for x>255
			int nextValue = currentValue>255 ? currentValue : currentValue + (255-currentValue)*dir*exp(-factor*pow((255-currentValue)/255.0,power) ) ;
			im->luts[c][k] = nextValue ;
		}
	}
}

//Using values found here : https://en.wikipedia.org/wiki/Grayscale#colourimetric_(perceptual_luminance-preserving)_conversion_to_grayscale
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

void vignetting( Image* im , float whRatio , float gradientSpeed , float innerRadiusRatio , float centerXratio , float centerYratio ){
	for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		//Calculate euclidian distance but with y-axis stretched by whRatio to transform circles into more general ellipses
    		float dist = sqrt( pow((j-im->width*centerXratio),2) + pow((i-im->height*centerYratio)*whRatio,2) )  ;
    		if( dist > innerRadiusRatio*im->width ){
	    		for( int c = 0 ; c < 3 ; ++c ){
	    			setPixel( im , i , j , c , toUnsignedChar(getPixel(im,i,j,c) - gradientSpeed*(dist-innerRadiusRatio*im->width) ) ) ;
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
    //Point towards the new image
	*im = newImPtr ;
}

void blur( Image **im , int kernelSize ){
	float x = 1.0/kernelSize/kernelSize ;
	float kernel[kernelSize*kernelSize] ;
	for( int l = 0 ; l < kernelSize*kernelSize ; ++l ){
		kernel[l] = x ;
	}
	applyConvolution( im , kernel , kernelSize ) ;
}

void blurEllipticGradient( Image** im , float whRatio , float gradientSpeed , float innerRadiusRatio , float centerXratio , float centerYratio ){
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
	    		float dist = sqrt( pow((j-(*im)->width*centerXratio),2) + pow((i-(*im)->height*centerYratio)*whRatio,2) ) ;
	    		if( dist > innerRadiusRatio * (*im)->width ){
	    			int kernelSize = toOddInteger( gradientSpeed*( dist - innerRadiusRatio * (*im)->width ) ) ;
	    	//Loop over the neighbours of the pixel
	    			float newValue = 0 ;
	    			for( int i_ = 0 ; i_ < kernelSize ; ++i_ ){
	    				for( int j_ = 0 ; j_ < kernelSize ; ++j_ ){
	    					int neighbourI =  i + i_ - kernelSize/2 ;
	    					int neighbourJ =  j + j_ - kernelSize/2 ;
	    					if( neighbourI >= 0 && neighbourI < (*im)->height && neighbourJ >= 0 && neighbourJ < (*im)->width ){
	    						newValue += 1.0/kernelSize/kernelSize * getPixel( *im , neighbourI , neighbourJ , c ) ;
	    					}
	    				}
	    			}
	    			setPixel( newImPtr , i , j , c , toUnsignedChar(newValue) ) ;
	    		}
		    	else{
		    		setPixel( newImPtr , i , j , c , getPixel( *im , i , j , c ) ) ;
		    	}
		    }
    	}
    }
    //Point towards the new image
    *im = newImPtr ;
}


void contrastViaConvolution( Image **im ){
	int kernelSize = 3 ;
	float x = 0.37 ;
	float kernel[kernelSize*kernelSize] ;
	for( int i = 0 ; i < kernelSize ; ++i ){
		for( int j = 0 ; j < kernelSize ; ++j ){
			if( j== 1){
				if( i==1 ){
					kernel[j+i*kernelSize] = 5*x ;
				}
				else{
					kernel[j+i*kernelSize] = -x ;
				}
			}
			else{
				kernel[j+i*kernelSize] = 0 ;
			}
		}
	}
	applyConvolution( im , kernel , kernelSize ) ;
}

void sharpen( Image **im , float x ){
	int kernelSize = 3 ;
	float kernel[kernelSize*kernelSize] ;
	for( int i = 0 ; i < kernelSize ; ++i ){
		for( int j = 0 ; j < kernelSize ; ++j ){
			if( j== 1 || i==1 ){
				if( j==1 && i==1 ){
					kernel[j+i*kernelSize] = 4*x+1 ;
				}
				else{
					kernel[j+i*kernelSize] = -x ;
				}
			}
			else{
				kernel[j+i*kernelSize] = 0 ;
			}
		}
	}
	applyConvolution( im , kernel , kernelSize ) ;
}

void edges( Image **im , float x ){
	int kernelSize = 3 ;
	float kernel[kernelSize*kernelSize] ;
	for( int i = 0 ; i < kernelSize ; ++i ){
		for( int j = 0 ; j < kernelSize ; ++j ){
			if( j== 1 && i==1 ){
				kernel[j+i*kernelSize] = 8*x ;
			}
			else{
				kernel[j+i*kernelSize] = -x ;
			}
		}
	}
	applyConvolution( im , kernel , kernelSize ) ;
}

//maps [0,2[ to 1, [2,4[ to 3 and so on
int toOddInteger( float x ){
	int tmp = x/2 ;
	return 2*tmp + 1 ;
}

//Algorithm found here : http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
RGBcolour temperatureColour( float Temperature ){
	RGBcolour colour ;
	Temperature /= 100 ;

	//Red
	if( Temperature <= 66 ){
        colour.R = 255 ;
	}
    else{
        colour.R = Temperature - 60 ;
        colour.R = 329.698727446 * pow(colour.R , -0.1332047592) ;
        if( colour.R < 0 ){
        	colour.R = 0 ;
        }
        if( colour.R > 255 ){
        	colour.R = 255 ;
        }
    }

    //Green
    if( Temperature <= 66 ){
        colour.G = Temperature ;
        colour.G = 99.4708025861 * log(colour.G) - 161.1195681661 ;
        if( colour.G < 0 ){
        	colour.G = 0 ;
        }
        if( colour.G > 255 ){
        	colour.G = 255 ;
        }
	}
    else{
        colour.G = Temperature - 60 ;
        colour.G = 288.1221695283 * pow(colour.G , -0.0755148492) ;
        if( colour.G < 0 ){
        	colour.G = 0 ;
        }
        if( colour.G > 255 ){
        	colour.G = 255 ;
        }
    }

    //Blue
    if( Temperature >= 66 ){
        colour.B = 255 ;
	}
    else{
    	if( Temperature <= 19 ){
    		colour.B = 0 ;
    	}
    	else{
	        colour.B = Temperature - 10 ;
	        colour.B = 138.5177312231 * log(colour.B) - 305.0447927307 ;
	        if( colour.B < 0 ){
	        	colour.B = 0 ;
	        }
	        if( colour.B > 255 ){
	        	colour.B = 255 ;
        	}
        }
    }
    return colour ;
}

void changeColourTemperature( Image *im , float temperature , float alpha ){
	RGBcolour tempColour = temperatureColour( temperature ) ;
	for( int i = 0 ; i < im->height ; ++i ){
		for( int j = 0 ; j < im->width ; ++j ){
			//Get RGB value of the pixel
			RGBcolour pixelColour ;
			pixelColour.R = getPixel( im , i , j , RED ) ;
			pixelColour.G = getPixel( im , i , j , GREEN ) ;
			pixelColour.B = getPixel( im , i , j , BLUE ) ;
			//Save lightness
			float V = HSVfromRGB( pixelColour ).V ;
			//Blend pixel colour with temperatur colour
			pixelColour.R = alpha*tempColour.R + (1-alpha)*pixelColour.R ;
			pixelColour.G = alpha*tempColour.G + (1-alpha)*pixelColour.G ;
			pixelColour.B = alpha*tempColour.B + (1-alpha)*pixelColour.B ;
			//Convert to HSV, put former lightness back, and convert back to RGB
			HSVcolour hsvColour = HSVfromRGB( pixelColour ) ;
			hsvColour.V = V ;
			pixelColour = RGBfromHSV( hsvColour ) ; 
			//Set changes
			setPixel( im , i , j , RED , toUnsignedChar(pixelColour.R) ) ;
			setPixel( im , i , j , GREEN , toUnsignedChar(pixelColour.G) ) ;
			setPixel( im , i , j , BLUE , toUnsignedChar(pixelColour.B) ) ;
		}
	}
}

void threshhold( Image* im , int R0 , int G0 , int B0 , int R1 , int G1 , int B1 , int threshhold ){
	for( int i = 0 ; i < im->height ; ++i ){
		for( int j = 0 ; j < im->width ; ++j ){
			int grayValue =  0.2126 * getPixel( im , i , j , 0 ) + 0.7152 * getPixel( im , i , j , 1 ) + 0.0722 * getPixel( im , i , j , 2 ) ;
			if( grayValue < threshhold ){
				setPixel( im , i , j , 0 , R0 ) ;
				setPixel( im , i , j , 1 , G0 ) ;
				setPixel( im , i , j , 2 , B0 ) ;
			}
			else{
				setPixel( im , i , j , 0 , R1 ) ;
				setPixel( im , i , j , 1 , G1 ) ;
				setPixel( im , i , j , 2 , B1 ) ;
			}
		}
	}
}

void threshholdUsingMedian( Image* im , int R0 , int G0 , int B0 , int R1 , int G1 , int B1 ){
	int median = calculateMedian( im ) ;
	for( int i = 0 ; i < im->height ; ++i ){
		for( int j = 0 ; j < im->width ; ++j ){
			int grayValue =  0.2126 * getPixel( im , i , j , 0 ) + 0.7152 * getPixel( im , i , j , 1 ) + 0.0722 * getPixel( im , i , j , 2 ) ;
			if( grayValue < median ){
				setPixel( im , i , j , 0 , R0 ) ;
				setPixel( im , i , j , 1 , G0 ) ;
				setPixel( im , i , j , 2 , B0 ) ;
			}
			else{
				setPixel( im , i , j , 0 , R1 ) ;
				setPixel( im , i , j , 1 , G1 ) ;
				setPixel( im , i , j , 2 , B1 ) ;
			}
		}
	}
}
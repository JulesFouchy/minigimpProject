#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imageTransformations.h"


int main( int argc , char* argv[] ){

	Image *im = loadImagePPM( argv[1] );

	for( int k = 2 ; k < argc ; ++k ){
		if( strcmp( argv[k] , "ADDLUM" ) == 0 ){
			changeLuminosity( im , atoll(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "DIMLUM" ) == 0 ){
			changeLuminosity( im , - atoll(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "ADDCON" ) == 0 ){
			// 10 is a reasonnable parameter
			changeContrast( im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "DIMCON" ) == 0 ){
			changeContrast( im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "INVERT" ) == 0 ){
			invertColours( im ) ;
		}
		else if( strcmp( argv[k] , "SEPIA" ) == 0 ){
			//This choice of parameters gives good results
			sepia( im , -0.2 , 1.4 , 1 , 1.4 , 5 , 1.4 ) ;
		}
		else if( strcmp( argv[k] , "SEPIAARGS" ) == 0 ){
			//Or you can choose your own parameters here
			sepia( im , atof(argv[k+1]) , atof(argv[k+2]) , atof(argv[k+3]) , atof(argv[k+4]) , atof(argv[k+5]) , atof(argv[k+6]) ) ;
			k+=6 ;
		}
		else if( strcmp( argv[k] , "BLACKS" ) == 0 ){
			// power=1 and factor=7 seem to be decent parameters
			darkenTheBlacks( im , atof(argv[k+1]) , atof(argv[k+2]) ) ;
			k+=2 ;
		}
		else if( strcmp( argv[k] , "WHITES" ) == 0 ){
			// power=1 and factor=7 seem to be decent parameters
			brightenTheWhites( im , atof(argv[k+1]) , atof(argv[k+2]) ) ;
			k+=2 ;
		}
		/* NB : the following functions directly modify the image,
		 * they do not make use of LUTs because they need at the same time the Red, Green and Blue information of the pixel.
		 */

	
		else if( strcmp( argv[k] , "GRAYSCALE" ) == 0 ){
			convertToGrayScale( im ) ;
		}
		else if( strcmp( argv[k] , "SAT" ) == 0 ){
			changeSaturation( im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "MIRROR" ) == 0 ){
			if( strcmp( argv[++k] , "RIGHT" ) == 0 ){
				verticalMirror( im , 1 ) ;
			}
			else{
				verticalMirror( im , -1 ) ;
			}
		}
		else if( strcmp( argv[k] , "VIG" ) == 0 ){
			vignetting( im , im->width/((float)im->height) , atof(argv[k+1]) , atof(argv[k+2]) ) ;
			k+=2 ;
		}
		else if( strcmp( argv[k] , "BLUR" ) == 0 ){
			int kernelSize = atoi(argv[++k]) ;
			if( kernelSize%2 == 0){
				printf("%s\n", "error : BLUR's argument must be an odd integer." );
				return EXIT_FAILURE ;
			}
			blur( &im , kernelSize ) ;
		}
		else if( strcmp( argv[k] , "CONvCONV" ) == 0 ){
			contrastViaConvolution( &im ) ;
		}
		else if( strcmp( argv[k] , "SHARPEN" ) == 0 ){
			sharpen( &im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "EDGES" ) == 0 ){
			edges( &im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "TEMPERATURE" ) == 0 ){
			changeColourTemperature( im , atof(argv[k+1]) , atof(argv[k+2]) ) ;
			k += 2 ;
		}
	}
	saveImagePPM( "result.ppm" , im ) ;

	free(im) ;

	return 0 ;
}
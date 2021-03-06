#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "imageTransformations.h"
#include "k-means.h"

//Checks if a string represents a number
//Code found here : https://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C
int isNumber (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

int main( int argc , char* argv[] ){

	Image *im = loadImagePPM( argv[1] ) ;

	for( int k = 2 ; k < argc ; ++k ){
		if( strcmp( argv[k] , "ADDLUM" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : ADDLUM expects one parameter\n");
				exit(201);
			}
			changeLuminosity( im , atoi(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "DIMLUM" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : DIMLUM expects one parameter\n");
				exit(202);
			}
			changeLuminosity( im , - atoi(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "ADDCON" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : ADDCON expects one parameter\n");
				exit(203);
			}
			// 10 is a reasonnable parameter
			changeContrast( im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "DIMCON" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : DIMCON expects one parameter\n");
				exit(204);
			}
			changeContrast( im , - atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "INVERT" ) == 0 ){
			invertColours( im ) ;
		}
		else if( strcmp( argv[k] , "SEPIA" ) == 0 ){
			//This choice of parameters gives a nice sepia
			applyLUT( im ) ;
			changeColourChannels( im , -0.2 , 1.4 , 1 , 1.4 , 5 , 1.4 ) ;
		}
		else if( strcmp( argv[k] , "COLORCHG" ) == 0 ){
			//Or you can choose your own parameters here
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2]) || !isNumber(argv[k+3]) || !isNumber(argv[k+4]) || !isNumber(argv[k+5]) || !isNumber(argv[k+6]) ){
				printf("ERROR : COLORCHG expects six parameters\n");
				exit(205);
			}
			changeColourChannels( im , atof(argv[k+1]) , atof(argv[k+2]) , atof(argv[k+3]) , atof(argv[k+4]) , atof(argv[k+5]) , atof(argv[k+6]) ) ;
			k+=6 ;
		}
		else if( strcmp( argv[k] , "ADDBLACKS" ) == 0 ){
			// power=1 and factor=7 seem to be decent parameters
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2])  ){
				printf("ERROR : ADDBLACKS expects two parameters\n");
				exit(206);
			}
			affectOnlyBlacks( im , atof(argv[k+1]) , atof(argv[k+2]) , -1 ) ;
			k+=2 ;
		}
		else if( strcmp( argv[k] , "DIMBLACKS" ) == 0 ){
			// power=1 and factor=7 seem to be decent parameters
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2])  ){
				printf("ERROR : DIMBLACKS expects two parameters\n");
				exit(207);
			}
			affectOnlyBlacks( im , atof(argv[k+1]) , atof(argv[k+2]) , 1 ) ;
			k+=2 ;
		}
		else if( strcmp( argv[k] , "ADDWHITES" ) == 0 ){
			// power=1 and factor=7 seem to be decent parameters
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2])  ){
				printf("ERROR : ADDWHITES expects two parameters\n");
				exit(208);
			}
			affectOnlyWhites( im , atof(argv[k+1]) , atof(argv[k+2]) , 1 ) ;
			k+=2 ;
		}
		else if( strcmp( argv[k] , "DIMWHITES" ) == 0 ){
			// power=1 and factor=7 seem to be decent parameters
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2])  ){
				printf("ERROR : DIMWHITES expects two parameters\n");
				exit(209);
			}
			affectOnlyWhites( im , atof(argv[k+1]) , atof(argv[k+2]) , -1 ) ;
			k+=2 ;
		}
		/* NB : the following functions directly modify the image,
		 * they do not make use of LUTs because they need at the same time the Red, Green and Blue information of the pixel.
		 */

		else if( strcmp( argv[k] , "GRAYSCALE" ) == 0 ){
			applyLUT( im ) ;
			convertToGrayScale( im ) ;
		}
		else if( strcmp( argv[k] , "SAT" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : SAT expects one parameter\n");
				exit(210);
			}
			applyLUT( im ) ;
			changeSaturation( im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "MIRROR" ) == 0 ){
			applyLUT( im ) ;
			if( strcmp( argv[k+1] , "RIGHT" ) == 0 ){
				verticalMirror( im , 1 ) ;
			} else if( strcmp( argv[k+1] , "LEFT" ) == 0 ){
					verticalMirror( im , -1 ) ;
			} else {
				printf("ERROR : MIRROR expects either LEFT or RIGHT as a parameter\n");
				exit(211);
			}
			k+=1 ;
		}
		else if( strcmp( argv[k] , "VIG" ) == 0 ){
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2]) || !isNumber(argv[k+3]) || !isNumber(argv[k+4]) ){
				printf("ERROR : VIG expects four parameters\n");
				exit(212);
			}
			applyLUT( im ) ;
			vignetting( im , im->width/((float)im->height) , atof(argv[k+1]) , atof(argv[k+2]) , atof(argv[k+3]) , atof(argv[k+4]) ) ;
			k+=4 ;
		}
		else if( strcmp( argv[k] , "BLUR" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : BLUR expects one parameter\n");
				exit(213);
			}
			int kernelSize = atoi(argv[++k]) ;
			if( kernelSize%2 == 0){
				printf("%s\n", "ERROR : BLUR's argument must be an odd integer." );
				exit(214);
			}
			applyLUT( im ) ;
			blur( &im , kernelSize ) ;
		}
		else if( strcmp( argv[k] , "BLURGRAD" ) == 0 ){
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2]) || !isNumber(argv[k+3]) || !isNumber(argv[k+4]) ){
				printf("ERROR : BLURGRAD expects four parameters\n");
				exit(215);
			}
			applyLUT( im ) ;
			blurEllipticGradient( &im , im->width/((float)im->height) , atof(argv[k+1]) , atof(argv[k+2]) , atof(argv[k+3]) , atof(argv[k+4]) ) ;
			k+=4 ;
		}
		else if( strcmp( argv[k] , "CONvCONV" ) == 0 ){
			applyLUT( im ) ;
			contrastViaConvolution( &im ) ;
		}
		else if( strcmp( argv[k] , "SHARPEN" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : SHARPEN expects one parameter\n");
				exit(216);
			}
			applyLUT( im ) ;
			sharpen( &im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "EDGES" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : EDGES expects one parameter\n");
				exit(217);
			}
			applyLUT( im ) ;
			edges( &im , atof(argv[++k]) ) ;
		}
		else if( strcmp( argv[k] , "TEMPERATURE" ) == 0 ){
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2]) ){
				printf("ERROR : TEMPERATURE expects two parameters\n");
				exit(218);
			}
			applyLUT( im ) ;
			changeColourTemperature( im , atof(argv[k+1]) , atof(argv[k+2]) ) ;
			k += 2 ;
		}
		else if( strcmp( argv[k] , "THRESH" ) == 0 ){
			if( !isNumber(argv[k+1]) || !isNumber(argv[k+2]) || !isNumber(argv[k+3]) || !isNumber(argv[k+4]) || !isNumber(argv[k+5]) || !isNumber(argv[k+6]) ){
				printf("ERROR : THRESH expects six or seven parameters\n");
				exit(219);
			}
			applyLUT( im ) ;
			if( !isNumber(argv[k+7]) ){
				//No threshhold specified so we will use the median as the theshhold
				threshholdUsingMedian( im, atoi(argv[k+1]) , atoi(argv[k+2]) , atoi(argv[k+3]) , atoi(argv[k+4]) , atoi(argv[k+5]) , atoi(argv[k+6]) ) ;
				k+=6 ;
			}
			else{
				threshhold( im, atoi(argv[k+1]) , atoi(argv[k+2]) , atoi(argv[k+3]) , atoi(argv[k+4]) , atoi(argv[k+5]) , atoi(argv[k+6]) , atoi(argv[k+7]) ) ;
				k+=7 ;
			}			
		}
		else if( strcmp( argv[k] , "KMEANS" ) == 0 ){
			if( !isNumber(argv[k+1]) ){
				printf("ERROR : KMEANS expects one parameter\n");
				exit(220);
			}
			applyLUT( im ) ;
			kMeans( im , atoi(argv[k+1]) , 10 , 5 , 10 ) ;
			k += 1 ;
		}
		else if( strcmp( argv[k] , "-histo" ) == 0 ){
			applyLUT( im ) ;
			calculateHistogram( im ) ;
			saveHistogram( im , 3000 , 2000 ) ;
		}
		else{
			printf("ERROR : no function called %s\n", argv[k] );
			exit(200);
		}
	}

	applyLUT( im ) ;
	saveImagePPM( argv[1] , im );

	free(im) ;

	return 0 ;
}
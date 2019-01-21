#include <stdio.h>
#include <stdlib.h>

#include "image.h"

#ifndef COLOR_CODES__
    #define RED 0
    #define GREEN 1
    #define BLUE 2
#endif

void initializeLUT( Image* im ){
	for( int k = 0 ; k < 256 ; ++k ){
		for( int c = 0 ; c < 3 ; ++c ){
			im->luts[c][k] = k ;
		}
	}
}

unsigned char getPixel( Image* im , int i , int j , int c ){
	return im->pixels[ im->width*3*i + 3*j + c ] ;
}

void setPixel( Image* im , int i , int j , int c , unsigned char value){
	im->pixels[ im->width*3*i + 3*j + c ] = value ;
}

unsigned char toUnsignedChar( float x ){
	if( x <= 0 ){
		return 0 ;
	}
	else if( x >= 255 ){
		return 255 ;
	}
	else 
		return x ;
}

Image* newImage( int width , int height ){
    Image* im = malloc(sizeof(Image) + width*height*3*sizeof(unsigned char)) ;
    if (!im) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }
    im->width = width ;
    im->height = height ;
    initializeLUT( im ) ;
    return im ;
}

//Based on a working version found here (https://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c)
Image* loadImagePPM( char path[] )
{
     char buff[16] ;
     Image* im ;
     FILE* file ;
     int c, rgb_comp_colour ;
     //open PPM file for reading
     file = fopen(path, "rb");
     if (!file) {
          fprintf(stderr, "Unable to open file '%s'\n", path);
          exit(1);
     }

     //read image format
     if (!fgets(buff, sizeof(buff), file)) {
          perror(path);
          exit(1);
     }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(1);
    }

    //check for comments
    c = getc(file);
    while (c == '#') {
	    while (getc(file) != '\n'){} ;
	    c = getc(file);
    }

    ungetc(c, file);
    //read image size information
    int width, height ;
    if (fscanf(file, "%d %d", &width, &height) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", path);
         exit(1);
    }

    //alloc memory for image
    im = newImage( width , height ) ;

    //read rgb component
    if (fscanf(file, "%d", &rgb_comp_colour) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", path);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp_colour!= 255) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", path);
         exit(1);
    }

    while (fgetc(file) != '\n') ;

    //read pixel data from file
    if (fread(im->pixels, 3 * im->width, im->height, file) != im->height) {
         fprintf(stderr, "Error loading image '%s'\n", path);
         exit(1);
    }

    fclose(file);
    
    return im;
}

void saveImagePPM( char path[] , Image* im )
{
    FILE* file;
    //open file for output
    file = fopen(path, "wb");
    if (!file) {
         fprintf(stderr, "Unable to open file '%s'\n", path);
         exit(1);
    }

    //write the header file
    //image format
    fprintf(file, "P6\n");

    //image size
    fprintf(file, "%d %d\n",im->width,im->height);

    // rgb component depth
    fprintf(file, "%d\n", 255);

    // pixel data
    for( int i = 0 ; i < im->height ; ++i ){
    	for( int j = 0 ; j < im->width ; ++j ) {
    		for( int c = 0 ; c < 3 ; ++c ){
    			unsigned char newPixelValue = toUnsignedChar(im->luts[c][getPixel(im,i,j,c)]) ;
    			fwrite( &newPixelValue , 1 , 1 , file) ;
    		}
    	}
    }

    fclose(file);
}




// My version, which was almost working :
/*Image* loadImagePPM2( char path[] ){
    FILE* file = fopen( path , "r"); // read only
    if( file == NULL ){
      printf("Opening image failed !");   
      exit(1);
    }
    char version[2] ;
    int width ;
    int height ;
    int maxValue ;
    unsigned char weirdValue ;
    fscanf(file,"%[^\n]", version) ;
    fscanf(file,"%d", & width) ;
    fscanf(file,"%d", & height) ;
    fscanf(file,"%d", &maxValue) ;
    fscanf(file,"%c", &weirdValue) ;

    Image* im = malloc(sizeof(Image) + width*height*3*sizeof(unsigned char)) ;
    im->width = width ;
    im->height = height ;
    for( int k = 0 ; k < width*height*3 ; ++k ){
        fscanf(file, "%c", &im->pixels[k]) ;
    }

    fclose(file) ;

    return im ;
}

void saveImagePPM2( char path[] , Image* im ){
    FILE* file ;
    if ( ! (file = fopen(path,"w")) ){
       printf("Saving image failed !") ;
       exit(1) ;
    }

    fprintf(file,"%s","P6\n") ;
    fprintf(file,"%d %d\n", im->width , im->height) ;
    fprintf(file,"%d ", 255) ;
    for( int i = 0 ; i < im->height ; ++i ){
        for( int j = 0 ; j < im->width ; ++j ){
            for( int c = 0 ; c < 3 ; ++c ){
                fprintf( file,"%c", getPixel(im,i,j,c) ) ;
            }
        }
        //fprintf(file, "\n" );
    }

    fclose(file); 
}*/

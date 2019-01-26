#ifndef IMAGE_H__
	#define IMAGE_H__

typedef struct Image {
    int width ;
    int height ;
    int luts[3][256] ;
    int histos[3][256] ;
    int avgHisto[256] ;
    unsigned char pixels[] ;
} Image ;

void initializeLUT( Image* im ) ;

void applyLUT( Image* im ) ;

unsigned char getPixel( Image* im , int i , int j , int colour ) ;

void setPixel( Image* im , int i , int j , int c , unsigned char value) ;

unsigned char toUnsignedChar( float x ) ;

Image* loadImagePPM( char path[] ) ;

void saveImagePPM( char path[] , Image* im ) ;

Image* newImage( int width , int height ) ;

#endif
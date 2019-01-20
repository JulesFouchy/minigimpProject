#ifndef COLOR_SPACES_H__
	#define COLOR_SPACES_H__

typedef struct RGBcolour {
    float R ;
    float G ;
    float B ;
} RGBcolour ;

typedef struct HSVcolour {
    float H ;
    float S ;
    float V ;
} HSVcolour ;

HSVcolour HSVfromRGB( RGBcolour colour ) ;

RGBcolour RGBfromHSV( HSVcolour colour ) ;

float modulo( float x , float m ) ;


#endif
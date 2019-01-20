#ifndef COLOR_SPACES_H__
	#define COLOR_SPACES_H__

typedef struct RGBcolour RGBcolour ;

typedef struct HSVcolour HSVcolour ;

HSVcolour HSVfromRGB( RGBcolour colour ) ;

RGBcolour RGBfromHSV( HSVcolour colour ) ;

float modulo( float x , float m ) ;


#endif
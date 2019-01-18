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

float modulo( float x , float m ) ;

HSVcolour HSVfromRGB( RGBcolour colour ) ;

RGBcolour RGBfromHSV( HSVcolour colour ) ;
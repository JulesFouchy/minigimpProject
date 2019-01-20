#include <math.h>

#include "colorSpaces.h"

    #ifndef COLOR_CODES__
        #define COLOR_CODES__
        #define RED 0
        #define GREEN 1
        #define BLUE 2
    #endif





float modulo( float x , float m ){
    while( x < 0 ){
        x+=m ;
    }
    while( x >= m ){
        x-=m ;
    }
    return x ;
}

HSVcolour HSVfromRGB( RGBcolour colour ){
    float R = colour.R / 255 ;
    float G = colour.G / 255 ;
    float B = colour.B / 255 ;

    float H,S,V ;

    float min = R ;
    float max = R ;
    int isMax = RED ;
    if( G < min ){
        min = G ;
    }
    if( G > max ){
        max = G ;
        isMax = GREEN ;
    }
    if( B < min ){
        min = B ;
    }
    if( B > max ){
        max = B ;
        isMax = BLUE ;
    }

    float D = max - min ;


    
        if( isMax == RED ){
            H = modulo( 60 * (G-B)/D , 360) ;
        }
        else{
            if( isMax == GREEN ){
                H = 60 *( (B-R)/D + 2 ) ;
            }
            else{
                H = 60 *( (R-G)/D + 4 ) ;
            }
        }
    

        S = D / max * 100 ;

    V = max*100 ;

    HSVcolour hsv ;
    hsv.H = H ;
    hsv.S = S ;
    hsv.V = V ;
    return hsv ;

}

RGBcolour RGBfromHSV( HSVcolour colour ){
    float H = colour.H ;
    float S = colour.S / 100 ;
    float V = colour.V / 100 ;

    float C = V * S ;
    float X = C * ( 1 - fabs(modulo((H / 60) , 2 ) - 1)) ;
    float m = V - C ;

    float R,G,B ;
    if( H < 60 ){
        R = C ;
        G = X ;
        B = 0 ;
    }
    else{
        if( H < 120 ){
            R = X ;
            G = C ;
            B = 0 ;
        }
        else{
            if( H < 180 ){
                R = 0 ;
                G = C ;
                B = X ;
            }
            else{
                if( H < 240 ){
                    R = 0 ;
                    G = X ;
                    B = C ;
                }
                else{
                    if( H < 300 ){
                        R = X ;
                        G = 0 ;
                        B = C ;
                    }
                    else{
                        R = C ;
                        G = 0 ;
                        B = X ;
                    }
                }
            }
        }
    }

    RGBcolour rgb ;
    rgb.R = ( R + m ) * 255 ;
    rgb.G = ( G + m ) * 255 ;
    rgb.B = ( B + m ) * 255 ;
    return rgb ;
}

//Conversion formulas found here : https://www.rapidtables.com/convert/color/rgb-to-hsl.html
/* Currently not working
HSLcolour HSLfromRGB( RGBcolour colour ){
    float R = colour.R / 255 ;
    float G = colour.G / 255 ;
    float B = colour.B / 255 ;
    HSLcolour hslCol ;
    
    //Calculate min and max of (R,G,B)
    float min = R ;
    float max = R ;
    int isMax = RED ;
    if( G < min ){
        min = G ;
    }
    if( G > max ){
        max = G ;
        isMax = GREEN ;
    }
    if( B < min ){
        min = B ;
    }
    if( B > max ){
        max = B ;
        isMax = BLUE ;
    }

    float D = max - min ;
    if( D < 0.00001 ){
        hslCol.H = 0 ;
    }
    else{
        if( isMax == RED ){
            hslCol.H = 60* modulo( (G-B)/D , 6 ) ;
        }
        else {
            if( isMax == GREEN ){
                hslCol.H = 60*( (B-R)/D + 2 ) ;
            }
            else{
                hslCol.H = 60*( (R-G)/D + 4 ) ;
            }
        }
    }
    if( hslCol.H < 0 ){
        hslCol.H += 360 ;
    }

    hslCol.L = (max + min) / 2 ;
        
    if( D < 0.00001 ){
        hslCol.S = 0 ;
    }
    else{
        hslCol.S = D / ( 1 - abs( 2*hslCol.L - 1) ) ;
    }
    
    return hslCol ;
}

//Conversion formulas found here : https://www.rapidtables.com/convert/color/hsl-to-rgb.html
RGBcolour RGBfromHSL( HSLcolour colour ){
    float C = colour.S * ( 1 - abs(2*colour.L-1) ) ;
    float X = C * (1 - abs( modulo(colour.H / 60 , 2 )- 1) ) ;
    float m = colour.L - C/2 ;
    //printf("%f\n", m );
    float R,G,B ;
    if( colour.H < 60 ){
        R = C ;
        G = X ;
        B = 0 ;
    }
    else if( colour.H < 120 ){
        R = X ;
        G = C ;
        B = 0 ;
    }
    else if( colour.H < 180 ){
        R = 0 ;
        G = C ;
        B = X ;
    }
    else if( colour.H < 240 ){
        R = 0 ;
        G = X ;
        B = C ;
    }
    else if( colour.H < 300 ){
        R = X ;
        G = 0 ;
        B = C ;
    }
    else{
        R = C ;
        G = 0 ;
        B = X ;
    }
    RGBcolour rgb ;
    rgb.R = (R+m)*255 ;
    rgb.G = (G+m)*255 ;
    rgb.B = (B+m)*255 ;
    return rgb ;
} */
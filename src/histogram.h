#ifndef HISTOGRAM_H__
	#define HISTOGRAM_H__

	#include "image.h"

void calculateHistogram( Image *im ) ;

void calculateAverageHistogram( Image *im ) ;

int maxOfHistos( int histos[3][256] ) ;

int roundfl( float x ) ;

void saveHistogram( Image *im , int width , int height ) ;

int calculateMedian( Image* im ) ;

#endif
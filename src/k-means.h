#ifndef K_MEANS_H__
	#define K_MEANS_H__

#include "image.h"
#include "colorSpaces.h"

typedef struct Cluster{
	RGBcolour centroid ;
	long unsigned int Rsum ;
	long unsigned int Gsum ;
	long unsigned int Bsum ;
	int nbPixels ;
} Cluster ;

float distSq( RGBcolour rgb , float R , float G , float B );

void kMeans( Image* im , int k , int samplingStep , float epsilonForConvergence , int maxNumberIterations );

#endif
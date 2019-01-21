#include "k-means.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

float distanceSq( RGBcolour rgb , float R , float G , float B ){
	return pow(rgb.R-R,2) + pow(rgb.G-G,2) + pow(rgb.B-B,2) ;
}

void kMeans( Image* im , int k , int samplingStep , float epsilonForConvergence , int maxNumberIterations ){
	Cluster clusters[k] ;
	srand(time(NULL));
	//Initialize clusters with a random centroid
	for( int l = 0 ; l < k ; ++l ){
		int i = (float) rand()/RAND_MAX*im->height;
		int j = (float) rand()/RAND_MAX*im->width ;
		clusters[l].centroid.R = getPixel( im , i , j , 0 ) ;
		clusters[l].centroid.G = getPixel( im , i , j , 1 ) ;
		clusters[l].centroid.B = getPixel( im , i , j , 2 ) ;
	}
	int convIsReached = 0 ;
	int nbIterations = 0 ;
	while( !convIsReached && nbIterations<maxNumberIterations ){
		nbIterations +=1 ;
		//Initialize
		for( int l = 0 ; l < k ; ++l ){
			clusters[l].Rsum = 0 ;
			clusters[l].Gsum = 0 ;
			clusters[l].Bsum = 0 ;
			clusters[l].nbPixels = 0 ;
		}
		//Calculate the next generation of centroids
		int indexMinDistSq ;
		float minDistSq , distSq ;
		float R,G,B ;
			//Loop over the pixels
		for( int i = 0 ; i < im->height ; i += samplingStep ){
			for( int j = 0 ; j < im->width ; j += samplingStep ){
				R = getPixel(im,i,j,0) ;
				G = getPixel(im,i,j,1) ;
				B = getPixel(im,i,j,2) ;
				//Find closest centroid
				indexMinDistSq = 0 ;
				minDistSq = distanceSq( clusters[0].centroid , R , G , B ) ;
				for( int l = 1 ; l < k ; ++l ){
					distSq = distanceSq( clusters[l].centroid , R , G , B ) ;
					if( distSq < minDistSq ){
						minDistSq = distSq ;
						indexMinDistSq = l ;
					}
				}
				//Add pixel to the cluster
				clusters[indexMinDistSq].Rsum += R ;
				clusters[indexMinDistSq].Gsum += G ;
				clusters[indexMinDistSq].Bsum += B ;
				clusters[indexMinDistSq].nbPixels += 1 ;
			}
		}
			//Update centroid value and check if convergence is reached
		float nextR , nextG , nextB ;
		convIsReached = 1 ;
		for( int l = 0 ; l < k ; ++l ){
			nextR = clusters[l].Rsum/clusters[l].nbPixels ;
			nextG = clusters[l].Gsum/clusters[l].nbPixels ;
			nextB = clusters[l].Bsum/clusters[l].nbPixels ;
			if( distanceSq( clusters[l].centroid , nextR , nextG , nextB ) > pow(epsilonForConvergence,2) ){
				convIsReached = 0 ;
			}
			clusters[l].centroid.R = nextR ;
			clusters[l].centroid.G = nextG ;
			clusters[l].centroid.B = nextB ;
		}
	}
	//Transform the pixel into their cluster's colour
	for( int i = 0 ; i < im->height ; i++ ){
		for( int j = 0 ; j < im->width ; j++ ){
		//Find closest centroid
			int indexMinDistSq ;
			float minDistSq , distSq ;
			float R,G,B ;
			R = getPixel(im,i,j,0) ;
			G = getPixel(im,i,j,1) ;
			B = getPixel(im,i,j,2) ;
			indexMinDistSq = 0 ;
			minDistSq = distanceSq( clusters[0].centroid , R , G , B ) ;
			for( int l = 1 ; l < k ; ++l ){
				distSq = distanceSq( clusters[l].centroid , R , G , B ) ;
				if( distSq < minDistSq ){
					minDistSq = distSq ;
					indexMinDistSq = l ;
				}
			}
		//Change colour
			setPixel(im,i,j,0,clusters[indexMinDistSq].centroid.R) ;
			setPixel(im,i,j,1,clusters[indexMinDistSq].centroid.G) ;
			setPixel(im,i,j,2,clusters[indexMinDistSq].centroid.B) ;
		}
	}
}
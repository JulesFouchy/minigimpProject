Example commands that are worth checking out : 
	./bin/minigimp images/ju.ppm MIRROR LEFT ADDLUM 10 ADDCON 15
	./bin/minigimp images/zoedesbois.ppm BLURGRAD 0.05 0.1 0.576 0.223
	./bin/minigimp images/paysage.ppm SEPIA
        ./bin/minigimp images/bonjour.ppm -histo KMEANS 11 -histo (check this twice to see variations on the k-means algorithm)

Documentation : 

-histo :              option that can be called to save the histogram of the image with all the effects called before -histo

ADDLUM( x ) :         increases luminosity
                      x should be between 0 (no modification) and 255 (fully white image)

DIMLUM( x ) :         decreases luminosity
                      x should be between 0 (no modification) and 255 (fully black image)

ADDCON( x ) :         increases contrast
                      x should be between 0 (no modification) and infinity
	  	      x=10 is a reasonable value

DIMCON( x ) :         decreases contrast
                      x should be between 0 (no modification) and infinity
		      x=10 is a reasonable value

INVERT :              inverts the colours
                      no parameters

SEPIA :               applies a sepia effect
                      no parameters

COLORCHG( REDa , REDb , GREENa , GREENb , BLUEa , BLUEb ) :

  		      each color channel is modified following a cubic curve
                      you can play with the coefficients a and b here : https://www.geogebra.org/classic/yfggz7nu

ADDBLACKS( power , factor ) :
 
                      darkens the dark regions of the image using an exponential function like e^(-factor*x^power)
                      factor should be >0 for sensible results
                      power = 1 and factor = 7 are reasonable values

DIMBLACKS( power , factor ) : 

                      brightens the dark regions of the image using an exponential function like e^(-factor*x^power)
                      factor should be >0 for sensible results
                      power = 1 and factor = 7 are reasonable values

ADDWHITES( power , factor ) : 

                      brightens the bright regions of the image using an exponential function like e^(-factor*x^power)
                      factor should be >0 for sensible results
                      power = 1 and factor = 7 are reasonable values

DIMWHITES( power , factor ) : 

                      darkens the bright regions of the image using an exponential function like e^(-factor*x^power)
                      factor should be >0 for sensible results
                      power = 1 and factor = 7 are reasonable values

GRAYSCALE :           converts to grayscale
                      no parameters

SAT( t ) :            affects saturation
                      0<t<1 reduces saturation (t=0 is the same as calling GRAYSCALE)
                      t>1 increases saturation

MIRROR( direction ) : direction can be RIGHT : copies a mirror image of the right part of the image onto the left  one
                                    or LEFT  : copies a mirror image of the left  part of the image onto the right one

VIG( gradientSpeed , unaffectedPart'sRadiusRatio , center'sXcoordinateRatio , center'sYcoordinateRatio ) :

                      adds vignetting
                      gradientSpeed = 0.3 is a reasonable parameter
                      the three last parameters are expressed as percentages of the image's width and heigth
                      they should be between 0 and 1

BLUR( kernelSize ) :  blurs the image
                      kernelSize must be an odd integer
                      the bigger kernelSize is, the stronger the blur is

BLURGRAD( gradientSpeed , unaffectedPart'sRadiusRatio , center'sXcoordinateRatio , center'sYcoordinateRatio ) :

                      creates an elliptic gradient of blur ; just like vignetting but blurs instead of darkening
                      same parameters as VIG
                      
CONvCONV :            adds contrast via a convolution kernel

SHARPEN( x ) :        sharpens the image. It is more usefull for image analysis than photo editing but it is here nonetheless
                      x=3 is a reasonable parameter

EDGES( x ) :          a very basic edge detector
                      should be preceded by BLUR for better results
                      BLUR 7 EDGES 5 should work OK

TEMPERATURE( temperature , t ) :

                      changes the color temperature of the image
                      t is between 0 and 1 and represents how strongly the effect is applied to the image
                      temperature = 2200  and t=0.1 give a warm tint
                      temperature = 10000 and t=0.1 give a cold tint

THRESH( R0 , G0 , B0 , R1 , G1 , B1 , threshhold ) : 

                      sets the pixels whose luminosity is below the threshhold to (R0,G0,B0), and the others to (R1,G1,B1)
                      threshhold should be between 0 and 255, as well as R , G and B 
                      if no threshhold is specified, the function will calculate a threshhold with half of the pixels below it, and the other half above
                      should be preceded by BLUR for a nicer effect

KMEANS( k ) :         finds a good representation of the image using only k colours
                      note that since the starting points are random, the algorithm may find different representative colours each time it is applied
                      so if you're not satisfied with the colours it picked, you may just run the algorithm again
                      

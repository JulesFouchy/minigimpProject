# makefile2 correspondant a l'exercice 2, TP8

# Dans un fichier makefile, on peut utiliser de variables.
# monvar = valeur    pour affecter
# $(monvar)          pour recuperer la valeur

# De plus, il y a quelques variables internes, qu'on peut utiliser:
# $@ pour nom de cible
# $< pour nom de la premiere dependance
# $^ pour toute la liste des dépendances

# le suivant alors fait precisement la meme chose que makefile1:


CC = gcc
CFLAGS = -Wall -O2


minigimp: main.o image.o imageTransformations.o colorSpaces.o histogram.o k-means.o
	$(CC) -o $@  $^ -lm

main.o: main.c imageTransformations.h image.h colorSpaces.h histogram.h k-means.h
	$(CC) -o $@ -c $< $(CFLAGS)

image.o: image.c image.h
	$(CC) -o $@ -c $< $(CFLAGS)

imageTransformations.o: imageTransformations.c imageTransformations.h image.h colorSpaces.h histogram.h
	$(CC) -o $@ -c $< $(CFLAGS)

colorSpaces.o: colorSpaces.c  colorSpaces.h
	$(CC) -o $@ -c $< $(CFLAGS)

histogram.o: histogram.c  histogram.h image.h
	$(CC) -o $@ -c $< $(CFLAGS)

k-means.o: k-means.c k-means.h image.h colorSpaces.h
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o

# mais en plus, on va effacer les fichiers .o

# On peut en faire encore plus ; dans certains cas, par exemple, certains
# regles peuvent etre omis, cars définis implicitement.
#
# Si vous voulez en connaitre plus, pour l'instant, cherchez en ligne.
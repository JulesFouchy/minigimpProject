CC = gcc
CFLAGS = -Wall -O2


bin/minigimp: bin/main.o bin/image.o bin/imageTransformations.o bin/colorSpaces.o bin/histogram.o bin/k-means.o
	$(CC) -o $@  $^ -lm

bin/main.o: src/main.c src/imageTransformations.h src/image.h src/colorSpaces.h src/histogram.h src/k-means.h
	$(CC) -o $@ -c $< $(CFLAGS)

bin/image.o: src/image.c src/image.h
	$(CC) -o $@ -c $< $(CFLAGS)

bin/imageTransformations.o: src/imageTransformations.c src/imageTransformations.h src/image.h src/colorSpaces.h src/histogram.h
	$(CC) -o $@ -c $< $(CFLAGS)

bin/colorSpaces.o: src/colorSpaces.c  src/colorSpaces.h
	$(CC) -o $@ -c $< $(CFLAGS)

bin/histogram.o: src/histogram.c  src/histogram.h src/image.h
	$(CC) -o $@ -c $< $(CFLAGS)

bin/k-means.o: src/k-means.c src/k-means.h src/image.h src/colorSpaces.h
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o

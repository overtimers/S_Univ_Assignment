all: dining_philosophers first_readers_writers

dining_philosophers: dining_philosophers.c
	gcc -o dining_philosophers dining_philosophers.c -lpthread

first_readers_writers: first_readers_writers.c
	gcc -o first_readers_writers first_readers_writers.c -lpthread

clean:
	rm dining_philosophers first_readers_writers

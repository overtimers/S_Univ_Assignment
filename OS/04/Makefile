CC = gcc
RM = rm -rf
LDFLAGS = -lpthread

all: multithread_practice multithread_practice_solution

multithread_practice: multithread_practice.c
	$(CC) -o multithread_practice multithread_practice.c
multithread_practice_solution: multithread_practice_solution.c
	$(CC) -o multithread_practice_solution multithread_practice_solution.c $(LDFLAGS)

clean:
	rm multithread_practice multithread_practice_solution

all: assignment_a assignment_b

assignment_a: assignment_a.cpp
	g++ -o assignment_a assignment_a.cpp -lpthread

assignment_b: assignment_b.c
	gcc -w -o assignment_b assignment_b.c

clean: assignment_a assignment_b
	rm assignment_a assignment_b

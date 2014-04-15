run:
	gcc -Wall -fopenmp calka_omp.c -lm -lrt
	./a.out 1000000
clean:
	rm -f *.o *~ *.in *.out
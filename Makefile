run:
	gcc -Wall calka_seq.c -o a.out -lrt -lm
	./a.out 1000000
clean:
	rm -f *.o *~ *.in *.out
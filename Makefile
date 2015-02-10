all:
	clang -Wall measure.c -o measure
my_shell:
	clang -Wall measure.c -o measure
run:
	./measure
clean: 
	rm -f measure *.o
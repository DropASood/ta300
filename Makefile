all:
	make clean
	#clang -Wall measure.c -o measure
	clang -Wall part2.c -o p2
measure:
	clang -Wall measure.c -o measure
part2:
	clang -Wall part2.c -o p2
run:
	./measure
clean: 
	rm -f measure p2 *.o
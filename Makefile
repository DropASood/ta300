all:
	clang -Wall hr-timer.c -o p1
	clang -Wall p2.c -o p2
	clang -Wall p3.c -o p3
	clang -Wall p4.c -o p4
	clang -Wall p5.c -o p5 -lpthread

p1:
	clang -Wall hr-timer.c -o p1
p2:
	clang -Wall p2.c -o p2
p3:
	clang -Wall p3.c -o p3
p4:
	clang -Wall p4.c -o p4
p5:
	clang -Wall p5.c -o p5 -lpthread

clean: 
	rm -f p1 p2 p3 p4 p5 *.o
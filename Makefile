all:
	g++ -std=c++11 proj1.cpp -o simulate
machine:
	gcc -std=c99 LC3101a.c -o makeMachine
cCode:
	gcc -std=c99 proj1.c -o simulate_c.x
run:
	./simulate machine.mc > output.txt
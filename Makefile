#
# EE 4374 Assignment 3 Makefile
# Author: Jesus Garcia
#
command: jgarcia_prime.o jgarcia_prog3.o
	gcc -o command jgarcia_prime.o jgarcia_prog3.o -pthread 

jgarcia_prime.o: jgarcia_prime.c
	gcc -c jgarcia_prime.c jgarcia_prime.h -pthread
jgarcia_prog3.o: jgarcia_prog3.c
	gcc -c jgarcia_prog3.c -pthread
clean:
	rm *.o; rm jgarcia_prog2

demo: command
	./command

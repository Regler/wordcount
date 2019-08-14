wordcount : main.o liblist1.a
	gcc -g -o wordcount main.o liblist1.a
main.o: main.c list.h
	gcc -c -Wall -g main.c 
#list.o: list.c list.h
#	gcc -c -Wall -g  list.c
#liblist1.a:list.o
#	ar rc liblist1.a list.o
.PHONY:clean
clean:
	rm -rf *.o wordcount


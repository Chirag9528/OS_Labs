target: pc.o zem.o
	gcc pc.o zem.o -o a.out
	./a.out 10 10

pc.o: pc_semaphore.c
	gcc pc_semaphore.c -c -o pc.o

zem.o: MySemaphore.c
	gcc MySemaphore.c -c -o zem.o

clean:
	rm *.o *.out first

target: queue queue_lock
	./queue
	./queue_lock

queue: queue.o queue_lock.o
	gcc queue.o -o queue
	gcc queue_lock.o -o queue_lock

queue.o: queue.c
	gcc -c queue.c -o queue.o

queue_lock.o: queue_lock.c
	gcc -c queue_lock.c -o queue_lock.o

plot:
	python3 plotting.py

clean:
	rm -f queue queue_lock *.o


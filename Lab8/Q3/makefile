target: linked_list linked_list_lock linked_list_hand
	./linked_list 
	./linked_list_lock
	./linked_list_hand

linked_list : linked_list.o linked_list_lock.o linked_list_hand.o
	gcc linked_list.o -o linked_list
	gcc linked_list_lock.o -o linked_list_lock
	gcc linked_list_hand.o -o linked_list_hand

linked_list.o: linked_list.c
	gcc -c linked_list.c -o linked_list.o

linked_list_lock.o: linked_list_lock.c
	gcc -c linked_list_lock.c -o linked_list_lock.o

linked_list_hand.o: linked_list_hand.c
	gcc -c linked_list_hand.c -o linked_list_hand.o 

debug: 
	gcc linked_list.c -g -o a.out
	valgrind --leak-check=full ./a.out

debug_lock:
	gcc linked_list_lock.c -g -o b.out
	valgrind --leak-check=full ./b.out

debug_hand:
	gcc linked_list_hand.c -g -o c.out
	valgrind --leak-check=full ./c.out

plot:
	python3 ./plotting.py

csv:
	rm -f *.csv

clean:
	rm -f linked_list linked_list_lock linked_list_hand *.o *.out 
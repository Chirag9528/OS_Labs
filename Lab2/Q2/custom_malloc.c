#include "custom_malloc.h"
free_h* head;
void* mymalloc(int size){
	free_h* temp = head;
	free_h* prev = head;
	while (temp && temp->size < size + node_size){
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL) return NULL;
	node_h* chunk = (node_h*)temp;
	if (temp == head){
		head = (free_h*)((void*)(chunk+1) + size);
		head->size = temp->size - size - node_size;
		head->next = temp->next;
	}
	else{
		int remain_size = temp->size - size - node_size;
		free_h* nextptr = temp->next;
		temp = (free_h*)((void*)(chunk+1) + size);
		temp->size = remain_size;
		temp->next = nextptr;
		prev->next = temp;

	}
	chunk->size = size;
	chunk->magic_number = 1234567;
	return (void*)(chunk + 1);	
}

void myfree(void* ptr){
	if (ptr == NULL) return;
	node_h* node = ptr - node_size;
	if ((void*)head > (void*)ptr){
		if ((void*)(node + 1) + node->size ==  (void*)head){
			free_h* newnode = (free_h*) node;
			newnode->size += head->size + free_size;
			newnode->next = head->next;
			head = newnode;
		}
		else{
			free_h* newnode = (free_h*)node;
			newnode->next = head;
			head = newnode;
		}
	}
	else{
		free_h* prev = head;
		free_h* curr = head->next;
		while (curr && (void*)curr < (void*)node){
			prev = curr;
			curr = curr->next;
		}
		if ((void*)(prev+1)+prev->size == (void*)node){
			prev->size += node->size + node_size;
			if (curr && ((void*)(node + 1 )+ node->size) == (void*)curr){
				prev->size += curr->size + free_size;
				prev->next = curr->next;
			}
		}
		else{
			free_h* newnode = (free_h*)node;
			newnode->next = curr;
			prev->next = newnode;
			if (curr && ((void*)(newnode+1)+newnode->size) == (void*)curr){
				newnode->size+= curr->size + free_size;
				newnode->next = curr->next;
			}
		}
	}
	return;
} 

void printlist(void* head){
	free_h* temp = (free_h*)head;
	printf("List: \n");
	while (temp){
		printf("size-> %d  === ", temp->size);
		temp = temp->next;
	}
	printf("\nListEnded\n");
}

int main(){
	head = (free_h*)(heap);
	head->size = 4096 - sizeof(head);
	head->next = NULL;

	printlist(head);

	int* arr1 = (int*)mymalloc(20);
	printlist(head);

	int* arr2 = (int*)mymalloc(80);
	printlist(head);

	int* arr3 = (int*)mymalloc(200);
	printlist(head);

	myfree(arr2);
	printlist(head);

	int* arr4 = (int*)mymalloc(100);
	printlist(head);

	int* arr5 = (int*)mymalloc(100);
	printlist(head);

	myfree(arr4);
	printlist(head);

	myfree(arr5);
	printlist(head);

	myfree(arr3);
	printlist(head);

	myfree(arr1);
	printlist(head);

	int* arr6 = (int*)mymalloc(84);
	printlist(head);
	// printlist(head);

	return 0;
}

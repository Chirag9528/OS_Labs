#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task{
    char type;
    int duration;
}task;

typedef struct Process{
    int process_num;
    int arrival_time;
    int tasks_num;
    int duration;
    task* tasks;
}process;

void SelectionSort(process** p , int total){
    for (int i=0 ; i<total ; i++){
        int idx = i;
        for (int j= i+1 ; j<total ; j++){
            if (p[idx]->arrival_time > p[j]->arrival_time){
                idx = j;
            }
        }
        if (idx != i){
            process* temp = p[i];
            p[i] = p[idx];
            p[idx] = temp;
        }
    }
}

int calculateprocess(FILE* fptr){
    int total = 0;
    char* process = NULL;
    size_t len = 0;
    size_t temp = 0;
    while (temp!=-1){
        temp = getline(&process , &len , fptr);
        if (temp!= -1) total++;
    }
    return total;
}

void makestruct(process* p , char* string){
    int first = 0;
    int second = 0;
    while (string[second] != ' '){
        second++;
    }
    first = second+1;
    second = second+1;
    while (string[second]!= ' '){
        second++;
    }    
    char* arrivaltime = (char*)malloc((second-first+1)*sizeof(char));
    for (int i=0 ; i<second-first ; i++){
        arrivaltime[i] = string[first+i];
    }
    int arrive = atoi(arrivaltime);
    p->arrival_time = arrive;

    second++;
    int temp = second;

    int numtask = 0;
    while (string[temp]!= '\n'){
        if (string[temp++] == ' ') numtask++;
    }
    numtask = numtask>>1;
    p->tasks_num = numtask;
    p->tasks = (task*)malloc(numtask*sizeof(task));
    p->duration = 0;
    for (int i=0 ; i<numtask ; i++){
        p->tasks[i].type = string[second];
        second+= 2;
        first = second;
        while (string[second]!= ' '){
            second++;
        }
        char* duration = (char*)malloc((second-first+1)*sizeof(char));
        for (int j=0 ; j<second-first ; j++){
            duration[j] = string[first+j];
        }
        int duration_time = atoi(duration);
        p->tasks[i].duration = duration_time;
        p->duration+= duration_time;
        second++;
    }
}

int main(){
    FILE* fptr = fopen("./../Q1/processes.txt" , "r");
    int total_process = calculateprocess(fptr);
    rewind(fptr);
    process** processes = (process**)malloc(total_process*sizeof(process*));

    for (int i=1 ; i<=total_process ; i++){
        char* temp = NULL;
        size_t len = 0;
        size_t temp2 = 0;
        temp2 = getline(&temp , &len , fptr);
        printf("%s" , temp);

        process* pro = (process*)malloc(sizeof(process));
        pro->process_num = i;
        makestruct(pro , temp);
        processes[i-1] = pro;
    }

    SelectionSort(processes , total_process);

    int turnaround_time = 0;
    int response_time = 0;
    int time = 0;
    if (total_process > 0){
        time = processes[0]->arrival_time;
    }
    for (int i=0 ; i<total_process ; i++){
        response_time += time - processes[i]->arrival_time;
        time+= processes[i]->duration;
        turnaround_time += time - processes[i]->arrival_time;
        if (i+1 < total_process){
            if (processes[i+1]->arrival_time > time){
                time = processes[i+1]->arrival_time;
            }
        }
    }
    printf("\n\nFIFO Scheduler: \n");
    printf("Turnaround Time: %f\n" , (1.0*turnaround_time)/total_process);
    printf("Response Time: %f\n" , (1.0*response_time)/total_process);

    fclose(fptr);
    return 0;
}
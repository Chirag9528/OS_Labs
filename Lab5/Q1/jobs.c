#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main(){
    FILE* fptr = fopen("processes.txt" , "w");
    int n;
    scanf("%d" , &n);
    srand(time(0));
    for (int i=1 ; i<=n ; i++){
        int start_time = 1  + (rand()%100);
        fprintf(fptr , "%c%d %d " , 'P' , i , start_time);
        int jobs = 1 + (rand()%10);
        for (int j=0 ; j<jobs ; j++){
            if ((j&1) == 0){
                int process_time = 1 + (rand()%200);
                fprintf(fptr , "%c %d " , 'C' , process_time);
            }
            else{
                int process_time = 1 + (rand()%200);
                fprintf(fptr , "%c %d ", 'I' , process_time);
            }
        }
        fprintf(fptr , "\n");
    }
    fclose(fptr);
    
    return 0;
}
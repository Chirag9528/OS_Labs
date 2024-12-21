#include "approximate_counter.h"
#include "precise.h"
#include <sched.h>

// Approximate
void init(counter_t* c , int threshold){
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock , NULL);
    c->local = (int*)malloc(NUMCPUS*sizeof(int));
    c->llocks = (pthread_mutex_t*)malloc(NUMCPUS*sizeof(pthread_mutex_t));
    for (int i=0 ; i<NUMCPUS ; i++){
        c->local[i] = 0;
        pthread_mutex_init(&c->llocks[i] , NULL);
    }
}

void* update(void* temp){
    
    func_arg* arg_list = (func_arg*)temp;
    counter_t* c = arg_list->counter;
    int threadID = arg_list->thread_id;
    int amt = arg_list->amount;
    int cpu = threadID % NUMCPUS;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(threadID%NUMCPUS, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    for (int i=0 ; i<1e6 ; i++){
        pthread_mutex_lock(&c->llocks[cpu]);
        c->local[cpu]+= amt;
        if (c->local[cpu] >= c->threshold){
            pthread_mutex_lock(&c->glock);
            c->global += c->local[cpu];
            pthread_mutex_unlock(&c->glock);
            c->local[cpu] = 0;
        }
        pthread_mutex_unlock(&c->llocks[cpu]);
    }
    return NULL;
}

int get(counter_t* c){
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}


// Precise
void* increment(){
    for (int i=0 ; i<1e6 ; i++){
        pthread_mutex_lock(&lock);
        counter.value++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}


int main(int argc , char* argv[]){
    if (argc < 3){
        fprintf(stderr , "Additional arguements are missing!!!\n");
        return -1;
    }

    NUMCPUS = atoi(argv[1]);
    int threshold = atoi(argv[2]);

    // Approximate
    FILE* fp = fopen("data1.txt" , "w");
    if (fp == NULL){
        printf("Error:Could not open file\n");
        exit(1);
    }

    for (int thread = 1 ; thread <= 4 ; thread++){
        int num_threads = thread;

        counter_t counter;
        init(&counter , threshold);
        
        printf("Approximate :-> Starting Counter: %d\n" , counter.global);
        clock_t start = clock();


        pthread_t threads[num_threads];

        for (int i=0 ; i<num_threads ; i++){
            func_arg argmts;
            argmts.amount = 1;
            argmts.counter = &counter;
            argmts.thread_id = i;
            pthread_create(&threads[i] , NULL , update , &argmts);
        }

        for (int i=0 ; i<num_threads ; i++){
            pthread_join(threads[i] , NULL);
        }

        clock_t end = clock();

        printf("Approximate :-> Ending Counter: %d\n" , counter.global);
        double time = (double)(end-start)/CLOCKS_PER_SEC;
        printf("Approximate :-> Time: %f\n", time);
        fprintf(fp , "%d %f\n" , thread , time);
    }

    fclose(fp);


    // Precise

    FILE* fp2 = fopen("data2.txt" , "w");
    if (fp2 == NULL){
        printf("Error:Could not open file\n");
        exit(1);
    }

    for (int thread = 1;  thread <=4 ; thread++){

        int num_threads = thread;
        pthread_t threads[num_threads];
        printf("Precise :-> Counter: %d\n" , counter.value);
        
        clock_t start = clock();
        
        for (int i = 0 ; i<num_threads ; i++){
            pthread_create(&threads[i] , NULL , increment , NULL);
        }

        for (int i = 0 ; i<num_threads ; i++){
            pthread_join(threads[i] , NULL);
        }

        clock_t end = clock();

        printf("Precise :-> Counter: %d\n" , counter.value);
        double time = (double)(end-start)/CLOCKS_PER_SEC;
        printf("Precise :-> Time: %f\n", time);
        fprintf(fp2 , "%d %f\n" , thread , time);
    }

    fclose(fp2);

    // Performance Vs Threshold factor

    FILE* fp3 = fopen("data3.txt" , "w");
    if (fp3 == NULL){
        printf("Error:Could not open file\n");
        exit(1);
    }

    for (int threshold=1 ; threshold<=1024 ; threshold*=2){
        
        int num_threads = 4;
        counter_t counter2;
        
        init(&counter2 , threshold);
        
        // printf("Approximate :-> Starting Counter: %d\n" , counter2.global);
        clock_t start = clock();

        pthread_t threads[num_threads];

        for (int i=0 ; i<num_threads ; i++){
            func_arg argmts;
            argmts.amount = 1;
            argmts.counter = &counter2;
            argmts.thread_id = i;
            pthread_create(&threads[i] , NULL , update , &argmts);
        }

        for (int i=0 ; i<num_threads ; i++){
            pthread_join(threads[i] , NULL);
        }

        clock_t end = clock();

        // printf("Approximate :-> Ending Counter: %d\n" , counter2.global);
        double time = (double)(end-start)/CLOCKS_PER_SEC;
        // printf("Approximate :-> Time: %f\n", time);
        fprintf(fp3 , "%d %f\n" , threshold , time);

    }
    fclose(fp3);
    

    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL){
        printf("Error: Could not open pipe to GNUplot\n");
        exit(1);
    }

    // fprintf(gnuplotPipe , "set terminal png size 1000,600\n");
    // fprintf(gnuplotPipe , "set output 'multiplot.png'\n");

    fprintf(gnuplotPipe, "set multiplot layout 1, 2 title 'Multiple Graphs'\n");
    
    fprintf(gnuplotPipe, "set xtics font ',3'\n");  // Set font for x-axis ticks
    fprintf(gnuplotPipe, "set ytics font ',3'\n");  // Set font for y-axis ticks
    fprintf(gnuplotPipe, "set key font ',4'\n");
    fprintf(gnuplotPipe, "set title 'Performance Vs No. of Threads' font ',4'\n");
    fprintf(gnuplotPipe, "set xlabel 'No. of Threads' font ',4'\n");
    fprintf(gnuplotPipe, "set ylabel 'Performance' font ',4'\n");
    fprintf(gnuplotPipe, "plot 'data1.txt' using 1:2 with linespoints title 'Approximate', \\\n");
    fprintf(gnuplotPipe, "     'data2.txt' using 1:2 with linespoints title 'Precise'\n");


    fprintf(gnuplotPipe, "set xtics font ',3'\n");  // Set font for x-axis ticks
    fprintf(gnuplotPipe, "set ytics font ',3'\n");  // Set font for y-axis ticks
    fprintf(gnuplotPipe, "set key font ',4'\n");
    fprintf(gnuplotPipe, "set title 'Performance Vs Threshold Factor' font ',4'\n");
    fprintf(gnuplotPipe, "set xlabel 'Threshold Factor' font ',4'\n");
    fprintf(gnuplotPipe, "set ylabel 'Performance' font ',4'\n");
    fprintf(gnuplotPipe, "plot 'data3.txt' using 1:2 with linespoints title 'Pagal'\n");

    fprintf(gnuplotPipe, "unset multiplot\n");
    

    fflush(gnuplotPipe);  // Ensure all commands are sent to GNUplot
    pclose(gnuplotPipe);  // Close the pipe




    return 0;
}
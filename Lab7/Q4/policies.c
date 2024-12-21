#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void opt_policy(int page_seq[] , int opt_hit[] , int total_ref){
    for (int cache_size = 1 ; cache_size<=100 ; cache_size++){

        int cache[cache_size];
        for (int i=0 ; i<cache_size ; i++) cache[i] = -1;  // initializing cache to make it empty
        
        int hits = 0;
        
        for (int i=0 ; i<total_ref ; i++){
            bool flag = false;
            int page = page_seq[i];
            int empty = false;

            // finding if page is in cache
            for (int j=0 ; j<cache_size ; j++){
                if (cache[j] == page){
                    flag = true;
                    break;
                }
                if (cache[j] == -1) empty = true;
            }

            if (flag) hits++;
            else if (empty){
                // if cache is not full
                for (int j=0 ; j<cache_size ; j++){
                    if (cache[j] == -1){
                        cache[j] = page;
                        break;
                    }
                }
            }
            else{
                // if cache is full , need to evict
                int temp = 0;
                int temp_arr[cache_size];
                for (int j=0 ; j<cache_size ; j++){
                    temp_arr[j] = 0;
                }
                for (int j = i+1 ; j<total_ref ; j++){
                    if (temp >= cache_size - 1){
                        break;
                    }
                    for (int k = 0 ; k<cache_size ; k++){
                        if (page_seq[j] == cache[k]){
                            if (temp_arr[k] == 0){
                                temp_arr[k]++;
                                temp++;
                            }
                            break;
                        }
                    }
                }
                for (int j=0 ; j<cache_size ; j++){
                    if (temp_arr[j] == 0){
                        cache[j] = page;
                        break;
                    }
                }
            }
        }
        opt_hit[cache_size - 1] = hits;
    }
    return;
}

void rand_policy(int page_seq[] , int rand_hit[] , int total_ref){
    for (int cache_size = 1 ; cache_size<=100 ; cache_size++){

        int cache[cache_size];
        for (int i=0 ; i<cache_size ; i++) cache[i] = -1;  // initializing cache to make it empty
        
        int hits = 0;
        
        for (int i=0 ; i<total_ref ; i++){
            bool flag = false;
            int page = page_seq[i];
            int empty = false;

            // finding if page is in cache
            for (int j=0 ; j<cache_size ; j++){
                if (cache[j] == page){
                    flag = true;
                    break;
                }
                if (cache[j] == -1) empty = true;
            }

            if (flag) hits++;
            else if (empty){
                // if cache is not full
                for (int j=0 ; j<cache_size ; j++){
                    if (cache[j] == -1){
                        cache[j] = page;
                        break;
                    }
                }
            }
            else{
                // if cache is full , need to evict
                int to_evict = rand()%cache_size;
                cache[to_evict] = page;
            }
        }
        rand_hit[cache_size - 1] = hits;
    }
    return;

}

void fifo_policy(int page_seq[] , int fifo_hit[] , int total_ref){
    for (int cache_size = 1 ; cache_size<=100 ; cache_size++){

        int cache[cache_size];
        for (int i=0 ; i<cache_size ; i++) cache[i] = -1;  // initializing cache to make it empty
        
        int hits = 0;
        int curr_size = 0;

        for (int i=0 ; i<total_ref ; i++){
            bool flag = false;
            int page = page_seq[i];

            // finding if page is in cache
            for (int j=0 ; j<curr_size ; j++){
                if (cache[j] == page){
                    flag = true;
                    break;
                }
            }

            if (flag) hits++;
            else if (curr_size < cache_size){
                // if cache is not full
                cache[curr_size++] = page;
            }
            else{
                // if cache is full , need to evict
                for (int j = 0; j<curr_size-1 ; j++) cache[j] = cache[j+1];
                cache[curr_size-1] = page;
            }
        }
        fifo_hit[cache_size - 1] = hits;
    }
    return;
}

void lru_policy(int page_seq[] , int lru_hit[] , int total_ref){
    for (int cache_size = 1 ; cache_size<=100 ; cache_size++){

        int cache[cache_size];
        for (int i=0 ; i<cache_size ; i++) cache[i] = -1;  // initializing cache to make it empty
        
        int hits = 0;
        int curr_size = 0;

        for (int i=0 ; i<total_ref ; i++){
            bool flag = false;
            int page = page_seq[i];

            // finding if page is in cache
            int idx = -1;
            for (int j=0 ; j<curr_size ; j++){
                if (cache[j] == page){
                    flag = true;
                    idx = j;
                    break;
                }
            }

            if (idx != -1){
                for (int k=idx ; k>0 ; k--){
                    cache[k] = cache[k-1]; 
                }
                cache[0] = page;
            }

            if (flag) hits++;
            else if (curr_size < cache_size){
                // if cache is not full
                for (int k = curr_size-1 ; k>0 ; k--){
                    cache[k] = cache[k-1];
                }
                cache[0] = page;
                curr_size++;
            }
            else{
                // if cache is full , need to evict
                for (int j = curr_size-1 ; j> 0 ; j--) cache[j] = cache[j-1];
                cache[0] = page;
            }
        }
        lru_hit[cache_size - 1] = hits;
    }
    return;
}

void approx_lru(int page_seq[] , int approx_hit[] , int total_ref){
    for (int cache_size = 1 ; cache_size<=100 ; cache_size++){

        int cache[cache_size];
        for (int i=0 ; i<cache_size ; i++) cache[i] = -1;  // initializing cache to make it empty
        
        int reference_bits[101];
        for (int i=0 ; i<101 ; i++){
            reference_bits[i] = 0;
        }

        int hits = 0;
        int curr_size = 0;

        for (int i=0 ; i<total_ref ; i++){
            bool flag = false;
            int page = page_seq[i];

            // finding if page is in cache
            int idx = -1;
            for (int j=0 ; j<cache_size ; j++){
                if (cache[j] == page){
                    flag = true;
                    idx = j;
                    break;
                }
            }

            if (idx != -1){
                reference_bits[page] = 1;   
            }

            if (flag) hits++;
            else if (curr_size < cache_size){
                // if cache is not full
                for (int j = 0; j<cache_size ; j++){
                    if (cache[j] == -1){
                        cache[j] = page;
                        reference_bits[page] = 1;
                        curr_size++;
                        break;
                    }
                }
            }
            else{
                int clock_hand = rand()%cache_size;
                // if cache is full , need to evict
                while (reference_bits[cache[clock_hand]] == 1){
                    reference_bits[cache[clock_hand]] = 0;
                    clock_hand = (1 + clock_hand)% cache_size;
                }

                cache[clock_hand] = page;
                reference_bits[page] = 1;

            }
        }
        approx_hit[cache_size - 1] = hits;
    }
    return;
}

void no_workload(){
    int page_seq[10000];
    for (int i=0 ; i<10000 ; i++){
        int gen_page =  1 + (rand()%100);  // No Locality Workload
        page_seq[i] = gen_page;
    }
    
    FILE* file = fopen("data1.csv" , "w");
    fprintf(file , "x,y1,y2,y3,y4,y5\n");

    // Optimum Policy
    int opt_hit[100];
    opt_policy(page_seq , opt_hit , 10000);

    // Random Policy
    int rand_hit[100];
    rand_policy(page_seq , rand_hit , 10000);

    // FIFO Policy
    int fifo_hit[100];
    fifo_policy(page_seq , fifo_hit , 10000);

    // LRU Policy
    int lru_hit[100];
    lru_policy(page_seq , lru_hit , 10000);

    // APPROX LRU Policy
    int approx_hit[100];
    approx_lru(page_seq , approx_hit , 10000);

    for (int i=0 ; i<100 ; i++){
        fprintf(file , "%d,%d,%d,%d,%d,%d\n", i+1 , opt_hit[i] , rand_hit[i] , fifo_hit[i] , lru_hit[i] , approx_hit[i]);
    }

}

void workload_80_20(){

    int pages_20[20];
    int pages_80[80];
    for (int i=0 ; i<20 ; i++){
        pages_20[i] = 1 + (rand()%100);
    }
    int flags[101];
    for (int i=0 ; i<101 ; i++){
        flags[i] = 0;
    }
    for (int i=0 ; i<20 ; i++){
        flags[pages_20[i]] = 1;
    }
    int j = 0;
    for (int i=1 ; i<101 ; i++){
        if (flags[i] != 1){
            pages_80[j++] = i;
        }
    }

    int page_seq[10000];
    for (int i=0 ; i<10000 ; i++){
        int num = 1 + (rand()%100);
        if (num <= 80){
            int num2 = rand()%20;
            page_seq[i] = pages_20[num2];
        }
        else{
            int num2 = rand()%80;
            page_seq[i] = pages_80[num2];
        }
    }

    FILE* file = fopen("data2.csv" , "w");
    fprintf(file , "x,y1,y2,y3,y4,y5\n");    

    // Optimum Policy
    int opt_hit[100];
    opt_policy(page_seq , opt_hit , 10000);

    // Random Policy
    int rand_hit[100];
    rand_policy(page_seq , rand_hit , 10000);

    // FIFO Policy
    int fifo_hit[100];
    fifo_policy(page_seq , fifo_hit , 10000);

    // LRU Policy
    int lru_hit[100];
    lru_policy(page_seq , lru_hit , 10000);

    // APPROX LRU Policy
    int approx_hit[100];
    approx_lru(page_seq , approx_hit , 10000);

    for (int i=0 ; i<100 ; i++){
        fprintf(file , "%d,%d,%d,%d,%d,%d\n", i+1 , opt_hit[i] , rand_hit[i] , fifo_hit[i] , lru_hit[i] , approx_hit[i]);
    }

}

void looping_sequential_workload(){
    
    int page_seq[10000];
    for (int i=0 ; i<10000 ; i++){
        page_seq[i] = i%50;
    }
    
    FILE* file = fopen("data3.csv" , "w");
    fprintf(file , "x,y1,y2,y3,y4,y5\n");

    // Optimum Policy
    int opt_hit[100];
    opt_policy(page_seq , opt_hit , 10000);

    // Random Policy
    int rand_hit[100];
    rand_policy(page_seq , rand_hit , 10000);

    // FIFO Policy
    int fifo_hit[100];
    fifo_policy(page_seq , fifo_hit , 10000);

    // LRU Policy
    int lru_hit[100];
    lru_policy(page_seq , lru_hit , 10000);


    // APPROX LRU Policy
    int approx_hit[100];
    approx_lru(page_seq , approx_hit , 10000);

    for (int i=0 ; i<100 ; i++){
        fprintf(file , "%d,%d,%d,%d,%d,%d\n", i+1 , opt_hit[i] , rand_hit[i] , fifo_hit[i] , lru_hit[i] , approx_hit[i]);
    }

}

int main(){
    no_workload();
    workload_80_20();
    looping_sequential_workload();
    return 0;
}
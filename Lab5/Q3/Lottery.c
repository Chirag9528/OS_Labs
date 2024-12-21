#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int compare(const void* a , const void* b){
    int l = *((int*)a);
    int r = *((int*)b);
    return (r-l);
}

int gcd(int a , int b){
    if (b == 0) return a;
    return gcd(b , a%b);
}

int lcm(int a,  int b){
    return (a*b)/gcd(a, b);
}

int main(){
    int n;
    scanf("%d" , &n);

    srand(time(0));

    int tickets[n];
    int total = 0;
    for (int i=0 ; i<n ; i++){
        tickets[i] = 1 +  (rand()%400);
        total+= tickets[i];
    }

    qsort(tickets , n , 4 , compare);
    
    int freq_lottery[n];
    int freq_stride[n];
    for (int i=0 ; i<n ; i++){
        freq_lottery[i] = 0;
        freq_stride[i] = 0;
    }

    for (int i=0 ; i<1000; i++){
        int num = rand()%total;
        int curr = 0;
        for (int j=0 ; j<n ; j++){
            curr+= tickets[j];
            if (curr > num){
                freq_lottery[j]++;
                break;
            }
        }
    }

    int stride[n];
    int number = 800;
    // for (int i=0 ; i<n ; i++){
    //     number = lcm(number , tickets[i]);
    // }

    for (int i=0 ; i<n ; i++){
        stride[i] = number/tickets[i];
    }

    int passvalue[n];
    for (int i=0 ; i<n  ; i++){
        passvalue[i] = 0;
    }


    for (int i=0; i<1000 ; i++){
        int minval = passvalue[0];
        int idx = 0;
        for (int j=1 ; j<n ; j++){
            if (passvalue[j] < minval){
                minval = passvalue[j];
                idx = j;
            }
        }
        freq_stride[idx]++;
        passvalue[idx]+= stride[idx];

    }

    for (int i=0 ; i<n ; i++){
        printf("%d ", tickets[i]);
    }
    printf("\n");

    for (int i=0 ; i<n ; i++){
        printf("%d ", freq_lottery[i]);
    }
    printf("\n");

    for (int i=0 ; i<n ; i++){
        printf("%d ", freq_stride[i]);
    }
    printf("\n");
    
    return 0;
}
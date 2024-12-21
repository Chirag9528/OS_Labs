#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

void opendirectory(char* path, int tab){
    bool found = false;
    int length = 0;
    int last_index = 0;
    while (path[length] != '\0'){
        if (path[length] == '/'){
            last_index = length;
            found = true;
        }
        length++;
    }

    if (!found){
        printf("%s\n" , path);
    }
    else{
        char path_name[1024];
        int k = 0;
        for (int z = last_index+1 ; z < length ; z++){
            path_name[k++] = path[z];
        }
        path_name[k] = '\0';
        printf("%s\n" , path_name);
    }

    DIR* dp = opendir(path);
    if (dp == NULL){
        fprintf(stderr , "Error: Enter a valid Directory\n");
        exit(1);
    }
    struct dirent* d;
    while ((d = readdir(dp)) != NULL){
        if ((strcmp(d->d_name , ".") == 0)  || (strcmp(d->d_name , "..") == 0)){
            continue;
        }
        struct stat sb;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, d->d_name);
        // printf("Full path: %s\n" , full_path);
        if (lstat(full_path , &sb) == -1){
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        if ((sb.st_mode & S_IFMT) == S_IFDIR){
            for (int l = 0; l<tab ; l++){
               printf("|\t");
            }
            printf("|-");
            opendirectory(full_path , tab+1);
        }
        else{
            for (int l=0 ; l<tab ; l++){
                printf("|\t");
            }
            printf("|-%s\n", d->d_name);
        }

    }
}

int main(int argc , char* argv[]){
    if (argc < 2){
        fprintf(stderr , "Additional Arguements missing\n");
        exit(1);
    }
    struct stat sb;
    if (lstat(argv[1] , &sb) == -1){
        perror("lstat");
        exit(EXIT_FAILURE);
    }
    if ((sb.st_mode & S_IFMT) != S_IFDIR){
        fprintf(stderr , "It is not a directory\n");
        exit(1);
    }
    opendirectory(argv[1] , 0);
    return 0;
}
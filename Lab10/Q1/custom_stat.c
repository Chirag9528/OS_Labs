#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
int main(int argc , char* argv[]){
    if (argc < 2){
        fprintf(stderr , "Additional arguement is missing!\n");
        exit(1);
    }
    struct stat sb;
    if (lstat(argv[1] , &sb) == -1){
        perror("lstat");
        exit(1);
    }

    printf("File size:                %jd bytes\n",(intmax_t) sb.st_size);
    printf("Blocks allocated:         %jd\n", (intmax_t) sb.st_blocks);
    printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);

    return 0;
}
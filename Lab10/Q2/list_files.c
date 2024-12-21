#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>

int main(int argc , char* argv[]){
    if (argc < 2){
        fprintf(stderr , "Additional arguements are missing!\n");
        exit(EXIT_FAILURE);
    }
    DIR *dp = opendir(argv[1]);
    assert(dp != NULL);
    struct dirent *d;
    while ((d = readdir(dp)) != NULL) {
        struct stat sb;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", argv[1], d->d_name);
        printf("Full path: %s\n" , full_path);
        if (lstat(full_path , &sb) == -1){
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        if (((sb.st_mode & S_IFMT) == S_IFREG) || ((sb.st_mode & S_IFMT) == S_IFLNK) || ((sb.st_mode & S_IFMT) == S_IFDIR)){
            printf("File : %s\n",d->d_name);
            int pflag = 0;
            int sflag = 0;
            char perm[11];
            if (argc == 3){
                char* flag = argv[2];
                if (strcmp(flag , "-p") == 0){
                    pflag = 1;
                }
                else if (strcmp(flag , "-s") == 0){
                    sflag = 1;
                }
                else{
                    fprintf(stderr , "Error: flag error\n");
                    exit(EXIT_FAILURE);
                }
                
            }
            else if (argc == 4){
                char* flag = argv[2];
                if (strcmp(flag , "-p") == 0) pflag = 1;
                else if (strcmp(flag , "-s") == 0) sflag = 1;
                else{
                    fprintf(stderr , "Error: flag error\n");
                    exit(EXIT_FAILURE);
                }

                char* flag2 = argv[3];
                if (strcmp(flag2 , "-p") == 0) pflag = 1;
                else if (strcmp(flag2 , "-s") == 0) sflag = 1;
                else{
                    fprintf(stderr , "Error: flag error\n");
                    exit(EXIT_FAILURE);
                }
            }
            if (sflag) printf("File Size: %jd bytes\n" , (intmax_t)sb.st_size);
            if (pflag){
                
                if ((sb.st_mode & S_IFMT) == S_IFREG) perm[0] = 'f';
                else if ((sb.st_mode & S_IFMT) == S_IFLNK) perm[0] = 'l';
                else perm[0] = 'd';

                // User permissions
                if (sb.st_mode & S_IRUSR) perm[1] = 'r'; else perm[1] = '-';
                if (sb.st_mode & S_IWUSR) perm[2] = 'w'; else perm[2] = '-';
                if (sb.st_mode & S_IXUSR) perm[3] = 'x'; else perm[3] = '-';

                // Group permissions
                if (sb.st_mode & S_IRGRP) perm[4] = 'r'; else perm[4] = '-';
                if (sb.st_mode & S_IWGRP) perm[5] = 'w'; else perm[5] = '-';
                if (sb.st_mode & S_IXGRP) perm[6] = 'x'; else perm[6] = '-';

                // Others permissions
                if (sb.st_mode & S_IROTH) perm[7] = 'r'; else perm[7] = '-';
                if (sb.st_mode & S_IWOTH) perm[8] = 'w'; else perm[8] = '-';
                if (sb.st_mode & S_IXOTH) perm[9] = 'x'; else perm[9] = '-';
                perm[10] = '\0';
                printf("Permissions: %s\n" , perm);
            }
            
            printf("----------------------\n\n");
        }

    }
    return 0;
}
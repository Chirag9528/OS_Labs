#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // For open()
#include <unistd.h>   // For close()
#include <stdbool.h>

int main(int argc , char* argv[]){
    if (argc < 3){
        fprintf(stderr , "Some Additional Arguements are missing\n");
        exit(1);
    }
    int fd = open(argv[2] , O_RDONLY);
    if (fd == -1){
        fprintf(stderr , "File isn't opened\n");
        exit(1);
    }
    int num = -atoi(argv[1]);  // no of lines needed to print
    int count = 0;

    lseek(fd , -1 , SEEK_END);
    
    char end_chr;
    read(fd , &end_chr , 1);
    if (end_chr != '\n') count =1;  // checking whether last character is '\n'

    while (true){
        off_t curr = lseek(fd , 0 , SEEK_CUR); // to store current file pointer
        off_t val = lseek(fd , -512 , SEEK_CUR);  // it will return -1 if reqd bytes are not available
        int size = curr - val; // tracking size of buffer

        if (val == -1){
            lseek(fd , 0 , SEEK_SET);
            size = curr;  // tracking size for buffer
        }

        char buffer[size+1];
        int chars_read = read(fd , buffer , size);
        buffer[size] = '\0';

        off_t now = lseek(fd , -size , SEEK_CUR);

        int flag = 0;
        for (int j=size-1 ; j>=0 ; j--){
            if (buffer[j] == '\n') count++;
            if (count == num+1){
                flag = 1;
                lseek(fd , j+1 , SEEK_CUR); // jumping to the correct location from where contents need to print
                break;
            }
        }
        if (flag) break;
        off_t curr_pos = lseek(fd , 0 , SEEK_CUR);
        if (curr_pos == 0) break; // if reaching the start point
    }

    char buff[513];
    int val = 1;
    while (val){
        val = read(fd , buff , 512); // return the number of bytes read
        buff[val] = '\0';
        printf("%s", buff);
    }
    return 0;
}
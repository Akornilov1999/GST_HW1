#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Error!\nYou didn't write number of matrices or outut file path!\n");
        exit(0);
    }
    int count_of_matrices = 0;
    if ((count_of_matrices = atoi(argv[1])) < 1){
        printf("Error!\nCount of matrices must be more than 0!\n");
        exit(0);
    }
    int fd = 0;
    if ((fd = open(argv[2], O_WRONLY | O_TRUNC)) < 0){
        printf("Error!\nThere is no such file or directory!\n");
        exit(0);
    }
    srand(time(NULL));
    int out = dup(1);
    dup2(fd, 1);
    for (int number_of_matrix = 0; number_of_matrix < count_of_matrices; number_of_matrix++){
        int count_of_rows = rand()%9 + 1;
        int count_of_colums = rand()%9 + 1;
        printf("%d %d\n", count_of_rows, count_of_colums);
        for (int row = 0; row < count_of_rows; row++){
            for (int column = 0; column < count_of_colums; column++){
                printf("%d ", rand()%999 + 1);
            }
            printf("\n");
        }
    }
    write(out, "Success!\n", 9);
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[]){
    int start_time = clock();
    if (argc < 3){
        printf("Error!\nYou didn't write input or outut file paths!\n");
        exit(0);
    }
    int fd_input = 0;
    if ((fd_input = open(argv[1], O_RDONLY)) < 0){
        printf("Error!\nThere is no such file or directory!\n");
        exit(0);
    }
    int fd_output = 0;
    if ((fd_output = open(argv[2], O_WRONLY | O_TRUNC)) < 0){
        printf("Error!\nThere is no such file or directory!\n");
        exit(0);
    }
    int out = dup(1);
    dup2(fd_output, 1);
    int end_position = lseek(fd_input, 0, SEEK_END);
    lseek(fd_input, 0, SEEK_SET);
    while (lseek(fd_input, 0, SEEK_CUR) != end_position){
        char dimension[5];
        read(fd_input, dimension, 4);
        char rows_buffer[2];
        rows_buffer[0] = dimension[0];
        char columns_buffer[2];
        columns_buffer[0] = dimension[2];
        int count_of_rows = atoi(rows_buffer);
        int count_of_columns = atoi(columns_buffer);
        if (count_of_rows == 0 || count_of_columns == 0){
                    write(out, "Error!\nInput file corrupted!\n", 29);
                    exit(0);
                }
        printf("%d %d\n", count_of_rows, count_of_columns);
        int new_matrix[count_of_rows][count_of_columns];
        for (int row = 0; row < count_of_rows; row++){
            for (int column = count_of_columns - (row%count_of_columns); column < count_of_columns; column++){
                char current_element[5];
                read(fd_input, current_element, 4);
                new_matrix[row][column] = atoi(current_element);
                if (new_matrix[row][column] == 0){
                    write(out, "Error!\nInput file corrupted!\n", 29);
                    exit(0);
                }
                int bit_depth = 100;
                while(1){
                    if (new_matrix[row][column]/bit_depth == 0){
                        bit_depth /= 10;
                        lseek(fd_input, -1, SEEK_CUR);
                    }
                    else break;
                }
            }
            for (int column = 0; column < count_of_columns - (row%count_of_columns); column++){
                char current_element[5];
                read(fd_input, current_element, 4);
                new_matrix[row][column] = atoi(current_element);
                if (new_matrix[row][column] == 0){
                    write(out, "Error!\nInput file corrupted!\n", 29);
                    exit(0);
                }
                int bit_depth = 100;
                while(1){
                    if (new_matrix[row][column]/bit_depth == 0){
                        bit_depth /= 10;
                        lseek(fd_input, -1, SEEK_CUR);
                    }
                    else break;
                }
            }
            for (int column = 0; column < count_of_columns; column++){
                printf("%d ", new_matrix[row][column]);
            }
            printf("\n");
            lseek(fd_input, 1, SEEK_CUR);
        }
    }
    write(out, "Success!\n", 9);
    printf("Размер обработанных данных: %f (Мб)\n", ((((float) end_position) / 1024.0) / 1024.0));
    int finish_time = clock();
    printf("Время выполнения вычислений: %d (мкс)\n", finish_time - start_time);
}
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define W 80  // 80
#define H 25  // 25
#define SPEED 1000000

void alloc(int ***array, int height, int width);
void output(int **matrix, int height, int width);
int start_init(int ***array, int height, int width, int param);
int keyboard_input(int ***array, int height, int width);
int get_count_of_neig(int **matrix, int heigth, int width, int x, int y);
void neighbours(int **matrix, int heigth, int wight);
int menu();

int main() {
    int param = menu();
    while (param == -1) {
        printf("Ошибка при получении номера");
        param = menu();
    }

    int **matrix = NULL;
    if (param == 7) {
        if (keyboard_input(&matrix, H, W)) {
            printf("Ошибка при вводе расстановки. Запускается режим из файла 1");
            free(matrix);
            start_init(&matrix, H, W, 1);
        } else {
        }
    } else {
        start_init(&matrix, H, W, param);
    }

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    move(0, 0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    refresh();
    char key = ' ';
    int speed = SPEED;
    output(matrix, H, W);
    refresh();
    move(0, 0);
    while (key != 'q') {
        key = getch();
        if (key == ERR) neighbours(matrix, H, W);
        if ((key == 'a' || key == 'A') && speed > 100) speed /= 2;
        if ((key == 'z' || key == 'Z') && speed < SPEED) speed *= 2;

        clear();
        output(matrix, H, W);
        refresh();
        move(0, 0);
        usleep(speed);
    }
    clear();
    move(0, 0);
    sleep(1);
    printw("Спасибо за игру!");
    refresh();
    sleep(1);
    free(matrix);
    endwin();
    return 0;
}

void alloc(int ***array, int height, int width) {
    *array = (int **)malloc(height * width * sizeof(int) + height * sizeof(int *));
    if (*array) {
        int *ptr = (int *)((*array) + height);
        for (int i = 0; i < height; i++) {
            (*array)[i] = ptr + i * (width);
        }
    }
}

void output(int **matrix, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (matrix[i][j] == 0) printw("-");
            if (matrix[i][j] == 1) printw("O");
            if (j != width - 1) printw(" ");
        }
        if (i != height - 1) printw("\n");
    }
}

int start_init(int ***array, int height, int width, int param) {
    FILE *fp = NULL;
    switch (param) {
        case 1:
            fp = fopen("1.txt", "r");
            break;
        case 2:
            fp = fopen("2.txt", "r");
            break;
        case 3:
            fp = fopen("3.txt", "r");
            break;
        case 4:
            fp = fopen("4.txt", "r");
            break;
        case 5:
            fp = fopen("5.txt", "r");
            break;
        case 6:
            fp = fopen("6.txt", "r");
            break;
    }
    int flag = 0;
    alloc(array, height, width);
    if (fp == NULL) {
        flag = 1;
    } else {
        char c;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                c = getc(fp);
                if (c == '\n') {
                    j--;
                    continue;
                }
                (*array)[i][j] = (int)c - 48;
            }
        }
    }
    return flag;
}

int keyboard_input(int ***array, int height, int width) {
    alloc(array, height, width);
    int flag = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            if (scanf("%d", &((*array)[i][j])) != 1) {
                flag = 1;
                break;
            }
        }
    return flag;
}

int get_count_of_neig(int **matrix, int heigth, int width, int x, int y) {
    int count = 0;

    if (matrix[(x + 1) % heigth][y] == 1) count++;
    if (matrix[(x - 1 + heigth) % heigth][y] == 1) count++;
    if (matrix[x][(y + 1) % width] == 1) count++;
    if (matrix[x][(y - 1 + width) % width] == 1) count++;

    if (matrix[(x + 1) % heigth][(y + 1) % width] == 1) count++;
    if (matrix[(x + 1) % heigth][(y - 1 + width) % width] == 1) count++;

    if (matrix[(x - 1 + heigth) % heigth][(y + 1) % width] == 1) count++;
    if (matrix[(x - 1 + heigth) % heigth][(y - 1 + width) % width] == 1) count++;

    return count;
}

void neighbours(int **matrix, int heigth, int wight) {
    int **empty_matrix;
    alloc(&empty_matrix, H, W);

    for (int i = 0; i < heigth; ++i) {
        for (int j = 0; j < wight; ++j) {
            int count = 0;
            count = get_count_of_neig(matrix, heigth, wight, i, j);

            if ((matrix[i][j] == 1 && count == 2) || count == 3) {
                empty_matrix[i][j] = 1;
            } else {
                empty_matrix[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < heigth; ++i) {
        for (int j = 0; j < wight; ++j) {
            matrix[i][j] = empty_matrix[i][j];
        }
    }
    free(empty_matrix);
}

int menu() {
    printf("Выберите начальную расстановку для игры\n");
    printf("1 - 6: Ввод из файла\n");
    printf("7: Ввод с клавиутуры\n");
    int choice = 0;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 7) choice = -1;
    return choice;
}

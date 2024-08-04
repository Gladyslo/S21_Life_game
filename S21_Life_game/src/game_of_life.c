#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define ROW 25
#define COLUMNS 80
#define SLOW 200000
#define MEDIUM 100000
#define FAST 25000

void input(int pole[ROW][COLUMNS]);
void check_live(int pole[ROW][COLUMNS], int new_pole[][COLUMNS], int* signal_for_stop);
void speed_end_set(char* interactive, int* delay, int* signal_for_stop);
void score_calculating(int pole[ROW][COLUMNS], int new_pole[][COLUMNS]);
void output(int new_pole[][COLUMNS]);
void array_swap(int pole[ROW][COLUMNS], int new_pole[][COLUMNS]);

int main() {
    initscr();
    nodelay(stdscr, true);
    int delay = SLOW;
    int signal_for_stop = 1;
    int pole[ROW][COLUMNS];
    input(pole);
    if (freopen("/dev/tty", "r", stdin) != 0) {
        output(pole);
        int new_pole[ROW][COLUMNS] = {0};
        while (signal_for_stop) {
            char interactive_command = getch();
            score_calculating(pole, new_pole);
            usleep(delay);
            clear();
            output(new_pole);
            check_live(pole, new_pole, &signal_for_stop);
            speed_end_set(&interactive_command, &delay, &signal_for_stop);
            array_swap(pole, new_pole);
        }
    }
    endwin();
    return 0;
}

void input(int pole[ROW][COLUMNS]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            scanf("%d", &pole[i][j]);
        }
    }
}

void check_live(int pole[ROW][COLUMNS], int new_pole[][COLUMNS], int* signal_for_stop) {
    int flag = 0;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (new_pole[i][j] != pole[i][j]) {
                flag++;
            }
        }
    }
    if (flag > 0) {
        *signal_for_stop = 1;
    } else {
        *signal_for_stop = 0;
    }
}

void speed_end_set(char* interactive, int* delay, int* signal_for_stop) {
    switch (*interactive) {
        case '1':
            *delay = SLOW;
            break;
        case '2':
            *delay = MEDIUM;
            break;
        case '3':
            *delay = FAST;
            break;
        case 'q':
            *signal_for_stop = 0;
            break;
        default:
            break;
    }
}

void score_calculating(int pole[ROW][COLUMNS], int new_pole[][COLUMNS]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            int sum = 0;
            for (int ni = i - 1; ni <= i + 1; ni++) {
                for (int nj = j - 1; nj <= j + 1; nj++) {
                    if (ni == i && nj == j) continue;
                    if (pole[(ni + ROW) % ROW][(nj + COLUMNS) % COLUMNS] == 1) sum++;
                }
            }
            if (pole[i][j] == 1) {
                if (sum < 2 || sum > 3)
                    new_pole[i][j] = 0;
                else
                    new_pole[i][j] = 1;
            } else {
                if (sum == 3)
                    new_pole[i][j] = 1;
                else
                    new_pole[i][j] = 0;
            }
        }
    }
}

void array_swap(int pole[ROW][COLUMNS], int new_pole[][COLUMNS]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            pole[i][j] = new_pole[i][j];
        }
    }
}

void output(int new_pole[][COLUMNS]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (new_pole[i][j] == 0) {
                printw(".");
            }
            if (new_pole[i][j] == 1) {
                printw("O");
            }
        }
        printw("\n");
    }
}
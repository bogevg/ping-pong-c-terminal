#include <curses.h>
#include <stdio.h>
#include <unistd.h>

int is_ball_on_paddle(int ball_x, int ball_y, int l_paddle_y, int r_paddle_y, int Width);
int is_ball_missed(int ball_x, int ball_y, int l_paddle_y, int r_paddle_y, int width);
int move_l_paddle(char v, int l_paddle_y, int height);
int move_r_paddle(char v, int r_paddle_y, int height);
void print_screen(int l_paddle, int r_paddle, int ball_x, int ball_y, int score_a, int score_b, int width,
                  int height);
void print_information(int score_a, int score_b);
void win_screen(int player);

int main() {
    const int Width = 80;
    const int Height = 25;
    const int Win = 21;
    int missed_ball;
    int ball_x = 39;
    int ball_y = 12;
    int index_x = 1;
    int index_y = 1;
    int l_paddle_y = 11;
    int r_paddle_y = 11;
    int score_a = 0, score_b = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    clear();

    print_screen(l_paddle_y, r_paddle_y, ball_x, ball_y, score_a, score_b, Width, Height);

    while (score_a != Win && score_b != Win) {
        timeout(70);
        char ch = getch();
        if (ch == ERR || ch == 'z' || ch == 'Z' || ch == 'm' || ch == 'M' || ch == 'a' || ch == 'A' ||
            ch == 'k' || ch == 'K') {
            l_paddle_y = move_l_paddle(ch, l_paddle_y, Height);
            r_paddle_y = move_r_paddle(ch, r_paddle_y, Height);

            missed_ball = is_ball_missed(ball_x, ball_y, l_paddle_y, r_paddle_y, Width);

            if (!missed_ball) {
                if (ball_x <= 1 || ball_x >= Width - 2 ||
                    is_ball_on_paddle(ball_x, ball_y, l_paddle_y, r_paddle_y, Width)) {
                    index_x *= -1;
                }
                if (ball_y <= 1 || ball_y >= Height - 2) {
                    index_y *= -1;
                }

                ball_x += index_x;
                ball_y += index_y;

            } else {
                if (ball_x + index_x <= Width / 2) {
                    score_b += 1;
                } else {
                    score_a += 1;
                }
                ball_x = 39;
                ball_y = 12;
            }
        }
        print_screen(l_paddle_y, r_paddle_y, ball_x, ball_y, score_a, score_b, Width, Height);
    }

    win_screen(score_a > score_b ? 0 : 1);

    endwin();

    return 0;
}

int is_ball_on_paddle(int ball_x, int ball_y, int l_paddle_y, int r_paddle_y, int Width) {
    return ((ball_x == Width - 3) && (r_paddle_y <= ball_y && ball_y <= r_paddle_y + 2)) ||
           (ball_x == 2 && (l_paddle_y <= ball_y && ball_y <= l_paddle_y + 2));
}

int is_ball_missed(int ball_x, int ball_y, int l_paddle_y, int r_paddle_y, int width) {
    if ((ball_x <= 1 && !(l_paddle_y <= ball_y && ball_y <= l_paddle_y + 2)) ||
        (ball_x >= width - 2 && !(r_paddle_y <= ball_y && ball_y <= r_paddle_y + 2))) {
        return 1;
    } else {
        return 0;
    }
}

int move_l_paddle(char v, int l_paddle_y, int height) {
    if ((v == 'z' || v == 'Z') && l_paddle_y < height - 4) {
        l_paddle_y += 1;
    } else if ((v == 'a' || v == 'A') && l_paddle_y > 1) {
        l_paddle_y -= 1;
    }

    return l_paddle_y;
}

int move_r_paddle(char v, int r_paddle_y, int height) {
    if ((v == 'm' || v == 'M') && r_paddle_y < height - 4) {
        r_paddle_y += 1;
    } else if ((v == 'k' || v == 'K') && r_paddle_y > 1) {
        r_paddle_y -= 1;
    }

    return r_paddle_y;
}

void print_screen(int l_paddle_y, int r_paddle_y, int ball_x, int ball_y, int score_a, int score_b, int width,
                  int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || (y == height - 1)) {
                mvaddch(y, x, '-');
            } else if ((x == 0 || x == width - 1) || (l_paddle_y <= y && y - l_paddle_y < 3 && x == 1) ||
                       (r_paddle_y <= y && y - r_paddle_y < 3 && x == width - 2)) {
                mvaddch(y, x, '|');
            } else if (x == ball_x && y == ball_y) {
                mvaddch(y, x, '*');
            } else {
                mvaddch(y, x, ' ');
            }
        }
        printw("\n");
    }
    refresh();

    print_information(score_a, score_b);
}

void print_information(int score_a, int score_b) {
    printw("  A/Z - move");
    for (int x = 0; x < 17; x++) printw(" ");

    printw("Space - skip action");

    for (int x = 0; x < 20; x++) printw(" ");

    printw("K/M - move\n");

    for (int x = 0; x < 32; x++) printw(" ");

    printw("Score: %d : %d\n", score_a, score_b);

    refresh();
}

void win_screen(int player) {
    clear();

    printw(
        "\n"
        "\n"
        ".----------------.  .----------------.  .-----------------.\n"
        "| .--------------. || .--------------. || .--------------. |\n"
        "| | _____  _____ | || |     _____    | || | ____  _____  | |\n"
        "| ||_   _||_   _|| || |    |_   _|   | || ||_   \\|_   _| | |\n"
        "| |  | | /\\ | |  | || |      | |     | || |  |   \\ | |   | |\n"
        "| |  | |/  \\| |  | || |      | |     | || |  | |\\ \\| |   | |\n"
        "| |  |   /\\   |  | || |     _| |_    | || | _| |_\\   |_  | |\n"
        "| |  |__/  \\__|  | || |    |_____|   | || ||_____|\\____| | |\n"
        "| |              | || |              | || |              | |\n"
        "| '--------------' || '--------------' || '--------------' |\n"
        " '----------------'  '----------------'  '----------------' \n");

    if (!player) {
        printw("                  Player 1 won the game!\n");
    } else {
        printw("                  Player 2 won the game!\n");
    }

    refresh();
    sleep(5);
}

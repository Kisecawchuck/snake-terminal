#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "renders.h"
#include "player.h"
#include "egg.h"

#define WIDTH 40
#define HEIGHT 15
#define FPS 15
#define MAX_SCORE 25

void term_mode(int mode);
void init_board(char board[]);
void update(char board[], node_t *player, egg_t *egg);
void clear();
void handle_pause();
int handle_input(char c, node_t *player);
int gen_allowed_x(node_t *player);
int gen_allowed_y(node_t *player);
void debug_mode(int mode, node_t *player);

int score = 0;
char input;
char prev_input;
bool should_close = false;

int main() {
    srand(time(NULL));

    char board[WIDTH * HEIGHT];
    node_t *player = create_player(9, HEIGHT / 2);
    egg_t *egg = create_egg(gen_allowed_x(player), gen_allowed_y(player));

    term_mode(1);
    clear();
    render_help();
    while (!should_close) {
        scanf("%c", &input);
        if (handle_input(input, player) == 1) should_close = true;
        else {
            clear();
            init_board(board);
            update(board, player, egg);
            fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
            usleep(1200 * 1200/FPS);
        }
        prev_input = input;
    }
    clear();
    if (score >= MAX_SCORE) {
        render_win();
        sleep(1);
    } else {
        render_lose();
        sleep(1);
    }
    clear();
    term_mode(0);

    return 0;
}

void term_mode(int mode) {
    static struct termios origt, newt;

    if(mode == 1) {
        tcgetattr(STDIN_FILENO, &origt);
        newt = origt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &origt);
    }

}

void init_board(char board[]) {
    memset(board, '.', WIDTH * HEIGHT);
}

void update(char board[], node_t *player, egg_t *egg) {
    if (check_collision(player, HEIGHT, WIDTH) != 0) {
        should_close = true;
        return;
    }

    pitch_node(player);

    spawn_egg(board, egg->x, egg->y, WIDTH);
    render_nodes(board, player, HEIGHT, WIDTH);

    render_title();
    render_board(board, HEIGHT, WIDTH);
    render_score(score);
    debug_mode(1, player);

    if (score == 25) {
        should_close = true;
        return;
    }

    if (player->x == egg->x && player->y == egg->y) {
        add_node(player);
        score++;
        egg->x = gen_allowed_x(player);
        egg->y = gen_allowed_y(player);
    }
}

void handle_pause() {
    clear();
    for (int cnt = 3; cnt >= 0; cnt--) {
        render_pause(cnt);
        sleep(1);
        clear();
    }
    input = prev_input;
}

int handle_input(char c, node_t *player) {
    if (c == 'q') {
        return 1;
    } else if (c == 'w') {
        if (player->dir != DOWN)
            player->dir = UP;
    } else if (c == 'd') {
        if (player->dir != LEFT)
            player->dir = RIGHT;
    } else if (c == 's') {
        if (player->dir != UP)
            player->dir = DOWN;
    } else if (c == 'a') {
        if (player->dir != RIGHT)
            player->dir = LEFT;
    } else if (c == 'p') {
        handle_pause();
    }
    return 0;
}


int gen_allowed_x(node_t *player) {
    int x;
    bool pass = false;
    while (!pass) {
        x = rand() % WIDTH;
        node_t *tmp = player;
        bool touched = false;
        while (tmp != NULL && !touched) {
            if (x == tmp->x) touched = true;
            tmp = tmp->next;
        }
        if (!touched) pass = true;
    }
    return x;
}

int gen_allowed_y(node_t *player) {
    int y;
    bool pass = false;
    while (!pass) {
        y = rand() % HEIGHT;
        node_t *tmp = player;
        bool touched = false;
        while (tmp != NULL && !touched) {
            if (y == tmp->y) touched = true;
            tmp = tmp->next;
        }
        if (!touched) pass = true;
    }
    return y;
}


void debug_mode(int mode, node_t *player) {
    if (mode == 1) {
        fprintf(stdout, "X: %d | Y: %d | INPUT: %c\n", player->x, player->y, input);
    }
}

void clear() {
    fprintf(stdout, "\033[2J");
    fprintf(stdout, "\033[H");
}

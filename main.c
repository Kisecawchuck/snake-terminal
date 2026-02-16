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
#define MAX_SCORE 100

void term_mode(int mode);
void init_board(char board[]);
void update(char board[], player_t *player, egg_t *egg);
void clear();
void handle_pause();
int handle_input(char c, player_t *player);
void gen_allowed_egg(egg_t *egg, player_t *player);
void debug_mode(int mode, player_t *player);

char input;
char prev_input;
bool should_close = false;

int main() {
    srand(time(NULL));

    char board[WIDTH * HEIGHT];
    player_t *player = create_player(WIDTH/2 - WIDTH/4, HEIGHT / 2);
    add_node(player);
    egg_t *egg = create_egg(WIDTH/2 + WIDTH/4, HEIGHT/2);

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

    if (player->score >= MAX_SCORE) {
        render_win();
        sleep(1);
    } else {
        render_lose();
        sleep(1);
    }
    clear();
    term_mode(0);

    node_t *p = player->head;
    while (p) {
        node_t *tmp = p;
        p = p->next;
        free(tmp);
    }
    free(player);
    free(egg);
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

void update(char board[], player_t *player, egg_t *egg) {
    if (check_collision(player, HEIGHT, WIDTH) != 0) {
        should_close = true;
        return;
    }

    pitch_player(player);

    spawn_egg(board, egg->x, egg->y, WIDTH);
    render_nodes(board, player, HEIGHT, WIDTH);

    render_title();
    render_board(board, HEIGHT, WIDTH);
    render_score(player->score);
    debug_mode(1, player);

    if (get_x(player) == egg->x && get_y(player) == egg->y) {
        player->score++;
        if (player->score >= MAX_SCORE) {
            should_close = true;
            return;
        }
        add_node(player);
        gen_allowed_egg(egg, player);
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

int handle_input(char c, player_t *player) {
    DIRECTION dir = get_dir(player);
    if (c == 'q') {
        return 1;
    } else if (c == 'w') {
        if (dir != DOWN)
            player->head->dir = UP;
    } else if (c == 'd') {
        if (dir != LEFT)
            player->head->dir = RIGHT;
    } else if (c == 's') {
        if (dir != UP)
            player->head->dir = DOWN;
    } else if (c == 'a') {
        if (dir != RIGHT)
            player->head->dir = LEFT;
    } else if (c == 'p') {
        handle_pause();
    }
    return 0;
}

void gen_allowed_egg(egg_t *egg, player_t *player) {
    int x, y;
    bool pass = false;
    while (!pass) {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
        node_t *tmp = player->head;
        bool touched = false;
        while (tmp && !touched) {
            if (x == tmp->x && y == tmp->y) touched = true;
            tmp = tmp->next;
        }
        if (!touched) pass = true;
    }
    egg->x = x;
    egg->y = y;
}

void debug_mode(int mode, player_t *player) {
    if (mode == 1) {
        fprintf(stdout, "X: %d | Y: %d | INPUT: %c\n", get_x(player), get_y(player), input);
    }
}

void clear() {
    fprintf(stdout, "\033[2J");
    fprintf(stdout, "\033[H");
}

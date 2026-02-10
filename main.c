#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "list.h"
#include "egg.h"

#define WIDTH 40
#define HEIGHT 15
#define FPS 15
#define MAX_SCORE 25

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    INVALID,
} DIRS;
void term_mode(int mode);
void init_board();
void render_board();
void pitch_node(node_t *player);
void update(node_t *player, egg_t *egg);
void clear();
void handle_pause();
int handle_input(char c, node_t *player);
void render_nodes(node_t *player);
void node_up(node_t *node);
void node_down(node_t *node);
void node_right(node_t *node);
void node_left(node_t *node);
void add_node(node_t *player);
node_t *create_player(int x, int y);
egg_t *create_egg(int x, int y);
void spawn_egg(int x, int y);
int gen_allowed_x(node_t *player);
int gen_allowed_y(node_t *player);
void render_score();
void debug_mode(int mode, node_t *player);
void render_title();
void check_collision(node_t *player);
void render_lose();
void render_win();
void render_help();
void render_pause(int pause_sec);

char board[WIDTH * HEIGHT];
char input;
char prev_input;
int score = 0;
bool should_close = false;

int main() {
    char proceed;
    srand(time(NULL));

    node_t *player = create_player(gen_allowed_x(NULL), gen_allowed_y(NULL));
    egg_t *egg = create_egg(gen_allowed_x(player), gen_allowed_y(player));

    term_mode(1);
    clear();
    render_help();
    while (!should_close) {
        scanf("%c", &input);
        if (handle_input(input, player) == 1) should_close = true;
        else {
            clear();
            init_board();
            update(player, egg);
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

void term_mode(int mode){
    static struct termios origt, newt;

    if(mode == 1){
        tcgetattr(STDIN_FILENO, &origt);
        newt = origt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }else{
        tcsetattr(STDIN_FILENO, TCSANOW, &origt);
    }

}

void init_board(){
    memset(board, '.', WIDTH * HEIGHT);
}

void render_board(){
    for (int i = 0; i < HEIGHT; i++) {
        fwrite(&board[i*WIDTH], WIDTH, 1, stdout);
        fputc('\n', stdout);
    }
}

void pitch_node(node_t *player) {
    if (player->dir == UP) node_up(player);
    if (player->dir == RIGHT) node_right(player);
    if (player->dir == DOWN) node_down(player);
    if (player->dir == LEFT) node_left(player);
}

void update(node_t *player, egg_t *egg){
    if (player->x >= WIDTH || player->x < 0) {
        should_close = true;
        return;
    }
    if (player->y >= HEIGHT || player->y < 0) {
        should_close = true;
        return;
    }

    pitch_node(player);

    spawn_egg(egg->x, egg->y);
    render_nodes(player);

    render_title();
    render_board();
    render_score();
    debug_mode(1, player);

    if (score == 25) {
        should_close = true;
    }

    if (get_node_x(player) == egg->x && get_node_y(player) == egg->y) {
        add_node(player);
        score++;
        egg->x = gen_allowed_x(player);
        egg->y = gen_allowed_y(player);
    }

    check_collision(player);
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

void render_nodes(node_t *player){
    node_t *tmp = player;

    while (tmp != NULL){
        if (!(tmp->next == NULL)){
            switch (tmp->dir) {
                case UP:
                    tmp->next->x = get_node_x(tmp);
                    tmp->next->y = get_node_y(tmp) + 1;
                    break;
                case RIGHT:
                    tmp->next->x = get_node_x(tmp) - 1;
                    tmp->next->y = get_node_y(tmp);
                    break;
                case DOWN:
                    tmp->next->x = get_node_x(tmp);
                    tmp->next->y = get_node_y(tmp) - 1;
                    break;
                case LEFT:
                    tmp->next->x = get_node_x(tmp) + 1;
                    tmp->next->y = get_node_y(tmp);
                    break;
            }
        }

        if (tmp != player){
            node_t *aux = player;
            while (aux != NULL){
                if (aux->next == tmp){
                    if (aux->y < tmp->y) tmp->dir = UP;
                    if (aux->x > tmp->x) tmp->dir = RIGHT;
                    if (aux->y > tmp->y) tmp->dir = DOWN;
                    if (aux->x < tmp->x) tmp->dir = LEFT;
                }
                aux = aux->next;
            }
        }
        board[(tmp->y * WIDTH) + tmp->x] = '#';
        tmp = tmp->next;
    }

}

void node_up(node_t *node){
    node->y--;
}

void node_down(node_t *node){
    node->y++;
}

void node_right(node_t *node){
    node->x++;
}

void node_left(node_t *node){
    node->x--;
}

void add_node(node_t *player){
    node_t *node = malloc(sizeof(node_t));
    insert_at_end(player, node);
    node_t *tmp = player;
    while(tmp != NULL){
        if(tmp->next == node){
            switch(tmp->dir){
                case UP:
                    node->x = get_node_x(tmp);
                    node->y = get_node_y(tmp) + 1;
                    node->dir = UP;
                    break;
                case RIGHT:
                    node->x = get_node_x(tmp) - 1;
                    node->y = get_node_y(tmp);
                    node->dir = RIGHT;
                    break;
                case DOWN:
                    node->x = get_node_x(tmp);
                    node->y = get_node_y(tmp) - 1;
                    node->dir = DOWN;
                    break;
                case LEFT:
                    node->x = get_node_x(tmp) + 1;
                    node->y = get_node_y(tmp);
                    node->dir = LEFT;
                    break;
            }
        }
        tmp = tmp->next;
    }
}

int gen_allowed_x(node_t *player){
    int x;
    bool pass = false;
    while (!pass) {
        x = rand() % WIDTH;
        node_t *tmp = player;
        bool touched = false;
        while (tmp != NULL && !touched) {
            if (x == get_node_x(tmp)) touched = true;
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
            if (y == get_node_y(tmp)) touched = true;
            tmp = tmp->next;
        }
        if (!touched) pass = true;
    }
    return y;
}

node_t *create_player(int x, int y) {
    node_t *tmp = malloc(sizeof(node_t));
    tmp->x = x;
    tmp->y = y;
    return tmp;
}

egg_t *create_egg(int x, int y) {
    egg_t *tmp = malloc(sizeof(egg_t));
    tmp->x = x;
    tmp->y = y;
    return tmp;
}

void spawn_egg(int x, int y){
    board[(y * WIDTH) + x] = 'O';
}

void render_score(){
    fprintf(stdout, "SCORE: %d\n", score);
}

void debug_mode(int mode, node_t *player){
    if (mode == 1){
        fprintf(stdout, "X: %d | Y: %d | INPUT: %c\n", get_node_x(player), get_node_y(player), input);
    }
}

void render_title(){
    fprintf(stdout, "SNAKE-TERMINAL by kevin\n");
}

void check_collision(node_t *player){
    node_t *tmp = player;
    while (tmp != NULL){
        if (tmp != player){
            if (get_node_x(player) == get_node_x(tmp) && get_node_y(player) == get_node_y(tmp)){
                should_close = true;
                break;
            }
        }
        tmp = tmp->next;
    }
}

void render_lose(){
    fprintf(stdout, "you lost :(\n");
}

void render_win(){
    fprintf(stdout, "you won :)\n");
}

void render_help(){
    fprintf(stdout, "commands:\nq = quit\np = pause\n\nmoves:\nw = up\na = left\ns = down\nd = right\n\ncollect 25 eggs to win\n\npress any key to start...\n");
}

void render_pause(int pause_sec){
    fprintf(stdout, "game paused for %ds\n", pause_sec);
}

void clear(){
    fprintf(stdout, "\033[2J");
    fprintf(stdout, "\033[H");
}

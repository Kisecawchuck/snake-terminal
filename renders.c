#include <stdio.h>
#include "player.h"
#include "renders.h"

void render_board(char board[], int height, int width) {
    for (int i = 0; i < height; i++) {
        fwrite(&board[i * width], width, 1, stdout);
        fputc('\n', stdout);
    }
}

void render_nodes(char board[], player_t *player, int height, int width) {
    node_t *tmp = player->head;

    int dirX[] = {0, -1, 0, 1, 0};
    int dirY[] = {1, 0, -1, 0, 0};
    while (tmp) {
        if (tmp->next) {
            tmp->next->x = tmp->x + dirX[tmp->dir];
            tmp->next->y = tmp->y + dirY[tmp->dir];
        }

        if (check_collision(player, height, width) != 0) return;
        board[(tmp->y * width) + tmp->x] = '#';
        tmp = tmp->next;
    }

    node_t *a = player->head;
    node_t *b = player->head->next;
    while (a && b) {
        if (a->y < b->y) b->dir = UP;
        if (a->x > b->x) b->dir = RIGHT;
        if (a->y > b->y) b->dir = DOWN;
        if (a->x < b->x) b->dir = LEFT;
        a = b;
        b = b->next;
    }
}

void render_score(int score) {
    fprintf(stdout, "SCORE: %d\n", score);
}

void render_title() {
    fprintf(stdout, "SNAKE-TERMINAL by kevin\n");
}

void render_lose() {
    fprintf(stdout, "you lost :(\n");
}

void render_win() {
    fprintf(stdout, "you won :)\n");
}

void render_help() {
    fprintf(stdout, "commands:\nq = quit\np = pause\n\nmoves:\nw = up\na = left\ns = down\nd = right\n\ncollect 100 eggs to win\n\npress any key to start...\n");
}

void render_pause(int pause_sec) {
    fprintf(stdout, "game paused for %ds\n", pause_sec);
}

#include <stdio.h>
#include "player.h"
#include "renders.h"

void render_board(char board[], int height, int width) {
    for (int i = 0; i < height; i++) {
        fwrite(&board[i * width], width, 1, stdout);
        fputc('\n', stdout);
    }
}

void render_nodes(char board[], node_t *player, int height, int width) {
    node_t *tmp = player;

    while (tmp != NULL) {
        if (!(tmp->next == NULL)) {
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
                default:
                    break;
            }
        }

        if (tmp != player) {
            node_t *aux = player;
            while (aux != NULL) {
                if (aux->next == tmp) {
                    if (aux->y < tmp->y) tmp->dir = UP;
                    if (aux->x > tmp->x) tmp->dir = RIGHT;
                    if (aux->y > tmp->y) tmp->dir = DOWN;
                    if (aux->x < tmp->x) tmp->dir = LEFT;
                }
                aux = aux->next;
            }
        }
        if (check_collision(player, height, width) != 0) return;
        board[(tmp->y * width) + tmp->x] = '#';
        tmp = tmp->next;
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
    fprintf(stdout, "commands:\nq = quit\np = pause\n\nmoves:\nw = up\na = left\ns = down\nd = right\n\ncollect 25 eggs to win\n\npress any key to start...\n");
}

void render_pause(int pause_sec) {
    fprintf(stdout, "game paused for %ds\n", pause_sec);
}

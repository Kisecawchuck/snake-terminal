#include <stdlib.h>
#include "player.h"

player_t *create_player(int x, int y) {
    player_t *player = malloc(sizeof(player_t));
    if (player == NULL) return NULL;

    node_t *head = malloc(sizeof(node_t));
    if (head == NULL) {
        free(player);
        return NULL;
    }

    head->x = x;
    head->y = y;
    head->dir = RIGHT;
    head->next = NULL;

    player->head = head;
    player->tail = head;
    player->size = 1;
    player->score = 0;

    return player;
}

DIRECTION get_dir(player_t *player) {
    return player->head->dir;
}

int get_x(player_t *player) {
    return player->head->x;
}

int get_y(player_t *player) {
    return player->head->y;
}

void node_up(node_t *node) {
    node->y--;
}

void node_right(node_t *node) {
    node->x++;
}

void node_down(node_t *node) {
    node->y++;
}


void node_left(node_t *node) {
    node->x--;
}

void pitch_player(player_t *player) {
    switch (player->head->dir) {
        case UP:
            node_up(player->head);
            break;
        case RIGHT:
            node_right(player->head);
            break;
        case DOWN:
            node_down(player->head);
            break;
        case LEFT:
            node_left(player->head);
            break;
        default:
            break;
    }
}

void add_node(player_t *player) {
    node_t *node = malloc(sizeof(node_t));
    if (!node) return;

    int dirX[] = {0, -1, 0, 1, 0};
    int dirY[] = {1, 0, -1, 0, 0};
    node_t *p = player->tail;

    node->x = p->x + dirX[p->dir];
    node->y = p->y + dirY[p->dir];
    node->dir = p->dir;
    node->next = NULL;

    player->tail->next = node;
    player->tail = node;
    player->size++;
}

int check_collision(player_t *player, int height, int width) {
    if (get_x(player) >= width || get_x(player) < 0) {
        return 1;
    }
    if (get_y(player) >= height || get_y(player) < 0) {
        return 1;
    }

    node_t *tmp = player->head->next;
    while (tmp) {
        if (get_x(player) == tmp->x && get_y(player) == tmp->y){
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

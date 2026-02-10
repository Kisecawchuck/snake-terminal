#include <stdlib.h>
#include "player.h"

node_t *create_player(int x, int y) {
    node_t *tmp = malloc(sizeof(node_t));
    tmp->x = x;
    tmp->y = y;
    tmp->dir = INVALID;
    return tmp;
}

void insert_at_end(node_t *head, node_t *new_node){
  node_t *tmp = head;
  if(new_node == head){
    return;
  }
  while(tmp != NULL){
    if(tmp->next == NULL){
      tmp->next = new_node;
      return;
    }
    tmp = tmp->next;
  }
}

int get_node_x(node_t *node){
  return node->x;
}

int get_node_y(node_t *node){
  return node->y;
}

int get_node_dir(node_t *node){
  return node->dir;
}

void node_up(node_t *node) {
    node->y--;
}

void node_down(node_t *node) {
    node->y++;
}

void node_right(node_t *node) {
    node->x++;
}

void node_left(node_t *node) {
    node->x--;
}

void pitch_node(node_t *player) {
    if (player->dir == UP) node_up(player);
    if (player->dir == RIGHT) node_right(player);
    if (player->dir == DOWN) node_down(player);
    if (player->dir == LEFT) node_left(player);
}

void add_node(node_t *player) {
    node_t *node = malloc(sizeof(node_t));
    insert_at_end(player, node);
    node_t *tmp = player;
    while (tmp != NULL) {
        if (tmp->next == node) {
            switch (tmp->dir) {
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
                default:
                    break;
            }
        }
        tmp = tmp->next;
    }
}

int check_collision(node_t *player, int height, int width) {
    if (player->x >= width || player->x < 0) {
        return 1;
    }
    if (player->y >= height || player-> y < 0) {
        return 1;
    }

    node_t *tmp = player;
    while (tmp != NULL){
        if (tmp != player) {
            if (player->x == tmp->x && player->y == tmp->y){
                return 1;
            }
        }
        tmp = tmp->next;
    }
    return 0;
}

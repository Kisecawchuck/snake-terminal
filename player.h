#ifndef PLAYER_H
#define PLAYER_H

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    INVALID,
} DIRECTION;

typedef struct node{
  int x;
  int y;
  DIRECTION dir;
  struct node *next;
} node_t;

node_t *create_player(int x, int y);
void insert_at_end(node_t *head, node_t *new_node);
int get_node_x(node_t *node);
int get_node_y(node_t *node);
int get_node_dir(node_t *node);
void node_up(node_t *node);
void node_down(node_t *node);
void node_right(node_t *node);
void node_left(node_t *node);
void pitch_node(node_t *player);
void add_node(node_t *player);
int check_collision(node_t *player, int height, int width);

#endif

#ifndef PLAYER_H
#define PLAYER_H

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    INVALID,
} DIRECTION;

typedef struct node {
  int x;
  int y;
  DIRECTION dir;
  struct node *next;
} node_t;

typedef struct _player {
    node_t *head;
    node_t *tail;
    int size;
    int score;
} player_t;

player_t *create_player(int x, int y);
DIRECTION get_dir(player_t *player);
int get_x(player_t *player);
int get_y(player_t *player);
void node_up(node_t *node);
void node_down(node_t *node);
void node_right(node_t *node);
void node_left(node_t *node);
void pitch_player(player_t *player);
void add_node(player_t *player);
int check_collision(player_t *player, int height, int width);

#endif

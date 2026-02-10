#ifndef EGG_H
#define EGG_H

typedef struct egg {
  int x;
  int y;
} egg_t;

egg_t *create_egg(int x, int y);
void spawn_egg(char board[], int x, int y, int width);
int get_egg_x(egg_t *egg);
int get_egg_y(egg_t *egg);

#endif

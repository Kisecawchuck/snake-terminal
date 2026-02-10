#ifndef EGG_H
#define EGG_H

typedef struct egg {
  int x;
  int y;
} egg_t;

int get_egg_x(egg_t *egg);
int get_egg_y(egg_t *egg);

#endif

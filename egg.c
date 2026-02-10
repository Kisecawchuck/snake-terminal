#include <stdlib.h>
#include "egg.h"

egg_t *create_egg(int x, int y) {
    egg_t *tmp = malloc(sizeof(egg_t));
    tmp->x = x;
    tmp->y = y;
    return tmp;
}

void spawn_egg(char board[], int x, int y, int width) {
    board[(y * width) + x] = 'O';
}

int get_egg_x(egg_t *egg){
  return egg->x;
}

int get_egg_y(egg_t *egg){
  return egg->y;
}

#ifndef RENDERS_H
#define RENDERS_H

#include "player.h"

void render_board(char board[], int height, int width);
void render_nodes(char board[], node_t *player, int height, int width);
void render_score(int score);
void render_title();
void render_lose();
void render_win();
void render_help();
void render_pause(int pause_sec);

#endif

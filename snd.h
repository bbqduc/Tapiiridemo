#ifndef TAPIIRI_SND_H
#define TAPIIRI_SND_H

#include <stdio.h>
#include <stdlib.h>
#include "bass.h"

void err(const char* text);
void snd_init();
void snd_destroy();
int snd_loadMOD(const char* file);
int snd_load(const char* file);
void snd_play(int chan);
unsigned int snd_getPosition(int chan);
unsigned int snd_getSeconds(int chan);

#endif

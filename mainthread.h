#ifndef WATEK_GLOWNY_H
#define WATEK_GLOWNY_H

#include <unistd.h>
#include "utils.h"
#include "Ship.h"

#include <mutex>

/* pętla główna aplikacji: zmiany stanów itd */
void mainLoop(Ship& ship);

#endif

#ifndef GLOBALH
#define GLOBALH

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <vector>

#include "utils.h"
#include "Ship.h"
#include "communication.h"
#include "mainthread.h"

/* używane w wątku głównym, determinuje jak często i na jak długo zmieniają się stany */
#define STATE_CHANGE_PROB 50
#define SEC_IN_STATE 2

#define ROOT 0

#endif

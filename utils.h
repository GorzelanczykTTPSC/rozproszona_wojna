#ifndef UTILS_H_
#define UTILS_H_
#include <mpi.h>

/* stany procesu */
typedef enum {InDock, Fighting, RequestingDock, ReadyToComeIn } state_t;

#define FIELDNO 2
typedef struct {
    int ts;         /* timestamp (zegar lamporta */
    int src;        /* od kogo pochodzi wiadomość (rank) */
    //int message;    /* typ wiadomości: 0 - REQUEST, 1 - OK */
} packet_t;

/* Typy wiadomości */
#define FINISH 1
#define REQUEST 2
#define INRUN 3
#define OK 4
#define GIVEMESTATE 5
#define STATE 6

#define DOCKS 1

extern MPI_Datatype MPI_PAKIET_T;

#endif
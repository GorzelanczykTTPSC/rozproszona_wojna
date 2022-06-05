#include "main.h"
//#include "watek_komunikacyjny.h"
//#include "watek_glowny.h"
//#include "monitor.h"
/* wątki */
#include <thread>
#include <csignal>
#include <functional>

/* sem_init sem_destroy sem_post sem_wait */
//#include <semaphore.h>
/* flagi dla open */
//#include <fcntl.h>

volatile char end = false;

MPI_Datatype MPI_PAKIET_T;

void check_thread_support(int provided)
{
    if (SHIP_DEBUG) printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE: 
            printf("Brak wsparcia dla wątków, kończę\n");
            /* Nie ma co, trzeba wychodzić */
	        fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
	        MPI_Finalize();
	        exit(-1);
	    break;
        case MPI_THREAD_FUNNELED: 
            printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
	    break;
        case MPI_THREAD_SERIALIZED: 
            /* Potrzebne zamki wokół wywołań biblioteki MPI */
            printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
	    break;
        case MPI_THREAD_MULTIPLE: 
            if (SHIP_DEBUG) printf("Pełne wsparcie dla wątków\n"); /* tego chcemy. Wszystkie inne powodują problemy */
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}

/* sprawdza, czy są wątki, tworzy typ MPI_PAKIET_T
*/
void init(int *argc, char ***argv)
{
    int provided;
    MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided);
    check_thread_support(provided);


    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    
    const int nitems=FIELDNO; 
    int       blocklengths[FIELDNO] = {1,1};
    MPI_Datatype typy[FIELDNO] = {MPI_INT, MPI_INT};

    MPI_Aint     offsets[FIELDNO]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    //offsets[2] = offsetof(packet_t, message);

    MPI_Type_create_struct(nitems, blocklengths, offsets, typy, &MPI_PAKIET_T);
    MPI_Type_commit(&MPI_PAKIET_T);

    //pthread_create( &threadKom, NULL, communicationThread, 0);
    
    //debug("App has started.");
}

/* usunięcie zamkków, czeka, aż zakończy się drugi wątek, zwalnia przydzielony typ MPI_PAKIET_T
   wywoływane w funkcji main przed końcem
*/
void finalize(int signal_num)
{
    //pthread_mutex_destroy( &stateMut);
    /* Czekamy, aż wątek potomny się zakończy */
    //println("czekam na wątek \"komunikacyjny\"\n" );
    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}

namespace {
std::function<void(int)> shutdown_handler;
void signal_handler(int signal) { shutdown_handler(signal); }
}

int main(int argc, char **argv)
{
    signal(SIGKILL, finalize);
    signal(SIGABRT, finalize);
    /* Tworzenie wątków, inicjalizacja itp */
    init(&argc,&argv); // tworzy wątek komunikacyjny w "watek_komunikacyjny.c"

    Ship ship = Ship();
    std::thread commThread(communicationThread, std::ref(ship));

    shutdown_handler = [&](int signal) {
        ship.finish();
        MPI_Type_free(&MPI_PAKIET_T);
        MPI_Finalize();
    };

    mainLoop(std::ref(ship));          // w pliku "watek_glowny.c"

    //finalize();
    return 0;
}


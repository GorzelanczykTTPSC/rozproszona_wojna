#include "communication.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void communicationThread(Ship& ship) {
    MPI_Status status;
    int is_message = false;
    packet_t pakiet;

    std::cout << "Rozpoczynam wątek komunikacyjny." << std::endl;

    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while (true) {
	    //debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        ship.stateMut.lock();
        ship.setClock(ship.getClock() > pakiet.ts ? ship.getClock()+1 : pakiet.ts+1);
        ship.stateMut.unlock();

        switch ( status.MPI_TAG ) {
	    case REQUEST: 
            state_t stat = ship.getState();
            if ( (stat == Fighting) || ((stat == RequestingDock) && (ship.getLastRequestTimestamp()<pakiet.ts))) {
                ship.send(pakiet.src, OK);
            }
	    break;
	    case OK: 
            // najpierw sprawdź czy jesteś w stanie oczekiwania na OKsy
            if (ship.getState()==RequestingDock) {
                // i czy jeszcze potrzebujesz ich, a jak nie to dodaj do vectora id ziomka od którego przyszła wiadomość
                if (pakiet.ts<ship.getLastRequestTimestamp()) {
                    ship.addReceivedOk();
                }

            }
	    break;
	    default:
	    break;
        }
    }
}

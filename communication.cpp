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

        pthread_mutex_lock(&ship.stateMut);
        ship.setClock(ship.getClock() > pakiet.ts ? ship.getClock()+1 : pakiet.ts+1);
        pthread_mutex_unlock(&ship.stateMut);

        switch ( status.MPI_TAG ) {
	    case FINISH: 
                //changeState(InFinish);
                std::cout << "Finish" << std::endl;
	    break;
	    case REQUEST: 
            state_t stat = ship.getState();
            if ( (stat == Fighting) || ((stat == RequestingDock) && (ship.getLastRequestTimestamp()<pakiet.ts))) {
                ship.send(pakiet.src, OK);
            }
	    break;
	    case GIVEMESTATE: 
                //pakiet.data = money;
                //sendPacket(&pakiet, ROOT, STATE);
                //debug("Wysyłam mój stan do monitora: %d funtów łoju na składzie!", money);
	    break;
        case STATE:
            //if (numberReceived > size-1) {
             //   debug("W magazynach mamy %d funtów łoju.", globalState);
            //} 
        break;
	    case OK: 
            // najpierw sprawdź czy jesteś w stanie oczekiwania na OKsy
            // i czy jeszcze potrzebujesz ich, a jak nie to dodaj do vectora id ziomka od którego przyszła wiadomość
            if (pakiet.ts<ship.getLastRequestTimestamp()) {
                ship.addReceivedOk();
            }
                //changeState( InMonitor );
                //debug("Od tej chwili czekam na polecenia od monitora");
	    break;
	    case INRUN: 
                //changeState( InRun );
                //debug("Od tej chwili decyzję podejmuję autonomicznie i losowo");
	    break;
	    default:
	    break;
        }
    }
}

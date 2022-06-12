#include "communication.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void communicationThread(Ship& ship) {
    MPI_Status status;
    int is_message = false;
    packet_t pakiet;

    if (ship.debug()) ship.print("Rozpoczynam wątek komunikacyjny.");

    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while (ship.isActive()) {
	    //debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        ship.stateMut->lock();
        ship.setClock(ship.getClock() > pakiet.ts ? ship.getClock()+1 : pakiet.ts+1);

        switch ( status.MPI_TAG ) {
	        case REQUEST: 
                {
                    if (ship.debug()) ship.print("Ship #"+std::to_string(pakiet.src)+" wants to get a dock!");
                    
                    state_t stat = ship.getState();
                    if ( (stat == Fighting) || ((stat == RequestingDock) && (ship.getLastRequestTimestamp()>pakiet.ts))) {
                        if (ship.debug()) ship.print("OK, I agree");
                        ship.send(pakiet.src, OK);
                    } else if ( (stat == InDock) || ((stat == RequestingDock) && (ship.getLastRequestTimestamp() < pakiet.ts))) {
                        if (ship.debug()) ship.print("Nah, I can't let him in");
                        ship.remember(pakiet.src);
                    }
                    ship.stateMut->unlock();
                }
	        break;
	        case OK: 
                ship.print("I received OK from "+std::to_string(pakiet.src)+" with lamport clock "+std::to_string(pakiet.ts)+". My last request clock: "+std::to_string(ship.getLastRequestTimestamp()));
                if (ship.getState()==RequestingDock) {
                    //ship.print(" and I am requesting a dock pkt_ts: "+std::to_string(pakiet.ts)+" last_req_ts: "+std::to_string(ship.getLastRequestTimestamp()));
                    if (pakiet.ts>ship.getLastRequestTimestamp()) {
                        ship.addReceivedOk();
                    }

                } 
                ship.stateMut->unlock();
	        break;
	        default:
	        break;
        }
    }
}

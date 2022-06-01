#include "mainthread.h"

int STATE_CHANGE_PROB = 50;

void mainLoop(Ship& ship)
{
    srandom(ship.getRank());
    while (true) { //(ship.getState() != InFinish) {
        int perc = random()%100; 

        ship.setState(Fighting);
        sleep((unsigned int)random()%10000);

        ship.setState(RequestingDock);

        ship.requestDockFromAll();

        std::unique_lock<std::mutex> lk(ship.stateMut);
        ship.canEnterDock.wait(lk);

        ship.setState(InDock);
        sleep((unsigned int)random()%10000);

/*
        state_t stanObecny = ship.getState();

        switch (stanObecny) {
            case ReadyToComeIn:

                break;
        }

        if (perc<STATE_CHANGE_PROB) {
            if (stan==InRun) {
		        //debug("Zmieniam stan na wysyłanie");
		        changeState( InSend );
		        packet_t *pkt = (packet_t *)malloc(sizeof(packet_t));
		        pkt->data = perc;
                changeMoney( -perc);
                sleep(3); // to nam zasymuluje, że wiadomość trochę leci w kanale
                                      // bez tego algorytm formalnie błędny za każdym razem dawałby poprawny wynik
		        ship.send((ship.getRank()+1)%ship.getAllShips(),MONEYTRANSFER);
		        changeState( InRun );
		        //debug("Skończyłem wysyłać");
            } 
        }
        */
        sleep(random()%1000);
    }
}

#include "mainthread.h"

int STATE_CHANGE_PROB = 50;

void mainLoop(Ship& ship)
{
    srandom(time(NULL)+getpid()+ship.getRank());
    ship.print("My pid: "+std::to_string(getpid()));
    while (true) { //(ship.getState() != InFinish) {
        int perc = random()%100; 

        unsigned int seconds = ship.getRank()==1 ? 1000 : (unsigned int)rand()%10;
        ship.print("Going into fight for "+std::to_string(seconds)+ " seconds!");
        ship.stateMut->lock();
        ship.setState(Fighting);
        ship.stateMut->unlock();
        sleep(seconds);

        ship.print("Fight done. Now I want to get a dock!");
        ship.stateMut->lock();
        ship.setState(RequestingDock);
        ship.stateMut->unlock();

        ship.requestDockFromAll();

        std::unique_lock<std::mutex> lk(*ship.stateMut);
        ship.canEnterDock->wait(lk);

        unsigned int secondsForDock = ship.getRank()==1 ? 1000 : (unsigned int)rand()%10;
        ship.print("Entering the dock for "+std::to_string(secondsForDock)+" seconds!");
        ship.setState(InDock);
        ship.print("I'm in the dock.");
        ship.canEnterDock->notify_all();
        sleep(secondsForDock);

        ship.print("Leaving the dock.");
        ship.sendWaitingOKs();

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
    }
}

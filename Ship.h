#ifndef SHIP_H
#define SHIP_H

#include <mpi.h>
#include <vector>
#include "utils.h"
#include <mutex>
#include <thread>
#include <condition_variable>

class Ship {

    private:

        int rank;
        int all_ships_num;

        int lamport_clock;
        int last_sent_request_msg_timestamp = -1;
        std::vector<int> to_send_back_OK;

        int received_oks = 0;

        state_t state;


    public:

        Ship();
        std::mutex stateMut;
        std::condition_variable canEnterDock;

        void send(int destination, int tag);
        void broadcast(int ts, int src, int tag); 
        void setClock(int newValue);
        void addReceivedOk();
        void requestDockFromAll();

        int getClock();
        int getRank();
        int getAllShips();
        int getLastRequestTimestamp();
        state_t getState();
        void setState(state_t state);

};

#endif
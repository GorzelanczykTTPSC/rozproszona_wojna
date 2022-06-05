#ifndef SHIP_H
#define SHIP_H

#include <mpi.h>
#include <string>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <mutex>
#include <thread>
#include <condition_variable>

class Ship {

    private:

        int rank;
        int all_ships_num;

        int lamport_clock = 0;
        int last_sent_request_msg_timestamp = 999991;
        std::vector<int> to_send_back_OK;

        int received_oks = 0;

        state_t state;

        bool isdebug = SHIP_DEBUG;
        bool isactive = true;


    public:

        Ship();
        std::mutex* stateMut;
        std::condition_variable* canEnterDock;

        void send(int destination, int tag);
        void setClock(int newValue);
        void addReceivedOk();
        void requestDockFromAll();
        void sendWaitingOKs();
        void print(std::string text);

        int getClock();
        int getRank();
        int getAllShips();
        int getLastRequestTimestamp();
        state_t getState();
        void setState(state_t state);
        void remember(int shipID);
        void debug(bool val);
        bool debug();
        void finish();
        bool isActive();

};

#endif
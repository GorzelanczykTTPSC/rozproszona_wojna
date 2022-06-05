#include "Ship.h"

Ship::Ship() {

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &all_ships_num);

    state = Fighting;

    stateMut = new std::mutex();
    canEnterDock = new std::condition_variable();

}

void Ship::setClock(int newValue) {
    lamport_clock = newValue;
}

int Ship::getClock() {

    return lamport_clock;
}

void Ship::debug(bool val) {
    isdebug = val;
}

bool Ship::debug() {
    return isdebug;
}

/*
 *
 * destination - rank okrętu docelowego
 * tag - tag wiadomości
 *
 */
void Ship::send(int destination, int tag)
{
    packet_t* pkt = (packet_t*)malloc(sizeof(packet_t));
    pkt->src = rank;
    pkt->ts = lamport_clock+1;
    if (tag == REQUEST) {
        last_sent_request_msg_timestamp=lamport_clock;
    }
    //print("Sending message "+std::to_string(tag)+" to #"+std::to_string(destination));
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    free(pkt);
}

void Ship::addReceivedOk() {
    received_oks += 1;
    if (debug()) print("Received oks: "+std::to_string(received_oks)+" All ships: "+std::to_string(all_ships_num));
    if (received_oks>=(all_ships_num-DOCKS==1 ? all_ships_num-DOCKS : all_ships_num-DOCKS)) {
        if (debug()) print("I got " + std::to_string(received_oks) + " oks. I'm coming in.");
        //stateMut->unlock();
        canEnterDock->notify_all();
    }
    return;
}

void Ship::requestDockFromAll() {
    for (int i=0; i<all_ships_num; i++) {
        if (i!=rank){
            send(i, REQUEST);
        }
    }
}

void Ship::print(std::string text) {
    std::cout << "\033["+std::to_string(30+(rank%7)+1)+"m" 
        + "[id: " + std::to_string(rank) 
        + "] [clock: " + std::to_string(lamport_clock)+"] \t" 
        + text + "\033[0m\n";
}

int Ship::getRank() {
    return rank;
}

int Ship::getAllShips() {
    return all_ships_num;
}

state_t Ship::getState() {
    return state;
}

void Ship::setState(state_t stat) {
    state = stat;
}

void Ship::finish() {
    isactive = false;
}

bool Ship::isActive() {
    return isactive;
}

int Ship::getLastRequestTimestamp() {
    return last_sent_request_msg_timestamp;
}

void Ship::remember(int shipID) {
    to_send_back_OK.push_back(shipID);

    auto it = unique(to_send_back_OK.begin(), to_send_back_OK.end());
    to_send_back_OK.resize(distance(to_send_back_OK.begin(), it));
}

void Ship::sendWaitingOKs() {
    //for (int shipid : to_send_back_OK) {
    //    send(shipid, OK);
    //}
    if (to_send_back_OK.size()>0) send(to_send_back_OK.front(), OK);
    to_send_back_OK.clear();
    received_oks = 0;
}
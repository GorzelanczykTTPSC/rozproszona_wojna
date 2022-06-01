#include "Ship.h"

Ship::Ship() {

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &all_ships_num);
    srand(rank);

    state = Fighting;

    int test = 4; // DOCKS

}

void Ship::setClock(int newValue) {
    lamport_clock = newValue;
}

int Ship::getClock() {

    return lamport_clock;
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
    pthread_mutex_lock(&stateMut);
    pkt->ts = lamport_clock+1;
    pthread_mutex_unlock(&stateMut);
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    free(pkt);
}

void Ship::broadcast(int ts, int src, int tag) {}

void Ship::addReceivedOk() {
    received_oks += 1;
    return;
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

void Ship::setState(state_t state) {
    this->state = state;
}

int Ship::getLastRequestTimestamp() {
    return last_sent_request_msg_timestamp;
}

void changeMoney( int newMoney )
{
    /*
    pthread_mutex_lock( &moneyMut );
    if (stan==InFinish) { 
	pthread_mutex_unlock( &moneyMut );
        return;
    }
    money += newMoney;
    pthread_mutex_unlock( &moneyMut );
    */
   return;
}

void changeState( state_t newState )
{
    /*
    pthread_mutex_lock( &stateMut );
    if (stan==InFinish) { 
	pthread_mutex_unlock( &stateMut );
        return;
    }
    stan = newState;
    pthread_mutex_unlock( &stateMut );
    */
    return;
}

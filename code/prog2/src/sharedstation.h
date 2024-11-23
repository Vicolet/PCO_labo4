#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include <pcosynchro/pcosemaphore.h>
#include "locomotive.h"

class SharedStation
{
public:
    SharedStation(int nbTrains);

    void waitingAtStation(Locomotive& loco);

private:
    /* TODO */
    bool waitingLocos;
    PcoSemaphore waiting_at_station_semaphore;
    PcoSemaphore goingToStation;
};

#endif // SHARED_STATION_H
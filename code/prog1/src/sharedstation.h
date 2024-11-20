#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include <pcosynchro/pcosemaphore.h>

class SharedStation
{
public:
    SharedStation(int nbTrains, int nbTours);

    /* Implémentez toute la logique que vous avez besoin pour que les locomotives
     * s'attendent correctement à la station */

     void attendreGare();

private:
    /* TODO */
    int trainsArrives;           // Nombre de trains arrivés en gare.
    const int totalTrains;       // Nombre total de trains attendus.
    const int toursParTrain;     // Nombre de tours avant l'attente en gare.
    PcoSemaphore mutex;            // Protège l'accès à `trainsArrives`.
    PcoSemaphore semaphore;      // Synchronise les trains en gare.
};

#endif // SHARED_STATION_H

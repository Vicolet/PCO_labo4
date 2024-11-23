//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include <chrono>
#include <thread>

#include "sharedstation.h"

SharedStation::SharedStation(int nbTrains, int nbTours) : nbTrains(nbTrains), nbTours(nbTours), mutex(1)
{}

int SharedStation::getNbTrains() { return nbTrains; }
int SharedStation::getNbTours() { return nbTours; }

void SharedStation::attendreGare()
{
     mutex.acquire();
    ++nbTrainsWaiting;
    mutex.release();
}


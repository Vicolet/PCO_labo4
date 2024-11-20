//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include <chrono>
#include <thread>

#include "sharedstation.h"

SharedStation::SharedStation(int nbTrains, int nbTours) : trainsArrives(0), totalTrains(nbTrains), toursParTrain(nbTours), semaphore(0)
{}

void SharedStation::attendreGare()
{
    {
        mutex.lock();
        trainsArrives++;

        // Si tous les trains sont arrivés
        if (trainsArrives == totalTrains) {
            semaphore.release(totalTrains - 1); // Libère les autres locomotives
            trainsArrives = 0;                 // Réinitialise le compteur
        } else {
            mutex.unlock(); // Libère le verrou avant d'attendre
            semaphore.acquire(); // Attend les autres trains
        }
    }

    // Pause pour montée/descente des passagers
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


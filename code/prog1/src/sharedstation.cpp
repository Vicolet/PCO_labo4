//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include <chrono>
#include <thread>

#include "sharedstation.h"
/*
SharedStation::SharedStation(int nbTrains, int nbTours)
    : trainsArrives(0), totalTrains(nbTrains), toursParTrain(nbTours),
      semaphore(0), mutex(1) {}
*/
void SharedStation::attendreGare() {
    mutex.acquire();
    trainsArrives++;

    if (trainsArrives == totalTrains) {
        for (int i = 0; i < totalTrains - 1; ++i) {
            semaphore.release();
        }
        trainsArrives = 0;
    } else {
        mutex.release();
        semaphore.acquire();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

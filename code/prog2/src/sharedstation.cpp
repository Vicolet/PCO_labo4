//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include <chrono>
#include <thread>

#include "sharedstation.h"

SharedStation::SharedStation(int nbTrains): waiting_at_station_semaphore(0), goingToStation(1) { // Initialize semaphores
    waitingLocos = false; // Initialize boolean
}


void SharedStation::waitingAtStation(Locomotive& loco)
{
    loco.arreter();
    loco.inverserSens(); // Inverse la direction de la locomotive
    loco.afficherMessage("Loco " + QString::number(loco.numero()) + " inversée, attend en gare...");

    goingToStation.acquire();
    if (waitingLocos) {
        // Si une autre locomotive est déjà en attente, libère cette locomotive après 3 secondes
        loco.afficherMessage("Une loco est déjà en gare");
        waitingLocos = false;
        goingToStation.release();
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Temps d'attente pour passagers
        waiting_at_station_semaphore.release(); // Libère la première locomotive
    } else {
        // Première locomotive arrivée, elle attend la seconde
        loco.afficherMessage("Je suis la première");
        waitingLocos = true;
        goingToStation.release();
        waiting_at_station_semaphore.acquire(); // Attente de libération par la seconde locomotive
    }

    loco.demarrer();
    loco.afficherMessage("Je repars !");
}
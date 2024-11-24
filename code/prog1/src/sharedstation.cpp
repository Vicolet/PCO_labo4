//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //

#include <chrono>
#include <thread>

#include "sharedstation.h"

SharedStation::SharedStation(int nbTrains)
    : semaphoreAttenteStation(0), mutex(1) { // Initialisation des sémaphores
  locosEnAttente = false;                    // Initialisation du booléen
}

void SharedStation::waitingAtStation(Locomotive &loco) {
  loco.arreter();
  loco.inverserSens(); // Inverse la direction de la locomotive
  loco.afficherMessage("Loco " + QString::number(loco.numero()) +
                       " inversée, attend en gare...");

  mutex.acquire();
  if (locosEnAttente) {
    // Si une autre locomotive est déjà en attente, libère cette locomotive
    // après 2 secondes
    locosEnAttente = false;
    mutex.release();
    std::this_thread::sleep_for(
        std::chrono::seconds(2));      // Temps d'attente pour passagers
    semaphoreAttenteStation.release(); // Libère la première locomotive
    loco.afficherMessage("Une locomotive est déjà en gare");
  } else {
    // Première locomotive arrivée, elle attend la seconde
    loco.afficherMessage("Je suis la première locomotive arrivée en gare");
    locosEnAttente = true;
    mutex.release();
    semaphoreAttenteStation.acquire(); // Attente de libération par la seconde locomotive
  }

  loco.demarrer();
  loco.afficherMessage("La locomotive " + QString::number(loco.numero()) +
                       " repart !");
}

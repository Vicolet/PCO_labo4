#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include "locomotive.h"
#include <pcosynchro/pcosemaphore.h>

/**
 * @brief La classe SharedStation représente une station partagée où les
 * locomotives peuvent attendre.
 */
class SharedStation {
public:
  /**
   * @brief SharedStation Constructeur de la classe SharedStation.
   * Initialise les sémaphores nécessaires en fonction du nombre de trains.
   * @param nbTrains Nombre de trains pouvant être gérés simultanément par la
   * station.
   */
  SharedStation(int nbTrains);

  /**
   * @brief waitingAtStation Permet à une locomotive d'attendre à la station
   * partagée. Cette méthode met la locomotive en attente jusqu'à ce qu'elle
   * soit autorisée à continuer.
   * @param loco La locomotive qui attend à la station.
   */
  void waitingAtStation(Locomotive &loco);

private:
  PcoSemaphore semaphoreAttenteStation; // Sémaphore pour gérer l'attente des locomotives
  PcoSemaphore mutex;  // Sémaphore pour gérer l'entrée des locomotives
  bool locosEnAttente; // Indique si des locomotives attendent à la station
};

#endif // SHARED_STATION_H
//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"

class SharedStation;

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive.
 */
class LocomotiveBehavior : public Launchable {
public:
    /*!
     * @brief LocomotiveBehavior Constructeur de la classe.
     * @param locomotive La locomotive dont on représente le comportement.
     * @param sectionPartagee Pointeur vers la section partagée.
     * @param stationPartagee Pointeur vers la station partagée.
     */
    LocomotiveBehavior(Locomotive &locomotive,
                       std::shared_ptr<SharedSectionInterface> sectionPartagee,
                       std::shared_ptr<SharedStation> stationPartagee)
        : loco(locomotive),
          sectionPartagee(sectionPartagee),
          stationPartagee(*stationPartagee) {
        // Code supplémentaire éventuel pour le constructeur
    }

protected:
    /*!
     * @brief run Fonction lancée par le thread, représente le comportement de la locomotive.
     */
    void run() override;

    /*!
     * @brief printStartMessage Message affiché lors du démarrage du thread.
     */
    void printStartMessage() override;

    /*!
     * @brief printCompletionMessage Message affiché lorsque le thread a terminé.
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement.
     */
    Locomotive &loco;

    /**
     * @brief sectionPartagee Pointeur vers la section partagée.
     */
    std::shared_ptr<SharedSectionInterface> sectionPartagee;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     * Par exemple : la gestion de la priorité ou le parcours de la locomotive.
     */

private:
    /**
     * @brief stationPartagee Référence vers la station partagée.
     */
    SharedStation &stationPartagee;
};

#endif // LOCOMOTIVEBEHAVIOR_H

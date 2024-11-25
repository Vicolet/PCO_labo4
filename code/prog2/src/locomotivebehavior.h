//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"

class SharedStation;
/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable {
public:
    /*!
     * @brief locomotiveBehavior Constructeur de la classe
     * @param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive &loco,
                       std::shared_ptr<SharedSectionInterface> sharedSection,
                       std::shared_ptr<SharedStation> sharedStation,
                       int priority)
        : loco(loco), sharedSection(sharedSection), sharedStation(sharedStation) {
        randPriority();
    }

    /**
    * @brief RandPriority Génère une priorité aléatoire pour la locomotive.
    *
    * Cette fonction attribue une priorité aléatoire à l'objet loco. La priorité est générée
    * en utilisant un générateur de nombres aléatoires avec une distribution uniforme entre 0 et 10.
    */
    void randPriority() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 10);
        this->loco.priority = dis(gen);
    }

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive &loco;

    /**
     * @brief moveForward Déplace la locomotive dans la direction avant
     * \param nbTours Nombre de tours effectués
     * \param maxTours Nombre maximum de tours avant changement de direction
     * \param directionAvant Référence à la direction actuelle
     */
    void moveForward(int &nbTours, int maxTours, bool &directionAvant);

    /**
     * @brief moveBackward Déplace la locomotive dans la direction arrière
     * \param nbTours Nombre de tours effectués
     * \param maxTours Nombre maximum de tours avant changement de direction
     * \param directionAvant Référence à la direction actuelle
     */
    void moveBackward(int &nbTours, int maxTours, bool &directionAvant);

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;


    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */

private:
    std::shared_ptr<SharedStation> sharedStation;
    int priority; // Priorité de la locomotive
};

#endif // LOCOMOTIVEBEHAVIOR_H

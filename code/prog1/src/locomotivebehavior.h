//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "launchable.h"
#include "locomotive.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une
 * locomotive
 */
class LocomotiveBehavior : public Launchable {
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(
        Locomotive &loco, std::shared_ptr<SharedSectionInterface> sharedSection,
        std::pair<int, int> delimitorsCS, const int station,
        std::map<int, int> aiguillagesChanges /*, autres paramètres éventuels */)
        : loco(loco), sharedSection(sharedSection), delimitorsCS(delimitorsCS),
          station(station), aiguillagesChanges(aiguillagesChanges) {
        // Eventuel code supplémentaire du constructeur
    }

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la
     * locomotive
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
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */

    // Délimiteurs de la section critique (numéros de conctact de début et fin)
    const std::pair<int, int> delimitorsCS;

    // Numéro de contact de la gare
    const int station;

    // Changements à faire sur l'aiguillages au moment de rentrer en SC
    std::map<int, int> aiguillagesChanges;
};

#endif // LOCOMOTIVEBEHAVIOR_H

//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"


void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->access(loco);
    //sharedSection->leave(loco);

    while(true) {
        // On attend qu'une locomotive arrive sur le contact 1.
        // Pertinent de faire ça dans les deux threads? Pas sûr...
        attendre_contact(1);
        loco.afficherMessage("J'ai atteint le contact 1");

        // Attente en gare
        attendre_contact(station);
        loco.afficherMessage("Arrêt en gare");
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause pour passagers

        // Entrée dans la SC
        attendre_contact(delimitorsCS.first);
        loco.afficherMessage("J'ai atteint la section commune");
        sharedSection->access(loco);
        loco.afficherMessage("En route dans la section commune");

        //Faire tous les changements d'aiguillages nécessaires pour passer en SC
        for (auto change: aiguillagesChanges) {
            diriger_aiguillage(change.first, change.second, 0);
        }

        // Sortie de la SC
        attendre_contact(delimitorsCS.second);
        loco.afficherMessage("Je quitte la section commune");
        sharedSection->leave(loco);
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}

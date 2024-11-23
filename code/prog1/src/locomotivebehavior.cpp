//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include "sharedstation.h"

void LocomotiveBehavior::run() {
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    int nbToursLoco7 = 0, nbToursLoco42 = 0;
    bool directionAvant = true; // Track direction (true = forward, false = backward)
    const int N1 = 1;
    const int N2 = 2;

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :

    while (!PcoThread::thisThread()->stopRequested()) {
        if (loco.numero() == 42) {
            if (directionAvant) {
                // Logic for moving forward
                attendre_contact(22);
                sharedSection->access(loco);

                attendre_contact(12);
                sharedSection->leave(loco);
            } else {
                // Logic for moving in reverse direction
                attendre_contact(11);
                sharedSection->access(loco);

                attendre_contact(20);
                sharedSection->leave(loco);
            }

            // Increment loop counter and check if we need to reverse direction
            attendre_contact(1);
            nbToursLoco42++;
            loco.afficherMessage("Loco 42: J'ai fait : " + QString::number(nbToursLoco42) + " tours");
            if (nbToursLoco42 >= N1) {
                directionAvant = !directionAvant; // Toggle the direction flag
                nbToursLoco42 = 0; // Reset the loop counter
                sharedStation.waitingAtStation(loco);
            }
        } else if (loco.numero() == 7) {
            if (directionAvant) {
                // Logic for moving forward
                attendre_contact(25);
                sharedSection->access(loco);
                diriger_aiguillage(10, DEVIE, 0);
                diriger_aiguillage(13, DEVIE, 0);

                attendre_contact(15);
                diriger_aiguillage(10, TOUT_DROIT, 0);
                diriger_aiguillage(13, TOUT_DROIT, 0);
                sharedSection->leave(loco);
            } else {
                // Logic for moving in reverse direction
                attendre_contact(14);
                sharedSection->access(loco);
                diriger_aiguillage(10, DEVIE, 0);
                diriger_aiguillage(13, DEVIE, 0);

                attendre_contact(24);
                sharedSection->leave(loco);
                diriger_aiguillage(10, TOUT_DROIT, 0);
                diriger_aiguillage(13, TOUT_DROIT, 0);
            }
            attendre_contact(5);
            nbToursLoco7++;
            loco.afficherMessage("Loco 7: J'ai fait : " + QString::number(nbToursLoco7) + " tours");
            if (nbToursLoco7 >= N2) {
                directionAvant = !directionAvant; // Toggle the direction flag
                nbToursLoco7 = 0; // Reset the loop counter
                sharedStation.waitingAtStation(loco);
            }
        }
    }
}

void LocomotiveBehavior::printStartMessage() {
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage() {
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}

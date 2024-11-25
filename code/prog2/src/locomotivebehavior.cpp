//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include "sharedstation.h"

void LocomotiveBehavior::run() {
    // Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    int nbTours = 0;
    bool directionAvant = true; // Direction de déplacement
    const int maxTours = (loco.numero() == 7) ? 1 : 1; // N2 pour loco 7, N1 pour loco 42

    while (!PcoThread::thisThread()->stopRequested()) {
        if (directionAvant) {
            moveForward(nbTours, maxTours, directionAvant);
        } else {
            moveBackward(nbTours, maxTours, directionAvant);
        }
        // Basculer le mode de priorité après un certain nombre de tours
        if (nbTours == maxTours) {
            sharedSection->togglePriorityMode(); // Bascule le mode de priorité
        }
    }
}

void LocomotiveBehavior::moveForward(int& nbTours, int maxTours, bool& directionAvant) {
    if (loco.numero() == 42) {
        // Logique pour la loco 42 en marche avant
        attendre_contact(29);
        sharedSection->request(loco, loco.priority);
        attendre_contact(22);
        sharedSection->access(loco, priority);
        attendre_contact(12);
        sharedSection->leave(loco);
        attendre_contact(1);
    } else if (loco.numero() == 7) {
        // Logique pour la loco 7 en marche avant
        attendre_contact(33);
        sharedSection->request(loco, priority);

        attendre_contact(25);
        sharedSection->access(loco, priority);
        diriger_aiguillage(10, DEVIE, 0);
        diriger_aiguillage(13, DEVIE, 0);

        attendre_contact(15);
        diriger_aiguillage(10, TOUT_DROIT, 0);
        diriger_aiguillage(13, TOUT_DROIT, 0);
        sharedSection->leave(loco);

        attendre_contact(5);
    }

    nbTours++;
    loco.afficherMessage(QString("J'ai fait : %1 tours").arg(nbTours));

    if (nbTours >= maxTours) {
        directionAvant = false; // Changer de direction
        nbTours = 0;
        sharedStation->waitingAtStation(loco);
        randPriority();
        sharedSection->togglePriorityMode(); // Bascule le mode de priorité
    }
}

void LocomotiveBehavior::moveBackward(int& nbTours, int maxTours, bool& directionAvant) {
    if (loco.numero() == 42) {
        // Logique pour la loco 42 en marche arrière
        attendre_contact(3);
        sharedSection->request(loco, loco.priority);
        attendre_contact(11);

        sharedSection->access(loco, loco.priority);
        attendre_contact(20);

        sharedSection->leave(loco);
        attendre_contact(1);
    } else if (loco.numero() == 7) {
        // Logique pour la loco 7 en marche arrière
        attendre_contact(6);
        sharedSection->request(loco, loco.priority);
        attendre_contact(14);

        sharedSection->access(loco, loco.priority);
        diriger_aiguillage(10, DEVIE, 0);
        diriger_aiguillage(13, DEVIE, 0);
        attendre_contact(24);

        sharedSection->leave(loco);
        diriger_aiguillage(10, TOUT_DROIT, 0);
        diriger_aiguillage(13, TOUT_DROIT, 0);
        attendre_contact(5);
    }

    nbTours++;
    loco.afficherMessage(QString("J'ai fait : %1 tours").arg(nbTours));

    if (nbTours >= maxTours) {
        directionAvant = true; // Changer de direction
        nbTours = 0;
        sharedStation->waitingAtStation(loco);
        randPriority();
        sharedSection->togglePriorityMode(); // Bascule le mode de priorité
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

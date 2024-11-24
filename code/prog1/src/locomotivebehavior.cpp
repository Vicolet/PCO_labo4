#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include "sharedstation.h"

void LocomotiveBehavior::run() {
    // Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    int nbTours = 0;
    const int maxTours = (loco.numero() == 42) ? 1 : 2; // Nombre maximal de tours
    bool directionAvant = true; // Direction de la locomotive (true = avant, false = arrière)

    while (!PcoThread::thisThread()->stopRequested()) {
        if (directionAvant) {
            moveForward(nbTours, maxTours, directionAvant);
        } else {
            moveBackward(nbTours, maxTours, directionAvant);
        }
    }
}

void LocomotiveBehavior::moveForward(int &nbTours, int maxTours, bool &directionAvant) {
    if (loco.numero() == 42) {
        // Logique pour la loco 42 en marche avant
        attendre_contact(22);
        sharedSection->access(loco);

        attendre_contact(12);
        sharedSection->leave(loco);
    } else if (loco.numero() == 7) {
        // Logique pour la loco 7 en marche avant
        attendre_contact(25);
        sharedSection->access(loco);
        diriger_aiguillage(10, DEVIE, 0);
        diriger_aiguillage(13, DEVIE, 0);

        attendre_contact(15);
        diriger_aiguillage(10, TOUT_DROIT, 0);
        diriger_aiguillage(13, TOUT_DROIT, 0);
        sharedSection->leave(loco);
    }

    attendre_contact((loco.numero() == 42) ? 1 : 5);
    nbTours++;
    loco.afficherMessage(QString("Loco %1: J'ai fait : %2 tours")
                         .arg(loco.numero())
                         .arg(nbTours));
    if (nbTours >= maxTours) {
        directionAvant = false; // Changer de direction
        nbTours = 0;
        sharedStation.waitingAtStation(loco);
    }
}

void LocomotiveBehavior::moveBackward(int &nbTours, int maxTours, bool &directionAvant) {
    if (loco.numero() == 42) {
        // Logique pour la loco 42 en marche arrière
        attendre_contact(11);
        sharedSection->access(loco);

        attendre_contact(20);
        sharedSection->leave(loco);
    } else if (loco.numero() == 7) {
        // Logique pour la loco 7 en marche arrière
        attendre_contact(14);
        sharedSection->access(loco);
        diriger_aiguillage(10, DEVIE, 0);
        diriger_aiguillage(13, DEVIE, 0);

        attendre_contact(24);
        sharedSection->leave(loco);
        diriger_aiguillage(10, TOUT_DROIT, 0);
        diriger_aiguillage(13, TOUT_DROIT, 0);
    }

    attendre_contact((loco.numero() == 42) ? 1 : 5);
    nbTours++;
    loco.afficherMessage(QString("Loco %1: J'ai fait : %2 tours")
                         .arg(loco.numero())
                         .arg(nbTours));
    if (nbTours >= maxTours) {
        directionAvant = true; // Changer de direction
        nbTours = 0;
        sharedStation.waitingAtStation(loco);
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

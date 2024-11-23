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

    int toursLoco42 = 0; // Nombre de tours effectués par la locomotive 42
    int toursLoco7 = 0; // Nombre de tours effectués par la locomotive 7
    const int maxToursLoco42 = 1; // Nombre maximal de tours avant de changer de direction pour la loco 42
    const int maxToursLoco7 = 2; // Nombre maximal de tours avant de changer de direction pour la loco 7
    bool estDirectionAvant = true; // Direction de la locomotive (true = avant, false = arrière)

    // Boucle principale pour gérer les locomotives
    while (!PcoThread::thisThread()->stopRequested()) {
        if (loco.numero() == 42) {
            if (estDirectionAvant) {
                // Logique pour la loco 42 en marche avant
                attendre_contact(22);
                sharedSection->access(loco);

                attendre_contact(12);
                sharedSection->leave(loco);
            } else {
                // Logique pour la loco 42 en marche arrière
                attendre_contact(11);
                sharedSection->access(loco);

                attendre_contact(20);
                sharedSection->leave(loco);
            }

            // Incrémenter le compteur de tours et vérifier si la direction doit changer
            attendre_contact(1);
            toursLoco42++;
            loco.afficherMessage("Loco 42: J'ai fait : " + QString::number(toursLoco42) + " tours");
            if (toursLoco42 >= maxToursLoco42) {
                estDirectionAvant = !estDirectionAvant; // Inverser la direction
                toursLoco42 = 0; // Réinitialiser le compteur de tours
                sharedStation.waitingAtStation(loco);
            }
        } else if (loco.numero() == 7) {
            if (estDirectionAvant) {
                // Logique pour la loco 7 en marche avant
                attendre_contact(25);
                sharedSection->access(loco);
                diriger_aiguillage(10, DEVIE, 0);
                diriger_aiguillage(13, DEVIE, 0);

                attendre_contact(15);
                diriger_aiguillage(10, TOUT_DROIT, 0);
                diriger_aiguillage(13, TOUT_DROIT, 0);
                sharedSection->leave(loco);
            } else {
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

            attendre_contact(5);
            toursLoco7++;
            loco.afficherMessage("Loco 7: J'ai fait : " + QString::number(toursLoco7) + " tours");
            if (toursLoco7 >= maxToursLoco7) {
                estDirectionAvant = !estDirectionAvant; // Inverser la direction
                toursLoco7 = 0; // Réinitialiser le compteur de tours
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

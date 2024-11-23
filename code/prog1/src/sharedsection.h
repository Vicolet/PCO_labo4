//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface
{
public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection(int premierAiguillageHoraire, int secondAiguillageHoraire, int premierAiguillageAntiHoraire, int secondAiguillageAntiHoraire) {
        // TODO
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée par une autre locomotive.
     * Si la locomotive et son thread ont été mis en attente,
     * le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive &loco) override {
        // TODO

         CSMutex.acquire();
        //Tant que ce n'est pas libre et qu'on est pas la prochaine locomotive à pouvoir traverser la SC
        while (!CSFree || loco.priority != nbLocoLeavedCS) {
            CSMutex.release();
            loco.arreter();
            loco.afficherMessage(qPrintable(QString("Je ne suis PAS prioritaire (priority = %1) donc j'attends !").arg(loco.priority)));
            CSAccess.acquire();
            CSMutex.acquire();
        }
        CSFree = false;
        CSMutex.release();

        loco.demarrer();

        loco.afficherMessage(qPrintable(QString("Je suis prioritaire (priority = %1) pour la SC").arg(loco.priority)));

        afficher_message(qPrintable(QString("The engine no. %1 accesses the shared section.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive& loco) override {
        // TODO

          CSMutex.acquire();
        nbLocoLeavedCS = ++nbLocoLeavedCS % TOTAL_NB_LOCOS;
        int nbLocoWaitingToGiveAChance = TOTAL_NB_LOCOS - nbLocoLeavedCS;
        CSFree = true;
        CSMutex.release();

        //Si nous ne sommes pas la dernière locomotive, on libère toutes les locomotives qui sont en attente
        for (int i = 0; i < nbLocoWaitingToGiveAChance; i++) {
            CSAccess.release();
        }

        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));
    }

private:

    /* A vous d'ajouter ce qu'il vous faut */

    // Méthodes privées ...
    // Attribut privés ...
    //Permet la gestion de l'entrée et sortie en section critique
    bool CSFree = true;//whether the critical section is free
    PcoSemaphore CSAccess{0};
    int nbLocoLeavedCS = 0;
    PcoSemaphore CSMutex{1};

    //Permet l'attente de l'autre locomotive en gare
    PcoSemaphore stationWait{0};

    //Permet la gestion des priorités et l'attente commune en gare
    int nbLocoWaiting = 0;
    PcoSemaphore stationMutex{1};
    const int TOTAL_NB_LOCOS = 2;


    int premierAiguillageHoraire;
    int secondAiguillageHoraire;
    int premierAiguillageAntiHoraire;
    int secondAiguillageAntiHoraire;
};


#endif // SHAREDSECTION_H

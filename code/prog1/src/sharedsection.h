//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface {
public:
    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection(): semaphore(1) {
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
        loco.afficherMessage("Demande d'accès à la section partagée.");
        semaphore.acquire(); // Attente si la section est occupée
        loco.afficherMessage("Accès accordé à la section partagée.");
        afficher_message(qPrintable(QString("La locomotive %1 entre dans la section partagée.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive &loco) override {
        // TODO
        loco.afficherMessage("Demande d'accès à la section partagée.");
        semaphore.release(); // Libération de la section
        afficher_message(qPrintable(QString("La locomotive %1 quitte la section partagée.").arg(loco.numero())));
    }

private:
    /* A vous d'ajouter ce qu'il vous faut */

    // Méthodes privées ...
    // Attribut privés ...
    PcoMutex mutex;
    PcoSemaphore semaphore;
    bool isUsed;
    int premierAiguillageHoraire;
    int secondAiguillageHoraire;
    int premierAiguillageAntiHoraire;
    int secondAiguillageAntiHoraire;
};


#endif // SHAREDSECTION_H

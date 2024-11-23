//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //

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
    SharedSection() {
        // TODO
        semaphoreSection.release();
        estOccupee = false;
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée par une autre locomotive.
     * Si la locomotive et son thread ont été mis en attente,
     * le thread doit être réveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarrée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie d'accéder à la section partagée
     */
    void access(Locomotive &loco) override {
        // TODO
        mutexSection.lock();
        if (estOccupee) {
            loco.arreter();
            mutexSection.unlock();
        }
        semaphoreSection.acquire();
        loco.demarrer();
        estOccupee = true;
        mutexSection.unlock();
        afficher_message(qPrintable(QString("La locomotive n° %1 accède à la section partagée.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (réveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive &loco) override {
        // TODO
        mutexSection.lock();
        estOccupee = false;
        semaphoreSection.release();
        mutexSection.unlock();
        afficher_message(qPrintable(QString("La locomotive n° %1 quitte la section partagée.").arg(loco.numero())));
    }

private:
    /* A vous d'ajouter ce qu'il vous faut */
    bool estOccupee = false; // Indique si la section est occupée
    PcoSemaphore semaphoreSection; // Sémaphore pour gérer l'accès à la section partagée
    PcoMutex mutexSection; // Mutex pour protéger les ressources critiques
};

#endif // SHAREDSECTION_H

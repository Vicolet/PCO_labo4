#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <queue>
#include <QDebug>
#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>
#include "sharedsectioninterface.h"
#include "locomotive.h"

class SharedSection : public SharedSectionInterface {
public:
    SharedSection() : priorityMode(PriorityMode::HIGH_PRIORITY), isUsed(false) {
        section_semaphore.release(); // Initialisation du sémaphore
    }

    void request(Locomotive& loco, int priority) override {
        section_mutex.lock();
        requests.emplace(priority, loco.numero());
        section_mutex.unlock();
        afficher_message(qPrintable(QString("Locomotive %1 demande l'accès avec priorité %2.")
                                    .arg(loco.numero()).arg(priority)));
    }

    void access(Locomotive& loco, int priority) override {
        section_mutex.lock();

        while (isUsed || (priorityMode == PriorityMode::HIGH_PRIORITY && hasHigherPriority(priority))) {
            loco.afficherMessage("En attente...");
            section_mutex.unlock();
            section_semaphore.acquire(); // Attend que la section soit libérée
            section_mutex.lock();
        }

        isUsed = true;
        section_mutex.unlock();
        afficher_message(qPrintable(QString("Locomotive %1 accède à la section partagée.")
                                    .arg(loco.numero())));
    }

    void leave(Locomotive& loco) override {
        section_mutex.lock();
        isUsed = false;

        if (!requests.empty()) {
            section_semaphore.release();
            requests.pop();
        }

        section_mutex.unlock();
        afficher_message(qPrintable(QString("Locomotive %1 quitte la section partagée.")
                                    .arg(loco.numero())));
    }

    void togglePriorityMode() override {
        section_mutex.lock();
        if (priorityMode == PriorityMode::HIGH_PRIORITY) {
            priorityMode = PriorityMode::LOW_PRIORITY;
            afficher_message("Mode de priorité bas activé.");
        } else {
            priorityMode = PriorityMode::HIGH_PRIORITY;
            afficher_message("Mode de priorité haut activé.");
        }
        section_mutex.unlock();
    }

private:
    /**
     * @brief Vérifie si une locomotive avec une priorité plus élevée est en attente.
     * @param currentPriority La priorité de la locomotive actuelle.
     * @return true si une locomotive avec une priorité plus élevée attend.
     */
    bool hasHigherPriority(int currentPriority) {
        std::priority_queue<std::pair<int, int>> tempQueue = requests;
        while (!tempQueue.empty()) {
            const auto& [priority, locoId] = tempQueue.top();
            if (priority > currentPriority) {
                return true;
            }
            tempQueue.pop();
        }
        return false;
    }

    bool isUsed; ///< Indique si la section est utilisée
    PriorityMode priorityMode; ///< Mode de gestion des priorités
    PcoSemaphore section_semaphore; ///< Sémaphore pour gérer l'accès à la section
    PcoMutex section_mutex; ///< Mutex pour protéger les ressources critiques
    std::priority_queue<std::pair<int, int>> requests; ///< File des demandes d'accès (priorité, ID locomotive)
};

#endif // SHAREDSECTION_H

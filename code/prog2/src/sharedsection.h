#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <queue>
#include <QDebug>
#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>
#include "sharedsectioninterface.h"
#include "locomotive.h"

/**
 * @brief Classe gérant l'accès à la section partagée avec gestion des priorités.
 */
class SharedSection : public SharedSectionInterface {
public:
    SharedSection() : isUsed(false), section_semaphore(1) {}

    void request(Locomotive& loco, int priority) override {
        std::lock_guard<PcoMutex> lock(section_mutex);
        requests.emplace(priority, loco.numero());
        afficher_message(qPrintable(QString("Locomotive %1 demande l'accès avec priorité %2.")
                                    .arg(loco.numero()).arg(priority)));
    }

    void access(Locomotive& loco, int priority) override {
        while (true) {
            {
                std::lock_guard<PcoMutex> lock(section_mutex);

                // Vérifie si la section est disponible et si la locomotive actuelle a la priorité
                if (!isUsed && canAccess(priority, loco.numero())) {
                    isUsed = true;
                    currentLoco = loco.numero();
                    removeRequest(loco.numero());
                    afficher_message(qPrintable(QString("Locomotive %1 accède à la section partagée.")
                                                .arg(loco.numero())));
                    break;
                }
            }

            // Attend que la section soit disponible (sémaphore)
            loco.afficherMessage("En attente d'accès à la section partagée...");
            section_semaphore.acquire();
        }
    }

    void leave(Locomotive& loco) override {
        {
            std::lock_guard<PcoMutex> lock(section_mutex);

            isUsed = false;
            currentLoco = -1;

            afficher_message(qPrintable(QString("Locomotive %1 quitte la section partagée.")
                                        .arg(loco.numero())));
        }

        // Libère la prochaine locomotive en attente
        section_semaphore.release();
    }

    void togglePriorityMode() override {
        std::lock_guard<PcoMutex> lock(section_mutex);
        if (priorityMode == PriorityMode::HIGH_PRIORITY) {
            priorityMode = PriorityMode::LOW_PRIORITY;
            afficher_message("Mode de priorité bas activé.");
        } else {
            priorityMode = PriorityMode::HIGH_PRIORITY;
            afficher_message("Mode de priorité haut activé.");
        }
    }

private:
    /**
     * @brief Vérifie si la locomotive actuelle peut accéder à la section.
     * @param priority La priorité de la locomotive actuelle.
     * @param locoId L'identifiant de la locomotive actuelle.
     * @return true si la locomotive peut accéder à la section.
     */
    bool canAccess(int priority, int locoId) {
        if (requests.empty()) {
            return true;
        }

        auto [highestPriority, waitingLocoId] = requests.top();
        return priority <= highestPriority && locoId == waitingLocoId;
    }

    /**
     * @brief Supprime une locomotive de la file des demandes.
     * @param locoId L'identifiant de la locomotive à supprimer.
     */
    void removeRequest(int locoId) {
        std::priority_queue<std::pair<int, int>> tempQueue;

        // Copie toutes les locomotives sauf celle à supprimer dans une file temporaire
        while (!requests.empty()) {
            auto [priority, id] = requests.top();
            requests.pop();
            if (id != locoId) {
                tempQueue.emplace(priority, id);
            }
        }

        // Remet les demandes dans la file originale
        requests = std::move(tempQueue);
    }

    bool isUsed; ///< Indique si la section est utilisée
    int currentLoco = -1; ///< ID de la locomotive actuelle
    PriorityMode priorityMode = PriorityMode::HIGH_PRIORITY; ///< Mode de gestion des priorités
    PcoSemaphore section_semaphore; ///< Sémaphore pour gérer l'attente
    PcoMutex section_mutex; ///< Mutex pour protéger les ressources critiques
    std::priority_queue<std::pair<int, int>> requests; ///< File des demandes d'accès (priorité, ID locomotive)
};

#endif // SHAREDSECTION_H

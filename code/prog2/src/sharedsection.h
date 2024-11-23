#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <queue>
#include <QDebug>
#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>
#include "sharedsectioninterface.h"
#include "locomotive.h"

/**
 * @brief Classe gérant l'accès à la section partagée avec gestion des priorités et des aiguillages.
 */
class SharedSection : public SharedSectionInterface {
public:
    SharedSection() : isUsed(false), section_semaphore(1) {}

    /**
     * @brief Une locomotive demande l'accès à la section.
     */
    void request(Locomotive& loco, int priority) override {
        std::lock_guard<PcoMutex> lock(section_mutex);
        requests.emplace(priority, loco.numero());
        afficher_message(qPrintable(QString("Locomotive %1 demande l'accès avec priorité %2.")
                                    .arg(loco.numero()).arg(priority)));
    }

    /**
     * @brief Une locomotive tente d'accéder à la section.
     */
    void access(Locomotive& loco, int priority) override {
        while (true) {
            {
                std::lock_guard<PcoMutex> lock(section_mutex);

                // Vérifie si la locomotive peut accéder à la section (priorité, aiguillage)
                if (!isUsed && canAccess(priority, loco.numero())) {
                    isUsed = true;
                    currentLoco = loco.numero();
                    removeRequest(loco.numero());

                    // Configurer les aiguillages pour la locomotive
                    configureSwitchesForLoco(loco);

                    afficher_message(qPrintable(QString("Locomotive %1 accède à la section partagée.")
                                                .arg(loco.numero())));
                    break;
                }
            }

            // Attend que la section soit disponible
            loco.afficherMessage("En attente d'accès à la section partagée...");
            section_semaphore.acquire();
        }
    }

    /**
     * @brief Une locomotive quitte la section.
     */
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

    /**
     * @brief Change le mode de priorité.
     */
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
     * @brief Vérifie si une locomotive peut accéder à la section partagée.
     */
    bool canAccess(int priority, int locoId) {
        if (requests.empty()) {
            return true;
        }

        // Vérifie si la locomotive est la prochaine dans la file des priorités
        auto [highestPriority, waitingLocoId] = requests.top();

        return (priority <= highestPriority && locoId == waitingLocoId);
    }

    /**
     * @brief Supprime une locomotive de la file des demandes.
     */
    void removeRequest(int locoId) {
        std::priority_queue<std::pair<int, int>> tempQueue;

        while (!requests.empty()) {
            auto [priority, id] = requests.top();
            requests.pop();
            if (id != locoId) {
                tempQueue.emplace(priority, id);
            }
        }

        requests = std::move(tempQueue);
    }

    /**
     * @brief Configure les aiguillages pour la locomotive donnée.
     */
    void configureSwitchesForLoco(Locomotive& loco) {
        if (loco.numero() == 7) {
            // Exemple de configuration des aiguillages pour la locomotive 7
            diriger_aiguillage(10, DEVIE, 0);
            diriger_aiguillage(13, DEVIE, 0);
            afficher_message("Aiguillages configurés pour locomotive 7.");
        } else if (loco.numero() == 42) {
            // Exemple de configuration des aiguillages pour la locomotive 42
            diriger_aiguillage(10, TOUT_DROIT, 0);
            diriger_aiguillage(13, TOUT_DROIT, 0);
            afficher_message("Aiguillages configurés pour locomotive 42.");
        }
    }

    bool isUsed; ///< Indique si la section est utilisée
    int currentLoco = -1; ///< ID de la locomotive actuelle
    PriorityMode priorityMode = PriorityMode::HIGH_PRIORITY; ///< Mode de gestion des priorités
    PcoSemaphore section_semaphore; ///< Sémaphore pour gérer l'attente
    PcoMutex section_mutex; ///< Mutex pour protéger les ressources critiques
    std::priority_queue<std::pair<int, int>> requests; ///< File des demandes d'accès (priorité, ID locomotive)
};

#endif // SHAREDSECTION_H

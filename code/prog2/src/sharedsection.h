#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <queue>
#include <vector>
#include <algorithm>
#include <QDebug>

#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcosemaphore.h>

#include "ctrain_handler.h"
#include "locomotive.h"
#include "sharedsectioninterface.h"

/**
 * @brief Classe gérant l'accès à la section partagée avec gestion des priorités.
 */
class SharedSection final : public SharedSectionInterface {
public:
    SharedSection()
        : semaphoreSection(0),
          mutexSection(1),
          estOccupee(false),
          priorityMode(PriorityMode::HIGH_PRIORITY) {}

    /**
     * @brief Une locomotive demande l'accès à la section avec une priorité donnée.
     * @param loco La locomotive demandant l'accès.
     * @param priority La priorité associée à cette demande.
     */
    void request(Locomotive& loco, int priority) override {
        mutexSection.acquire(); // Protéger la file des demandes
        requests.emplace(priority, loco.numero());
        afficher_message(qPrintable(QString("Locomotive %1 demande l'accès avec priorité %2.")
                                    .arg(loco.numero())
                                    .arg(priority)));
        mutexSection.release();
    }

    /**
     * @brief Une locomotive essaie d'accéder à la section partagée.
     * Elle attend si la section est occupée ou si elle n'a pas la priorité.
     * @param loco La locomotive qui tente d'accéder.
     * @param priority La priorité associée à la locomotive.
     */
    void access(Locomotive& loco, int priority) override {
        mutexSection.acquire();
        while (estOccupee || !canAccess(priority, loco.numero())) {
            loco.arreter();
            loco.afficherMessage("En attente d'accès à la section partagée...");
            mutexSection.release();
            semaphoreSection.acquire(); // Attend qu'une notification soit envoyée
            mutexSection.acquire();
        }

        // La locomotive a la priorité et peut accéder
        estOccupee = true;
        currentLoco = loco.numero();
        removeRequest(loco.numero());
        loco.demarrer();
        afficher_message(qPrintable(QString("Locomotive %1 accède à la section partagée.").arg(loco.numero())));
        mutexSection.release();
    }

    /**
     * @brief Une locomotive quitte la section.
     * Réveille les locomotives en attente et libère la section.
     * @param loco La locomotive quittant la section.
     */
    void leave(Locomotive& loco) override {
        mutexSection.acquire();
        estOccupee = false;
        currentLoco = -1;

        // Notifie une autre locomotive en attente
        if (!requests.empty()) {
            semaphoreSection.release();
        }
        afficher_message(qPrintable(QString("Locomotive %1 quitte la section partagée.").arg(loco.numero())));
        mutexSection.release();
    }

    /**
     * @brief Change le mode de priorité (HIGH_PRIORITY ou LOW_PRIORITY).
     */
    void togglePriorityMode() override {
        mutexSection.acquire();
        priorityMode = (priorityMode == PriorityMode::HIGH_PRIORITY)
                           ? PriorityMode::LOW_PRIORITY
                           : PriorityMode::HIGH_PRIORITY;
        afficher_message(priorityMode == PriorityMode::HIGH_PRIORITY
                             ? "Mode de priorité haut activé."
                             : "Mode de priorité bas activé.");
        mutexSection.release();
    }

private:
    /**
     * @brief Retourne la locomotive ayant la priorité actuelle, selon le mode.
     * @return Le numéro de la locomotive ayant la priorité.
     */
    bool canAccess(int priority, int locoId) {
        if (requests.empty()) {
            return true; // Pas de demandes
        }

        auto [highestPriority, topLocoId] = requests.top();
        return (priority <= highestPriority && locoId == topLocoId);
    }

    /**
     * @brief Retire une locomotive de la file des demandes.
     * @param locoId Le numéro de la locomotive à retirer.
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

    bool estOccupee; ///< Indique si la section est occupée
    int currentLoco = -1; ///< Numéro de la locomotive actuelle dans la section
    PriorityMode priorityMode; ///< Mode de gestion des priorités
    PcoSemaphore semaphoreSection; ///< Sémaphore pour gérer les locomotives en attente
    PcoSemaphore mutexSection; ///< Mutex pour protéger les ressources critiques
    std::priority_queue<std::pair<int, int>> requests; ///< File des demandes d'accès
};

#endif // SHAREDSECTION_H

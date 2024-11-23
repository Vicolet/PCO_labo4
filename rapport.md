
# Titre du laboratoire

Auteurs: Camille Koestli, Victor Nicolet

## Description des fonctionnalités du logiciel

Ce laboratoire a pour but de simuler le fonctionnement d'un réseau ferroviaire à travers la gestion de ressources partagées. Ces ressources comprennent le tronçon commun, qui est emprunté par une seule locomotive à la fois et une gare où les locomotives doivent s'attendre à chaque tour.

La gestion d'entrée dans le tronçon commun est accordé à la locomotive qui a une priorité supérieur à l'autre. Si les deux locomotives ont la même priorité, alors elle sera accordée à la première arrivée sur le tronçon commun.

![parcours-choisi.png](/imgs/parcours-choisi.jpg)

## Choix d'implémentation

### Gestion de l'attente en gare

La classe `SharedStation` gère l’attente des locomotives dans une gare. Elle régule l'entrée et sortie en fonction du nombre maximum de locomotives autorisées à attendre.

Le sémaphore `semaphoreEntreeStation` contrôle le nombre de locomotives pouvant entrer dans la station.
La sémaphore `semaphoreAttenteStation` gère le nombre maximum de locomotives pouvant attendre dans la gare.
Lorsqu'une locomotive atteint le seuil des tours prédéfinis, elle appelle `waitingAtStation()`, ce qui met en pause son thread jusqu'à ce qu'elle soit autorisée à repartir.
La variable `locosEnAttente` indique si des locomotives sont actuellement en attente. Cela pourrait être utile pour une gestion avancée des priorités.

Cela illustre un usage efficace des sémaphores pour synchroniser plusieurs threads sur une ressource partagée.

### Gestion de l'accès au tronçon commun

La classe `SharedSection` représente un tronçon partagé du réseau ferroviaire. Ce tronçon est une ressource critique qui ne peut être utilisée que par une locomotive à la fois. L’objectif principal de cette classe est de garantir l’exclusion mutuelle dans l’accès à cette section.

- Accès à la section : Lorsque la méthode `access` est appelée par une locomotive, elle vérifie si la section est occupée. Si oui, la locomotive s'arrête, et son thread est mis en attente jusqu'à ce que la section soit libérée. Cette attente est gérée par un sémaphore (`semaphoreSection`) qui permet de bloquer les threads en attente.
- Sortie de la section : Une fois que la locomotive quitte la section, elle appelle `leave`, qui libère le sémaphore et réveille les threads en attente. Un verrou (`mutexSection`) protège l'accès aux indicateurs d'état internes (`estOccupee`).

Ce mécanisme évite les conditions de course (data races) et garantit une synchronisation correcte.

### Gestion de la priorité

## Tests effectués

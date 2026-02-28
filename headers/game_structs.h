/**
 * @file game_structs.h
* @brief Contiene le struct implementate nel gioco
*/
#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <stdbool.h>

#define MAX_PLAYERS 20
#define MIN_PLAYERS 2
#define DEFAULT_LIVES 2
#define DECK_SIZE 40
#define CARDS_PER_PLAYER 2
/**
 * @brief Rappresenta una singola carta da gioco.
 */
typedef struct {
    char value[3];      // Valore carta: "1"-"7", "J", "Q", "K"
    char seed[10];      // Seme: "Fiori", "Picche", "Quadri", "Cuori"
    bool isHidden;      // Se la carta è coperta
} Card;
/**
 * @brief Struttura che rappresenta un giocatore.
 */
typedef struct {
    char name[25];      // Nome giocatore
    Card cards[CARDS_PER_PLAYER]; // Carte in mano
    int index;          // Indice del giocatore
    int lives;          // Punti vita
    bool isEliminated;  // Se è eliminato
    bool effectApplied[CARDS_PER_PLAYER]; // Se l'effetto è stato applicato
    bool pendingEffect[CARDS_PER_PLAYER]; // Se l'effetto è in attesa
} Player;
/**
 * @brief Stato globale del gioco.
 */
typedef struct {
    Player* players;            // Array giocatori attivi
    Player* eliminatedPlayers;  // Array giocatori eliminati
    Card* deck;                 // Mazzo di carte
    int playersCount;           // Numero giocatori totali
    int activePlayers;          // Numero giocatori attivi
    int startingLives;          // Vite iniziali
    int livesOnField;           // Vite sul campo di gioco
} GameState;


#endif // GAME_STRUCTS_H

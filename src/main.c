/**
 * @file main.c
 * @brief Funzione principale del gioco di carte.
 *
 * Inizializza il gioco, gestisce il ciclo principale dei turni,
 * e stampa il vincitore al termine della partita.
 */

#include <stdio.h>
#include "../headers/game_structs.h"
#include "../headers/game_logic.h"
/**
 * @brief Punto di ingresso del programma.
 *
 * Inizializza lo stato del gioco, avvia il ciclo principale,
 * e determina il vincitore una volta che resta un solo giocatore.
 *
 * @return int Codice di uscita del programma.
 */
int main() {
    GameState game;
    
    printf("=== GIOCO DI CARTE ===\n");
    initializeGame(&game);
    
    // Game loop principale
    while (game.activePlayers > 1) {
        playPhase(&game);
    }
    
    // Trova e stampa il vincitore
    for (int i = 0; i < game.playersCount; i++) {
        if (!game.players[i].isEliminated) {
            printf("\n\U0001F3C6 %s ha vinto la partita con %d vite rimaste! \U0001F3C6\n", 
                   game.players[i].name, game.players[i].lives);
            break;
        }
    }
    
    cleanupGame(&game);
    return 0;
}

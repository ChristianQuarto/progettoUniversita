/**
 * @file game_logic.c
 * @brief Contiene l'intera logica del gioco di carte, inclusa l'inizializzazione, la gestione dei turni,
 * gli effetti delle carte e la visualizzazione dello stato di gioco.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/game_logic.h"
#include <ctype.h>
//---------------------------------- Funzioni di utilità -------------------------------------
/**
 * @brief Pulisce il buffer di input standard.
 */
void clearInputBuffer() {
    while (getchar() != '\n');
}
/**
 * @brief Chiede un input intero all'utente entro un intervallo.
 * @param prompt Messaggio mostrato all'utente.
 * @param min Valore minimo accettato.
 * @param max Valore massimo accettato.
 * @return int Valore inserito valido.
 */
int getIntegerInput(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s (%d-%d): ", prompt, min, max);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            clearInputBuffer();
            return value;
        }
        clearInputBuffer();
        printf("Input non valido. Riprova.\n");
    }
}
/**
 * @brief Attende che l'utente prema invio per continuare.
 */
void pressEnterToContinue() {
    printf("\nPremi Invio per continuare...");
  //  clearInputBuffer();
    getchar();
}

//--------------------------------- Inizializzazione ------------------------------------------
/**
 * @brief Inizializza un mazzo di carte standard.
 * @param deck Puntatore all'array di carte da inizializzare.
 */
void initializeDeck(Card* deck) {
    char* seeds[] = {"Fiori", "Picche", "Quadri", "Cuori"};
    char* values[] = {"1", "2", "3", "4", "5", "6", "7", "J", "Q", "K"};
    
    int cardIndex = 0;
    for (int s = 0; s < 4; s++) {
        for (int v = 0; v < 10; v++) {
            strncpy(deck[cardIndex].value, values[v], sizeof(deck[cardIndex].value) - 1);
            strncpy(deck[cardIndex].seed, seeds[s], sizeof(deck[cardIndex].seed) - 1);
            deck[cardIndex].isHidden = true;
            cardIndex++;
        }
    }
}
/**
 * @brief Mescola il mazzo di carte.
 * @param deck Puntatore al mazzo da mescolare.
 * @param size Numero di carte nel mazzo.
 */
void shuffleDeck(Card* deck, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}
/**
 * @brief Inizializza i giocatori.
 * @param players Array di giocatori.
 * @param count Numero totale di giocatori.
 * @param lives Vite iniziali per ciascun giocatore.
 */
void initializePlayers(Player* players, int count, int lives) {
    for (int i = 0; i < count; i++) {
        snprintf(players[i].name, sizeof(players[i].name), "Giocatore %d", i + 1);
        players[i].index = i;
        players[i].lives = lives;
        players[i].isEliminated = false;
        
        for (int j = 0; j < CARDS_PER_PLAYER; j++) {
            players[i].cards[j].value[0] = '\0';
            players[i].cards[j].seed[0] = '\0';
            players[i].cards[j].isHidden = true;
            players[i].pendingEffect[j] = false;
            players[i].effectApplied[j] = false;
        }
    }
}
//------------------------------------ Visualizzazione ----------------------------------------
/**
 * @brief Stampa una singola carta.
 * @param card Puntatore alla carta da stampare.
 */
void printCard(const Card* card) {
    if (card->isHidden) {
        printf("[Carta Coperta]");
    } else {
        printf("%s di %s", card->value, card->seed);
    }
}
/**
 * @brief Stampa lo stato attuale di un giocatore.
 * @param player Puntatore al giocatore.
 */
void printPlayerStatus(const Player* player) {
    if (player -> lives == 1)
    {
       printf("\n%s (%d vita)\n", player->name, player->lives);  
    }
    else
    {
        printf("\n%s (%d vite)\n", player->name, player->lives);
    }
   
    printf("  Carta 1: "); printCard(&player->cards[0]); printf("\n");
    printf("  Carta 2: "); printCard(&player->cards[1]); printf("\n");
}
/**
 * @brief Stampa lo stato attuale del gioco.
 * @param game Puntatore allo stato del gioco.
 */
void printGameStatus(const GameState* game) {
    printf("\n=== STATO DEL GIOCO ===\n");
    printf("Vite sul campo: %d\n", game->livesOnField);
    printf("Giocatori attivi: %d/%d\n\n", game->activePlayers, game->playersCount);
    
    for (int i = 0; i < game->playersCount; i++) {
        if (!game->players[i].isEliminated) {
            printPlayerStatus(&game->players[i]);
        }
    }
    
    if (game->playersCount - game->activePlayers > 0) {
        printf("\nGiocatori eliminati:\n");
        for (int i = 0; i < game->playersCount - game->activePlayers; i++) {
            printf("- %s\n", game->eliminatedPlayers[i].name);
        }
    }
}
//-------------------------------------- Logica gioco -----------------------------------------
/**
 * @brief Distribuisce carte ai giocatori attivi.
 * @param game Puntatore allo stato del gioco.
 */
void distributeCards(GameState* game) {
    shuffleDeck(game->deck, DECK_SIZE);
    
    int deckIndex = 0;
    for (int i = 0; i < game->playersCount; i++) {
        if (game->players[i].isEliminated) continue;
        
        for (int j = 0; j < CARDS_PER_PLAYER; j++) {
            game->players[i].cards[j] = game->deck[deckIndex++];
            if (j == 1) {
                game->players[i].cards[j].isHidden = false;
            }
            // Reset degli effetti per la nuova fase
            game->players[i].effectApplied[j] = false;
            game->players[i].pendingEffect[j] = false;
        }
    }
}
/**
 * @brief Rivela una carta coperta del giocatore.
 * @param player Puntatore al giocatore.
 * @param cardIndex Indice della carta da rivelare.
 */
void revealCard(Player* player, int cardIndex) {
    if (player->cards[cardIndex].isHidden) {
        player->cards[cardIndex].isHidden = false;
    }
}
/**
 * @brief Trova il prossimo giocatore attivo.
 * @param game Puntatore allo stato del gioco.
 * @param start Indice del giocatore attuale.
 * @param direction Direzione di ricerca (-1 o +1).
 * @return Indice del prossimo giocatore attivo, oppure -1 se nessuno.
 */
int findNextActivePlayer(GameState* game, int start, int direction) {
    int current = (start + direction + game->playersCount) % game->playersCount;
    while (game->players[current].isEliminated) {
        current = (current + direction + game->playersCount) % game->playersCount;
        if (current == start) return -1; // Nessun giocatore attivo trovato
    }
    return current;
}
/**
 * @brief Esegue l'effetto di una carta scoperta.
 * @param game Puntatore allo stato del gioco.
 * @param playerIndex Indice del giocatore corrente.
 * @param cardIndex Indice della carta da attivare.
 */
void executeCardEffect(GameState* game, int playerIndex, int cardIndex) {
    Player* currentPlayer = &game->players[playerIndex];
    Card* card = &currentPlayer->cards[cardIndex];
    
    if (card->isHidden || currentPlayer->effectApplied[cardIndex]) return;
    
    int prevPlayer = findNextActivePlayer(game, playerIndex, -1);
    int nextPlayer = findNextActivePlayer(game, playerIndex, 1);
    int nextNextPlayer = (nextPlayer != -1) ? findNextActivePlayer(game, nextPlayer, 1) : -1;
    
    switch (card->value[0]) {
        case '1':
            if (currentPlayer->lives > 0) {
                currentPlayer->lives--;
                game->livesOnField++;
                printf("\n%s perde 1 vita con la carta 1! (Vite sul campo: %d)\n", 
                       currentPlayer->name, game->livesOnField);
            }
            break;
            
        case 'J':
            if (currentPlayer->lives > 0 && prevPlayer != -1) {
                currentPlayer->lives--;
                game->players[prevPlayer].lives++;
                printf("\n%s da 1 vita a %s con la carta J!\n", 
                       currentPlayer->name, game->players[prevPlayer].name);
            } else if (prevPlayer == -1) {
                printf("\nNessun giocatore valido a cui dare vite con la carta J!\n");
            }
            break;
            
        case 'Q':
            if (currentPlayer->lives > 0 && nextNextPlayer != -1) {
                currentPlayer->lives--;
                game->players[nextNextPlayer].lives++;
                printf("\n%s da 1 vita a %s con la carta Q!\n", 
                       currentPlayer->name, game->players[nextNextPlayer].name);
            } else if (nextNextPlayer == -1) {
                printf("\nNessun giocatore valido a cui dare vite con la carta Q!\n");
            }
            break;
            
        case 'K':
            if (game->livesOnField > 0) {
                currentPlayer->lives += game->livesOnField;
                if (game -> livesOnField == 1)
                {
                     printf("\n%s raccoglie %d vita dal campo con la carta K!\n", 
                       currentPlayer->name, game->livesOnField);
                }
                else
                {
                    printf("\n%s raccoglie %d vite dal campo con la carta K!\n", 
                       currentPlayer->name, game->livesOnField);
                }
                
                game->livesOnField = 0;
            }
            else
            {
                printf("\nNon ci sono vite sul campo!\n");
            }
            break;
            
        case '7':
            if (nextPlayer != -1) {
                Player* target = &game->players[nextPlayer];
                 if (target->cards[0].isHidden) {
                        target->cards[0].isHidden = false;
                        target->pendingEffect[0] = true;
                        printf("\n%s è costretto a scoprire la carta: ", target->name);
                        printCard(&target->cards[0]);
                        printf("\n");
                    }
            } else {
                printf("\nNessun giocatore valido da forzare!\n");
            }
            break;
            
        default: // Carte 2-6: nessun effetto
            printf("\nLa carta %s di %s non ha effetto\n", card->value, card->seed);
            break;
    }
    
    currentPlayer->effectApplied[cardIndex] = true;
    currentPlayer->pendingEffect[cardIndex] = false;
}
/**
 * @brief Elimina i giocatori con 0 vite.
 * @param game Puntatore allo stato del gioco.
 */
void eliminatePlayers(GameState* game) {
    for (int i = 0; i < game->playersCount; i++) {
        if (!game->players[i].isEliminated && game->players[i].lives <= 0) {
            game->players[i].isEliminated = true;
            game->eliminatedPlayers[game->playersCount - game->activePlayers] = game->players[i];
            game->activePlayers--;
            printf("\n%s è stato eliminato!\n", game->players[i].name);
        }
    }
}



//------------------------------------ Game Loop ---------------------------------------------
/**
 * @brief Chiede al giocatore se vuole scoprire la carta coperta.
 * @param player Puntatore al giocatore.
 * @return true Se il giocatore accetta, false altrimenti.
 */
bool askToRevealCard(const Player* player) {
    if (!player->cards[0].isHidden) {
        printf("\nLa tua carta coperta è già stata scoperta!\n");
        return false;
    }
    
    printf("\nVuoi scoprire la tua carta coperta? (S/N): ");
    char choice;
    scanf(" %c", &choice);
    clearInputBuffer();
    // default da N qualsiasi cosa viene inserita.Dice si solo se s e S
    return toupper(choice) == 'S';
}
/**
 * @brief Gestisce un turno di gioco per un giocatore specifico.
 * @param game Puntatore allo stato del gioco.
 * @param playerIndex Indice del giocatore attivo.
 */
void playTurn(GameState* game, int playerIndex) {
    Player* currentPlayer = &game->players[playerIndex];
    
    printf("\n=== TURNO DI %s ===\n", currentPlayer->name);
    printPlayerStatus(currentPlayer);
    
    // Risolvi effetti carte scoperte e pending
    for (int i = 0; i < CARDS_PER_PLAYER; i++) {
        if ((!currentPlayer->cards[i].isHidden && !currentPlayer->effectApplied[i]) || 
            currentPlayer->pendingEffect[i]) {
            executeCardEffect(game, playerIndex, i);
        }
    }
    
    // Chiedi se vuole scoprire la carta coperta
    if (currentPlayer->cards[0].isHidden) {
        if (askToRevealCard(currentPlayer)) {
            // Controlla se la carta 1 farebbe perdere l'ultima vita
            if ((strcmp(currentPlayer->cards[0].value, "1") == 0 || strcmp(currentPlayer->cards[0].value, "J") == 0 || (strcmp(currentPlayer->cards[0].value, "Q") == 0 && game -> activePlayers > 2) ) && currentPlayer->lives == 1) {
                printf("\nAttenzione! Scoprendo questa carta perderai l'ultima vita!\n");
                printf("\nConfermi? (S/N): ");
                char confirm;
                scanf(" %c", &confirm);
                clearInputBuffer();
                
                if (toupper(confirm) == 'S') {
                    revealCard(currentPlayer, 0);
                    executeCardEffect(game, playerIndex, 0);
                }
            } else {
                revealCard(currentPlayer, 0);
                executeCardEffect(game, playerIndex, 0);
            }
        }
    }
    
    pressEnterToContinue();
}

/**
 * @brief Gestisce una fase completa del gioco (distribuzione + turni + eliminazioni).
 * @param game Puntatore allo stato del gioco.
 */
void playPhase(GameState* game) {
    printf("\n=== NUOVA FASE ===\n");
    
    // Distribuisci carte
    distributeCards(game);
    
    // Scegli giocatore iniziale casuale
    int firstPlayer;
    do {
        firstPlayer = rand() % game->playersCount;
    } while (game->players[firstPlayer].isEliminated);
    
    // Gioca i turni in ordine ad anello
    for (int i = 0; i < game->playersCount; i++) {
        int currentPlayer = (firstPlayer + i) % game->playersCount;
        if (!game->players[currentPlayer].isEliminated) {
            playTurn(game, currentPlayer);
            printGameStatus(game);
        }
    }
    
    // Elimina giocatori senza vite
    eliminatePlayers(game);
}

//-------------------------------------- Main --------------------------------------------------
/**
 * @brief Inizializza l'intero stato del gioco.
 * @param game Puntatore allo stato del gioco da inizializzare.
 */
void initializeGame(GameState* game) {
    srand((unsigned int)time(NULL));
    
    // Configurazione iniziale
    game->playersCount = getIntegerInput("Numero di giocatori", MIN_PLAYERS, MAX_PLAYERS);
    game->startingLives = getIntegerInput("Vite per giocatore", 1, 10);
    game->activePlayers = game->playersCount;
    game->livesOnField = 0;
    
    // Allocazione memoria
    game->players = malloc(game->playersCount * sizeof(Player));
    game->eliminatedPlayers = malloc(game->playersCount * sizeof(Player));
    game->deck = malloc(DECK_SIZE * sizeof(Card));
    
    if (!game->players || !game->eliminatedPlayers || !game->deck) {
        printf("Errore di allocazione memoria!\n");
        exit(1);
    }
    
    // Inizializza componenti del gioco
    initializePlayers(game->players, game->playersCount, game->startingLives);
    initializeDeck(game->deck);
}
/**
 * @brief Libera la memoria allocata dinamicamente.
 * @param game Puntatore allo stato del gioco.
 */
void cleanupGame(GameState* game) {
    free(game->players);
    free(game->eliminatedPlayers);
    free(game->deck);
}




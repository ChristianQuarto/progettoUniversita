/**
 * @file game_logic.h
 * @brief Contiene i prototipi di funzione successivamente implementati in game_logic.c .
 */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_structs.h"

void clearInputBuffer();
int getIntegerInput(const char* prompt, int min, int max);
void pressEnterToContinue();
void initializeDeck(Card* deck);
void shuffleDeck(Card* deck, int size);
void initializePlayers(Player* players, int count, int lives);
void printCard(const Card* card);
void printPlayerStatus(const Player* player);
void printGameStatus(const GameState* game);
void distributeCards(GameState* game);
void revealCard(Player* player, int cardIndex);
int findNextActivePlayer(GameState* game, int start, int direction);
void executeCardEffect(GameState* game, int playerIndex, int cardIndex);
void eliminatePlayers(GameState* game);
bool askToRevealCard(const Player* player);
void playTurn(GameState* game, int playerIndex);
void playPhase(GameState* game);
void initializeGame(GameState* game);
void cleanupGame(GameState* game);
#endif // GAME_LOGIC_H

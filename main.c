#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int main(){

     // allocate memory for arrays
    Type *Types = malloc(sizeof(Type) * 19);
    Move *Moves = malloc(sizeof(Move) * 486);
    Pokemon *Pokemons = malloc(sizeof(Pokemon) * 1015);
    
    // create players
    Player Player1, Player2;
    
    // initialize all data from files
    initialize(Types, Moves, Pokemons, &Player1, &Player2);
    // start the game
    game(&Player1, &Player2);
    
    // free memory
    free(Types);
    free(Moves);
    free(Pokemons);

    return 0;
}

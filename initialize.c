#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"

// checks if value is already in the array
int isDuplicate(int* array, int size, int value){
    for(int i = 0; i < size; i++){
        if(array[i] == value)
        return 1;
    }
    return 0;
}

// generates a unique random number
int randomUnique(int min, int max, int chosen[], int *chosenCount){
    int i;
    do{
        i = min + rand() % (max - min + 1);
    }while(isDuplicate(chosen, *chosenCount, i));
    chosen[*chosenCount] = i;
    (*chosenCount)++;

    return i;
}

// reads types.txt and fills type data
void initializeTypes(Type *types){
    FILE *f = fopen("types.txt", "r");

// reads all types and their effectiveness data
 for(int i = 0; i < 18; i++){
    fscanf(f, "%s", types[i].name);
    
    // reads defender types and multipliers
    for(int j = 0; j < 18; j++){
        char defName[40];
        float multiplier;

     fscanf(f, "%s %f", defName, &multiplier);

     strcpy(types[i].effects[j].atkName, types[i].name);
     strcpy(types[i].effects[j].defName, defName);
     types[i].effects[j].multiplier = multiplier;
    }

    strcpy(types[i].effects[18].atkName, types[i].name);
    strcpy(types[i].effects[18].defName, "None");
    types[i].effects[18].multiplier = 1;
}

strcpy(types[18].name, "None");

// initialize None type effects
for(int j = 0; j < 18; j++){
    strcpy(types[18].effects[j].atkName, "None");
    strcpy(types[18].effects[j].defName, types[j].name);
    types[18].effects[j].multiplier = 1;
}

strcpy(types[18].effects[18].atkName, "None");
strcpy(types[18].effects[18].defName, "None");
types[18].effects[18].multiplier = 1;

fclose(f);
}

// reads moves from file and initializes moves
void initializeMoves(Move *moves, Type *types){
    FILE *f = fopen("moves.txt", "r");
    
    // reads each move data from file
    for(int i = 0; i < 486; i++){
        char moveName[30];
        char typeName[30];
        char categoryS[30];
        float power;
         
      fscanf(f,"%s %s %s %f", moveName, typeName, categoryS, &power);

        strcpy(moves[i].name, moveName);
        
        // finds type index for the move
        int typeIndex = -1;
        for (int k = 0; k < 19; k++){
            if (strcmp(types[k].name, typeName) == 0){
                typeIndex = k;
                break;
            }
        }

        moves[i].type = types[typeIndex];
        
        // choose move category. physical or special
        if(strcmp(categoryS, "Physical") == 0){
            moves[i].category = CATEGORY_PHYSICAL;
        }else if(strcmp(categoryS, "Special") == 0){
            moves[i].category = CATEGORY_SPECIAL;
        }else{
            printf("moves: invalid category\n");
            fclose(f);
            return;
        }

        moves[i].power = power;
    }
    fclose(f);
}

// reads pokemons from file and gives random moves
void initializePokemons(Pokemon *pokemons, Type *types, Move *moves){
    FILE *f = fopen("pokemon.txt", "r");
     
    // read 1015 pokemons
    for(int i = 0; i < 1015; i++){
        char pokemonName[30];
        char type1Name[30];
        char type2Name[30];
        int maxHP, attack, defense, spAtk, spDef, speed;
       
        // read one pokemon line
       fscanf(f,"%s %s %s %d %d %d %d %d %d",pokemonName, type1Name, type2Name,
       &maxHP, &attack, &defense, &spAtk, &spDef, &speed);

        strcpy(pokemons[i].name, pokemonName);
       
          // find type1 index
        int type1Index = -1;
        for(int k = 0; k < 19; k++){
            if(strcmp(types[k].name, type1Name) == 0){
                type1Index = k;
                break;
            }
        }

        pokemons[i].types[0] = types[type1Index];
       
         // type2 (None if "-")
        if(strcmp(type2Name, "-") == 0){
            pokemons[i].types[1] = types[18];
        }else{
            // find type2 index
            int type2Index = -1;
            for(int k = 0; k < 19; k++){
                if(strcmp(types[k].name, type2Name) == 0){
                    type2Index = k;
                    break;
                }
            }

           pokemons[i].types[1] = types[type2Index];
        }

        // pokemon stats
        pokemons[i].maxHP = maxHP;
        pokemons[i].currentHP = maxHP;
        pokemons[i].attack = attack;
        pokemons[i].defense = defense;
        pokemons[i].spAtk = spAtk;
        pokemons[i].spDef = spDef;
        pokemons[i].speed = speed;
        
         // choose 4 unique moves
        int chosen[4];
        int chosenSize = 0;
        for(int j = 0; j < 4; j++){
            int moveIndex = randomUnique(0, 485, chosen, &chosenSize);
            pokemons[i].moves[j] = moves[moveIndex];
        }
    }
    fclose(f);
}

// initializes types, moves, pokemons and players
void initialize(Type *types, Move *moves, Pokemon *pokemons, Player *p1, Player *p2){
    srand(time(NULL));
    
     // initialize arrays from files
    initializeTypes(types);
    initializeMoves(moves,types);
    initializePokemons(pokemons,types,moves);
    
    // player names
    strcpy(p1->name, "Tarik");
    strcpy(p2->name, "Yusuf");
    
    // chosen pokemon indexes (to avoid duplicates)
    int chosen[12];
    int chosenSize = 0;
    
    // give 6 random pokemons to player 1
    for(int i = 0; i < 6; i++){
        int pokemonIndex = randomUnique(0, 1014, chosen, &chosenSize);
        p1->Pokemons[i] = pokemons[pokemonIndex];
    }
    
    // give 6 random pokemons to player 2
    for(int i = 0; i < 6; i++){
        int pokemonIndex = randomUnique(0, 1014, chosen, &chosenSize);
        p2->Pokemons[i] = pokemons[pokemonIndex];
    }
    
     // start index
    p1->currentIndex = 1;
    p2->currentIndex = 1;
}












#include "structs.h"
#include <stdio.h>
#include <string.h>

// Does this player have at least one Pokemon still alive on their team?
int isTeamDefeated(Player p){
    for(int i = 0; i < 6; i++){
        if(p.Pokemons[i].currentHP > 0)
        return 0;
    }
    return 1;
}

// To find the index of the first alive Pokemon in the team (HP > 0)
int nextAliveIndex(Player *p){
    for(int i = 0; i < 6; i++){
        if(p->Pokemons[i].currentHP > 0)
        return i;
    }
    return -1;
}

// To find how many times (multiplier) moveType is effective against defType
float getTypeEffectMultiplier(Type moveType, Type defType){
    if(strcmp(defType.name,"None") == 0){
    return 1;
    }

    for(int i = 0; i < 19; i++){
        if(strcmp(moveType.effects[i].defName, defType.name) == 0){
            return moveType.effects[i].multiplier;
        }
    }
    return 1;
}

// This function calculates the STAB
float getSTAB(Pokemon attacker, Move move){
    if(strcmp(move.type.name,attacker.types[0].name) == 0){
    return 1.5;
     }

    if(strcmp(attacker.types[1].name,"None") != 0 && strcmp(move.type.name, attacker.types[1].name) == 0){
        return 1.5;
    }
    return 1.0;
}

// Calculates how much HP damage an attack deals to the defender
float calculateDamage(Pokemon attacker, Move move, Pokemon defender){
    float atkValue, defValue;

    if(move.category == CATEGORY_PHYSICAL){
        atkValue = (float)attacker.attack;
        defValue = (float)defender.defense;
    }else{
        atkValue = (float)attacker.spAtk;
        defValue = (float)defender.spDef;
    }

    float type1 = getTypeEffectMultiplier(move.type, defender.types[0]);
    float type2 = getTypeEffectMultiplier(move.type, defender.types[1]);
    float stab  = getSTAB(attacker, move);

    float damage = (float)move.power * (atkValue / defValue) * type1 * type2 * stab;

    return damage;
}

// Prints both players active Pokemon and their HP each turn
void showStatus(Player p1, Player p2){
    printf("\n----------------------------------\n");
    printf("%s: %s (HP: %d/%d)\n",p1.name,p1.Pokemons[p1.currentIndex].name,
              p1.Pokemons[p1.currentIndex].currentHP,
              p1.Pokemons[p1.currentIndex].maxHP);

    printf("%s: %s (HP: %d/%d)\n",p2.name,p2.Pokemons[p2.currentIndex].name,
           p2.Pokemons[p2.currentIndex].currentHP,
           p2.Pokemons[p2.currentIndex].maxHP);
    printf("-------------------------------------\n");
}

// Lists the Pokémon’s 4 moves on the screen
void showMoves(Pokemon p){
    printf("Moves:\n");
    for(int i = 0; i < 4; i++){
    printf("%d)  %s (Type: %s, Power: %.0f)\n", i + 1, p.moves[i].name, p.moves[i].type.name, p.moves[i].power);
    }
}

// Lists the player's Pokémons and lets them choose one with currentHP > 0
int choosePokemon(Player *p){
    int choice;

    while(1){
        printf("Available Pokemon (only alive ones can be selected):\n");

        for(int i = 0; i < 6; i++){
            if(p->Pokemons[i].currentHP > 0){
        printf("%d - %s (HP: %d/%d)\n", i + 1, p->Pokemons[i].name, p->Pokemons[i].currentHP, p->Pokemons[i].maxHP);
            }else{
                printf("%d - %s (FAINTED)\n", i + 1, p->Pokemons[i].name);
            }
        }

        printf("Select Pokemon (1-6): ");
        scanf("%d", &choice);

        choice--; // because of arrays start 0

        if(choice >= 0 && choice < 6 && p->Pokemons[choice].currentHP > 0){
            return choice;
        }else{
         printf("Invalid choice. Select an alive Pokemon\n");
        }
    }
}

// Shows the active Pokémon’s 4 moves, gets a 1–4 choice, returns it as 0–3 index
int chooseMove(Player *p){
    int choice;
    showMoves(p->Pokemons[p->currentIndex]);

    while(1){
        printf("Select move (1-4): ");
        scanf("%d", &choice);

        if(choice >= 1 && choice <= 4){
            return choice - 1;
        }else{
           printf("Invalid choice.\n");
         }
    }
}

// Applies a single attack: calculates damage, updates HP, and switches Pokemon if it faints
void doAttack(Player *attackerP, Player *defenderP, int moveIndex){
    Pokemon *attacker = &attackerP->Pokemons[attackerP->currentIndex];
    Pokemon *defender = &defenderP->Pokemons[defenderP->currentIndex];

    Move selectedMove = attacker->moves[moveIndex];

    float dmgF = calculateDamage(*attacker, selectedMove, *defender);
    int damage = (int)(dmgF + 0.5f);

    defender->currentHP -= damage;
    if(defender->currentHP < 0){
         defender->currentHP = 0;
    }

    printf("%s used %s! %d damage\n", attacker->name, selectedMove.name, damage);

    if(defender->currentHP == 0){
        printf("%s fainted!\n", defender->name);

        defenderP->currentIndex = nextAliveIndex(defenderP);
        if(defenderP->currentIndex != -1){
            printf("%s switched to: %s\n", defenderP->name, defenderP->Pokemons[defenderP->currentIndex].name);
        }
    }
}

// uses speed order, handles fainting and change option rules
void applyDamage(Player *p1, Player *p2,int p1Action, int p1MoveIndex,int p2Action, int p2MoveIndex){
    int p1CanAttack = (p1Action == 1);
    int p2CanAttack = (p2Action == 1);

    Pokemon *a = &p1->Pokemons[p1->currentIndex];
    Pokemon *b = &p2->Pokemons[p2->currentIndex];

    int p1First = (a->speed >= b->speed);

    if(p1First){
        if(p1CanAttack && !isTeamDefeated(*p2)){
            doAttack(p1, p2, p1MoveIndex);
        }
        if(p2CanAttack && !isTeamDefeated(*p1) && p2->Pokemons[p2->currentIndex].currentHP > 0){
            doAttack(p2, p1, p2MoveIndex);
        }
    }else{
        if(p2CanAttack && !isTeamDefeated(*p1)){
            doAttack(p2, p1, p2MoveIndex);
        }
        if(p1CanAttack && !isTeamDefeated(*p2) && p1->Pokemons[p1->currentIndex].currentHP > 0){
            doAttack(p1, p2, p1MoveIndex);
        }
    }
}

//  takes inputs for one round, updates currentIndex on change, then calls applyDamage
void playRound(Player *p1, Player *p2){
    int p1Action, p2Action;
    int p1MoveIndex = 0, p2MoveIndex = 0;

    showStatus(*p1, *p2);

    do{
        printf("\n%s choose:\n1 - Attack\n2 - Change Pokemon\nYour choice: ", p1->name);
        scanf("%d", &p1Action);
    }while(p1Action != 1 && p1Action != 2);

    do{
        printf("\n%s choose:\n1 - Attack\n2 - Change Pokemon\nYour choice: ", p2->name);
        scanf("%d", &p2Action);
    }while(p2Action != 1 && p2Action != 2);

    if(p1Action == 1){
        p1MoveIndex = chooseMove(p1);
    }else{
        int index = choosePokemon(p1);
        p1->currentIndex = index;
        printf("%s changed Pokemon -> %s\n", p1->name, p1->Pokemons[p1->currentIndex].name);
    }

    if(p2Action == 1){
        p2MoveIndex = chooseMove(p2);
    }else{
        int index = choosePokemon(p2);
        p2->currentIndex = index;
        printf("%s changed Pokemon -> %s\n", p2->name, p2->Pokemons[p2->currentIndex].name);
    }

    printf("Speed -> %s: %d | %s: %d\n",
           p1->Pokemons[p1->currentIndex].name, p1->Pokemons[p1->currentIndex].speed,
           p2->Pokemons[p2->currentIndex].name, p2->Pokemons[p2->currentIndex].speed);

    applyDamage(p1, p2, p1Action, p1MoveIndex, p2Action, p2MoveIndex);

    if(!isTeamDefeated(*p1) && p1->Pokemons[p1->currentIndex].currentHP <= 0){
        int nextIndex = nextAliveIndex(p1);
        if(nextIndex != -1){
            p1->currentIndex = nextIndex;
        }
    }
    if(!isTeamDefeated(*p2) && p2->Pokemons[p2->currentIndex].currentHP <= 0){
        int nextIndex = nextAliveIndex(p2);
        if(nextIndex != -1){
            p2->currentIndex = nextIndex;
        }
    }
}

//  calls round until game ends
void game(Player *p1, Player *p2){
    printf("Pokemon Battle Starts! %s vs %s\n", p1->name, p2->name);

    if(p1->Pokemons[p1->currentIndex].currentHP <= 0){
        p1->currentIndex = nextAliveIndex(p1);
    }
    if(p2->Pokemons[p2->currentIndex].currentHP <= 0){
        p2->currentIndex = nextAliveIndex(p2);
    }

    while(!isTeamDefeated(*p1) && !isTeamDefeated(*p2)){
        playRound(p1, p2);
    }

    if(isTeamDefeated(*p1)){
        printf("\nWell done! Winner: %s\n", p2->name);
    }else{
        printf("\nWell done! Winner: %s\n", p1->name);
    }
}



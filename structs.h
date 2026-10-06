#ifndef POKEMON_STRUCTS_H    // header guard
                               
#define POKEMON_STRUCTS_H    

// stores type effectiveness
typedef struct{
 char atkName[40];     
 char defName[40];    
 float multiplier;     
}TypeEffect;

// stores a type name and its effectiveness list
typedef struct{
    char name[30];         
    TypeEffect effects[19];  
 }Type;

 // shows move category
 typedef enum{
   CATEGORY_PHYSICAL,
    CATEGORY_SPECIAL
}Category;

// stores move info
 typedef struct{
   char name[30];    
   Type type;
   Category category;     
   float power;     
 }Move;

 // stores pokemon info
 typedef struct{
  char name[30];  
  Type types[2];   
  int maxHP;       
  int currentHP;    
  int attack;       
  int defense;      
  int spAtk;      
  int spDef;       
  int speed;       
  Move moves[4];
 }Pokemon;

 // stores player info
 typedef struct{
  char name[30];      
  Pokemon Pokemons[6];     
  int currentIndex;         
 }Player;

 // initialization functions for types, moves, pokemons and players
void initialize(Type *types, Move *moves, Pokemon *pokemons, Player *p1, Player* p2);
void initializeTypes(Type *types);
void initializeMoves(Move *moves, Type *types);
void initializePokemons(Pokemon *pokemons, Type *types, Move *moves);

// game functions for game loop, round play and damage calculation
void game(Player *p1, Player *p2);
void playRound(Player *p1, Player *p2);
void applyDamage(Player *p1, Player *p2,int p1Action, int p1MoveIndex,int p2Action, int p2MoveIndex);

#endif

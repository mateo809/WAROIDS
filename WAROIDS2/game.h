
//define the folder game.h for main and game.c

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.h>

#define PI 3.141592653589793238462643
#define MAX_JEEP 5
#define SPEED_JEEP 5

// all strucure

typedef struct Player
{
	int IsAlive;
	float x;
	float y;
	float currentx;
	float currenty;
	int rotation;
	float deceleration;
	float decelerationSpeed;
	float speed;
	int longueur;
	int score;
} Player;

typedef struct Tank
{
	int IsAlive;
	float x;
	float y;
	float currentx;
	float currenty;
	int rotation;
	float speed;

} Tank;

typedef struct Projectile 
{
	float ProjectileX;
	float ProjectileY;
	float projectileDirectionX;
	float projectileDirectionY;
	float projectileSpeed;
	float projectileSpeedX;
	float projectileSpeedY;
	float distanceProjectile;
} Projectile;

typedef struct Enemy 
{
	float x;
	float y;
	float randomDirection;
	sfSprite* sprite;
} Enemy;

typedef struct Enemmis //list
{
	float X[MAX_JEEP];
	float Y[MAX_JEEP];
	float randomDirection[MAX_JEEP];
	sfSprite* AsteoridsSprite[MAX_JEEP];
} Enemmis;

typedef struct Sprites //sprites for collisions 
{
	int SpaceShipSpriteWidth;
	int SpaceShipSpriteHeight;
	int AsteoridsSpriteWidth;
	int AsteoridsSpriteHeight;
	int ProjectileSpriteWidth;
	int ProjectileSpriteHeight;
} Sprites;

//all fonctions on game.c

void Sprite(sfRenderWindow* window, Player* player, Enemmis* enemies);
void deplacement(Player* player, sfEvent event);
void spawnAndMoveTank(Tank* tank, sfVideoMode videoMode);
void shootProjectile(Player* player, Projectile* projectile, sfSprite* projectileSprite);
void updateProjectile(Projectile* projectile, sfSprite* projectileSprite);
void UpdateAsteroidsPosition(Enemmis* enemmis, Sprites* Sprites);
void checkCollisions(Player* player, Enemmis* enemies, Projectile* projectile);


#endif
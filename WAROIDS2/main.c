#include <SFML/Graphics.h> //library
#include <SFML/Window.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "game.h" // .h folder

#define PI 3.141592653589793238462643
#define MAX_JEEP 5
#define SPEED_JEEP 5



int spriteWidth = 16.0f;      //sprite size
int spriteHeight = 16.0f;

int score = 0;

float distance(sfVector2f point1, sfVector2f point2)
{
	float dx = point1.x - point2.x;
	float dy = point1.y - point2.y;
	return sqrt(dx * dx + dy * dy);
}

//variables
sfEvent event;

sfSprite* soldat;        
sfSprite* mapSprite;
sfRenderWindow* window;
sfSprite* tank;                    
sfSprite* projectileSprite;
sfFont* fontScore;
sfText* scoreText;
sfTexture* jeepTexture;
sfFont* fontGO;
sfText* GameOverText;
sfText* victoryText;



int main()
{
	Player player;
	player.IsAlive = 1;
	player.x = 0;
	player.y = 0;
	player.rotation = 0;
	player.currentx = 960;
	player.currenty = 540;
	player.deceleration = 0.98;
	player.decelerationSpeed = 0.9809;
	player.speed = 1.0f;
	player.longueur = 1;
	player.score = 0;

	Tank tank1;
	tank1.IsAlive = 1;
	tank1.x = 0;
	tank1.y = 0;
	tank1.rotation = 0;
	tank1.currentx = 150;
	tank1.currenty = -0;
	tank1.speed = 3.0f;

	Projectile projectile;
	projectile.ProjectileX = 0;
	projectile.projectileDirectionY = 0;
	projectile.projectileSpeed = 0;
	projectile.distanceProjectile = 0;


	Sprites Sprites;
	Sprites.SpaceShipSpriteWidth = 64;
	Sprites.SpaceShipSpriteHeight = 64;
	Sprites.AsteoridsSpriteHeight = 64;
	Sprites.AsteoridsSpriteWidth = 64;
	Sprites.ProjectileSpriteHeight = 10;
	Sprites.ProjectileSpriteWidth = 10;


	Enemmis enemmis; //list
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX_JEEP; ++i)
	{
		enemmis.X[i] = -20;
		enemmis.Y[i] = -20;
		enemmis.randomDirection[i] = rand() % 360;
		enemmis.AsteoridsSprite[i] = sfSprite_create();
	}


	sfRenderWindow* window; //windowscreen
	sfVideoMode videoMode = { 1920, 1080, 64 };
	window = sfRenderWindow_create(videoMode, "WAROIDS", sfClose, NULL);

	Sprite(window, &player, &enemmis);
	sfRenderWindow_setFramerateLimit(window, 60);

	float spriteWidth = 32.0f; // sprite size
	float spriteHeight = 32.0f;


	fontScore = sfFont_createFromFile("police/arial.ttf");
	scoreText = sfText_create(); //Score text
	sfText_setFont(scoreText, fontScore);
	sfText_setCharacterSize(scoreText, 24);
	sfText_setFillColor(scoreText, sfWhite);
	sfVector2f scorePosition = { 1750, 20 };
	sfText_setPosition(scoreText, scorePosition);

	fontGO = sfFont_createFromFile("police/arial.ttf"); // GAME OVER text
	GameOverText = sfText_create();
	sfText_setFont(GameOverText, fontGO);
	sfText_setCharacterSize(GameOverText, 100);
	sfText_setColor(GameOverText, sfRed);
	sfVector2f positionGO = { 100.0f, 100.0f };
	sfText_setPosition(GameOverText, positionGO);


	float timeSinceLastShotTank = 0.0f; // time move tank
	sfClock* clock = sfClock_create();


	while (sfRenderWindow_isOpen(window)) 
	{

		if (sfKeyboard_isKeyPressed(sfKeySpace))
		{
			shootProjectile(&player, &projectile, projectileSprite);

		}

		if (player.IsAlive == 0)
		{
			GameOver(window, &player);
		}


		victoire(window, &player); // fonctions
		updateProjectile(&projectile, projectileSprite);
		UpdateAsteroidsPosition(&enemmis, &Sprites);
		spawnAndMoveTank(&tank1, videoMode);
		checkCollisions(&player, &enemmis, &projectile);
		spawnAndMoveTank(&tank1, videoMode);

		sfSprite_setPosition(tank, (sfVector2f) { tank1.currentx, tank1.currenty });

		deplacement(&player, event);
		sfVector2f position = sfSprite_getPosition(soldat);
		position.x += player.x;
		position.y += player.y;


		if (position.x > videoMode.width) { // teleportation other sides of the screen
			position.x = -spriteWidth;
		}
		else if (position.x < -spriteWidth) {
			position.x = videoMode.width;
		}

		if (position.y > videoMode.height) {
			position.y = -spriteHeight;
		}
		else if (position.y < -spriteHeight) {
			position.y = videoMode.height;
		}


		player.currentx = position.x;
		player.currenty = position.y;




		if (sfRenderWindow_pollEvent(window, &event))
		{
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(window);
		}



		sfRenderWindow_clear(window, sfBlack);  //draw all sprite
		sfRenderWindow_drawSprite(window, mapSprite, NULL);
		sfRenderWindow_drawSprite(window, soldat, NULL);
		sfRenderWindow_drawSprite(window, tank, NULL);
		sfRenderWindow_drawSprite(window, projectileSprite, NULL);
		printscore(window, &player);

		for (int i = 0; i < MAX_JEEP; ++i)
		{
			sfRenderWindow_drawSprite(window, enemmis.AsteoridsSprite[i], NULL);
		}
		sfRenderWindow_display(window);


	}

	for (int i = 0; i < MAX_JEEP; ++i)
	{
		sfSprite_destroy(enemmis.AsteoridsSprite[i]);
	}

	//destroy sprites
	sfTexture_destroy(jeepTexture);

	sfSprite_destroy(soldat);
	sfSprite_destroy(mapSprite);
	sfSprite_destroy(tank);
	sfSprite_destroy(projectileSprite);

	sfFont_destroy(fontScore);
	sfText_destroy(scoreText);
	sfFont_destroy(fontGO);
	sfText_destroy(GameOverText);


	return 0;

}


void printscore(sfRenderWindow* window, Player* player) // fonction score
{
	char scoreString[50];
	snprintf(scoreString, sizeof(scoreString), " Score: %d", player->score);
	sfText_setString(scoreText, scoreString);
	sfRenderWindow_drawText(window, scoreText, NULL);
}




void GameOver(sfRenderWindow* window, Player* player) // fonction game over
{
	if (player->IsAlive == 0)
	{
		char GOString[50];
		snprintf(GOString, sizeof(GOString), " GAME OVER");
		sfText_setString(GameOverText, GOString);


		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawText(window, GameOverText, NULL);
		sfRenderWindow_display(window);
	    sfSleep(sfMilliseconds(100));


	}

}


void victoire(sfRenderWindow* window, Player* player) // fonction victoire
{
	if (player->score >= 400)
	{
		char GOString[50];
		snprintf(GOString, sizeof(GOString), " GAME OVER");
		sfText_setString(victoryText, GOString);


		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawText(window, victoryText, NULL);
		sfRenderWindow_display(window);

		sfSleep(sfMilliseconds(100));
	}


}

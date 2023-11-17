#include "game.h"

int spriteWidth = 16.0f;
int spriteHeight = 16.0f;

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

void Sprite(sfRenderWindow* window, Player* player, Enemmis* enemmies) // fonction sprite
{

	sfTexture* SpaceShipTexture = sfTexture_createFromFile("sprites/soldat.png", NULL); // soldat
	soldat = sfSprite_create();
	sfVector2f playercord = { player->currentx, player->currenty };
	sfVector2f sOrigin = { 16.0f, 16.0f }; // Réduisez la taille de l'origine
	sfSprite_setOrigin(soldat, sOrigin);
	sfSprite_setTexture(soldat, SpaceShipTexture, sfFalse);
	sfSprite_setPosition(soldat, playercord);
	sfSprite_setScale(soldat, (sfVector2f) { 1.5, 1.5 });

	sfTexture* vehicule = sfTexture_createFromFile("sprites/mathis.png", NULL); //tank
	tank = sfSprite_create();
	sfVector2f tankPlayercord = { player->currentx, player->currenty };
	sfVector2f tankOrigin = { 16.0f, 16.0f };
	sfSprite_setOrigin(tank, tankOrigin);
	sfSprite_setTexture(tank, vehicule, sfFalse);
	sfSprite_setPosition(tank, tankPlayercord);
	sfSprite_setScale(tank, (sfVector2f) { 1.5, 1.5 });


	sfTexture* mapTexture = sfTexture_createFromFile("sprites/map.png", NULL); // map
	mapSprite = sfSprite_create();
	sfSprite_setTexture(mapSprite, mapTexture, sfFalse);
	sfSprite_setScale(mapSprite, (sfVector2f) { 1, 1 });
	sfVector2f mapcord = { 0, 0 };
	sfSprite_setPosition(mapSprite, mapcord);

	sfTexture* projectileTexture = sfTexture_createFromFile("sprites/balle.png", NULL); // projectiles
	projectileSprite = sfSprite_create();
	sfSprite_setTexture(projectileSprite, projectileTexture, sfFalse);
	sfSprite_setScale(projectileSprite, (sfVector2f) { 0.2, 0.2 });
	sfSprite_setPosition(projectileSprite, (sfVector2f) { -200.0f, -200.0f });

	jeepTexture = sfTexture_createFromFile("sprites/jeep1.png", NULL); // jeep 

	for (int i = 0; i < MAX_JEEP; ++i)
	{
		sfVector2f enemmisCord = { enemmies->X[i], enemmies->Y[i] };
		sfVector2f sOrigin1 = { 32.0f, 32.0f };
		sfSprite_setOrigin(enemmies->AsteoridsSprite[i], sOrigin1);
		sfSprite_setTexture(enemmies->AsteoridsSprite[i], jeepTexture, sfFalse);
		sfSprite_setPosition(enemmies->AsteoridsSprite[i], enemmisCord);
		sfSprite_setScale(enemmies->AsteoridsSprite[i], (sfVector2f) { 1.0, 1.0 });
	}




}

void deplacement(Player* player, sfEvent event) // movement
{
	if (player->IsAlive <= 1)
	{


		if (sfKeyboard_isKeyPressed(sfKeyZ))
		{

			if (player->speed < 5)
			{
				player->speed += 0.1;
			}


			player->currentx += player->speed * cos(player->rotation * PI / 180);
			player->currenty += player->speed * sin(player->rotation * PI / 180);

			sfSprite_setPosition(soldat, (sfVector2f) { player->currentx, player->currenty });
		}
		else
		{

			if (player->speed > 0)
			{
				player->speed -= 0.05;
			}


			player->currentx += player->speed * cos(player->rotation * PI / 180);
			player->currenty += player->speed * sin(player->rotation * PI / 180);
			sfSprite_setPosition(soldat, (sfVector2f) { player->currentx, player->currenty });
		}

		if (sfKeyboard_isKeyPressed(sfKeyD))
		{
			player->rotation += 5;


		}
		if (sfKeyboard_isKeyPressed(sfKeyQ))
		{
			player->rotation -= 5;

		}

		if (!sfKeyboard_isKeyPressed(sfKeyZ) && !sfKeyboard_isKeyPressed(sfKeyD) && !sfKeyboard_isKeyPressed(sfKeyQ))
		{
			player->speed *= player->decelerationSpeed;
		}

	}


	float minX = 500.0f;

	float maxX = 1920 - spriteWidth;



	player->currentx = fminf(maxX, fmaxf(minX, player->currentx));


	sfSprite_setPosition(soldat, (sfVector2f) { player->currentx, player->currenty });

}

void spawnAndMoveTank(Tank* tank, sfVideoMode videoMode) // movement tank 
{

	tank->currenty += tank->speed;


	if (tank->currenty < 0 || tank->currenty > videoMode.height)
	{


		tank->currentx = 150;

		tank->currenty = 0;
	}



	tank->currentx = fminf(videoMode.width - 1, fmaxf(0, tank->currentx));
	tank->currenty = fminf(videoMode.height - 1, fmaxf(0, tank->currenty));
}


void shootProjectile(Player* player, Projectile* projectile, sfSprite* projectileSprite) // fonction projectiles
{
	if (projectile->distanceProjectile <= 0)
	{
		projectile->distanceProjectile = 100;

		float projectileSpeed = 15.0f;

		// Définir la vitesse horizontale comme une valeur négative
		projectile->projectileSpeedX = -projectileSpeed;

		// Vitesse verticale nulle (tir horizontal)
		projectile->projectileSpeedY = 0.0f;

		// Position initiale du projectile en fonction de la position du joueur
		projectile->ProjectileX = player->currentx - 20.0f;
		projectile->ProjectileY = player->currenty - 1.0f;

		// Configure la position et l'échelle du sprite du projectil.e
		sfSprite_setPosition(projectileSprite, (sfVector2f) { projectile->ProjectileX, projectile->ProjectileY });

	}
}

void updateProjectile(Projectile* projectile, sfSprite* ObusSprite) // fonction update
{
	if (projectile->distanceProjectile > 0)
	{

		projectile->ProjectileX += projectile->projectileSpeedX;
		projectile->ProjectileY += projectile->projectileSpeedY;


		sfSprite_setPosition(ObusSprite, (sfVector2f) { projectile->ProjectileX, projectile->ProjectileY });
		sfSprite_setScale(projectileSprite, (sfVector2f) { 0.2, 0.2 });
		projectile->distanceProjectile -= 1.5;
	}
}

void UpdateAsteroidsPosition(Enemmis* enemmis, Sprites* Sprites) // update jeep
{

	for (int i = 0; i < MAX_JEEP; ++i)
	{

		sfVector2f asteroidPos = sfSprite_getPosition(enemmis->AsteoridsSprite[i]);
		asteroidPos.x += SPEED_JEEP * cos(enemmis->randomDirection[i] * PI / 180);
		asteroidPos.y += SPEED_JEEP * sin(enemmis->randomDirection[i] * PI / 180);

		// Vérifiez si l'astéroïde sort de l'écran et faites-le réapparaître de l'autre côté si nécessaire

		//bord droite
		if (asteroidPos.x > 1920 + 50)
		{
			asteroidPos.x = -Sprites->AsteoridsSpriteWidth;
			enemmis->randomDirection[i] += rand() % 360;
		}
		//bord gauche
		else if (asteroidPos.x < -Sprites->AsteoridsSpriteWidth - 50)
		{
			asteroidPos.x = 1080;
			enemmis->randomDirection[i] += rand() % 360;
		}
		//bord bas
		if (asteroidPos.y > 1920 + 50)
		{
			asteroidPos.y = -Sprites->AsteoridsSpriteHeight;
			enemmis->randomDirection[i] += rand() % 360;
		}
		//bord haut
		else if (asteroidPos.y < -Sprites->AsteoridsSpriteHeight - 50)
		{
			asteroidPos.y = 1080;
			enemmis->randomDirection[i] += rand() % 360;
		}
		sfSprite_setPosition(enemmis->AsteoridsSprite[i], asteroidPos);
	}
}



void checkCollisions(Player* player, Enemmis* enemmis, Projectile* projectile) // fonction collisions enter soldat and jeep
{
	sfVector2f projectilePos = sfSprite_getPosition(projectileSprite);
	sfFloatRect projectileBounds = sfSprite_getGlobalBounds(projectileSprite);

	sfVector2f playerPos = sfSprite_getPosition(soldat);
	sfFloatRect playerBounds = sfSprite_getGlobalBounds(soldat);

	for (int i = 0; i < MAX_JEEP; ++i)
	{
		sfVector2f asteroidPos = sfSprite_getPosition(enemmis->AsteoridsSprite[i]);
		float asteroidRadius = 32 * 1.5;

		if (distance(asteroidPos, projectilePos) < asteroidRadius + projectileBounds.width)
		{
			// Projectile hit the jeep
			projectile->ProjectileX = -100;
			projectile->ProjectileY = -100;
			projectile->distanceProjectile = 0;

			// Remove the jeep by setting its position outside the window
			sfSprite_setPosition(enemmis->AsteoridsSprite[i], (sfVector2f) { -1000, -1000 });

			// Assuming you have a Score variable in Player
			player->score += 200; // Add 200 points for hitting a jeep
		}

		if (distance(asteroidPos, playerPos) < asteroidRadius + playerBounds.width)
		{
			// Player hit by the jeep (or other collision logic)
			player->IsAlive = 0;
		}
	}
}

/*

	Civ_rob_2
	Copyright SAUTER Robin 2017-2019 (robin.sauter@orange.fr)
	last modification on this file on version:0.18
	file version : 1.6

	You can check for update on github.com -> https://github.com/phoenixcuriosity/Civ_rob_2.0

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "GamePlay.h"
#include "IHM.h"
#include "SaveReload.h"
#include "KeyboardMouse.h"

/* *********************************************************
 *			START GamePlay::STATIC::NEW-GAME			   *
 ********************************************************* */

 
/*
* NAME : newGame
* ROLE : Initialisation de la nouvelle partie
* INPUT  PARAMETERS : struct Sysinfo& : structure globale du programme
* OUTPUT PARAMETERS : Noms des joueurs, groundGen, positions des settlers
* RETURNED VALUE    : void
*/
void GamePlay::newGame(Sysinfo& sysinfo)
{
	IHM::logfileconsole("[INFO]___: Newgame Start");
	sysinfo.var.statescreen = STATEscreennewgame;

	unsigned int space(32);

	sysinfo.var.tempX = sysinfo.screen.screenWidth / 2;
	sysinfo.var.tempY = 164;

	// cr�ation d'une sauvegarde concernant la nouvelle partie
	SaveReload::createSave(sysinfo);

	// Fond noir
	SDL_RenderClear(sysinfo.screen.renderer);


	// Premi�re demande au joueur : Le nombre de joueurs ?
	sysinfo.allTextes.newGame["Press Return or kpad_Enter to valid selection"]->render();
	sysinfo.allTextes.newGame["How many player(s) (max 9):"]->render();
	SDL_RenderPresent(sysinfo.screen.renderer);

	sysinfo.var.cinState = cinScreenNewGameNbPlayer;
	sysinfo.var.waitEvent = true;
	// Le joueur doit rentrer une valeur entre 1 et 9, par d�faut 1
	KeyboardMouse::eventSDL(sysinfo);
	sysinfo.var.tempY += space;

	sysinfo.var.cinState = cinScreenNewGameNamePlayer;
	// Deuxi�me demande au joueur : Le nom des joueurs
	for (unsigned int i(0); i < sysinfo.var.nbPlayer; i++)
	{
		sysinfo.var.waitEvent = true;
		sysinfo.var.tempY += space;

		Texte::writeTexte(sysinfo.screen.renderer, sysinfo.allTextures.font,
			blended,"Name of player nb:" + std::to_string(i), { 255, 0, 0, 255 }, NoColor, 24, sysinfo.screen.screenWidth / 2, sysinfo.var.tempY, no_angle, center_x);
		SDL_RenderPresent(sysinfo.screen.renderer);

		sysinfo.var.tempY += space;

		// valeur par d�faut avec incr�mentation en fonction du nombre de joueur : noName 
		KeyboardMouse::eventSDL(sysinfo);

		sysinfo.var.s_player.tabPlayerName.push_back(sysinfo.var.tempPlayerName);
		sysinfo.var.tempPlayerName = "";
		sysinfo.tabplayer.push_back(new Player(sysinfo.var.s_player.tabPlayerName[i]));
		sysinfo.var.tempY += space;
	}
	
	groundGen(sysinfo);
	newGameSettlerSpawn(sysinfo);

	/*** Sauvegarde des param�tres appliqu�s et de la g�n�ration de la map ***/
	SaveReload::savemaps(sysinfo);
	SaveReload::savePlayer(sysinfo);
	
	// Cr�ation des boutons pour s�l�ctionner les joueurs
	int initspacename(200), spacename(24);
	sysinfo.var.statescreen = STATEmainmap;
	sysinfo.var.cinState = cinMainMap;
	for (unsigned int i(0); i < sysinfo.tabplayer.size(); i++)
	{
		ButtonTexte::createButtonTexte(sysinfo.screen.renderer, sysinfo.allTextures.font, sysinfo.var.statescreen, sysinfo.var.select, sysinfo.allButton.player,
			shaded, sysinfo.var.s_player.tabPlayerName[i], { 127, 255, 127, 255 }, { 64, 64, 64, 255 }, 16, 0, initspacename += spacename, nonTransparent, no_angle);
	}

	
	sysinfo.screen.enableFPS = true;
	sysinfo.screen.fpsTimer.start();

	/* ### Don't put code below here ### */

	IHM::logfileconsole("[INFO]___: Newgame End");
}

/*
* NAME : groundGen
* ROLE : G�n�ration du sol et des spec de la map
* INPUT  PARAMETERS : struct Sysinfo& : structure globale du programme
* OUTPUT PARAMETERS : G�n�ration du sol et des spec de la map
* RETURNED VALUE    : void
*/
void GamePlay::groundGen(Sysinfo& sysinfo)
{
	IHM::logfileconsole("[INFO]___: Groundgen Start");
	unsigned int randomground = 0, randomspecgrass = 0, randomspecwater = 0, randomspecwater1 = 0, randomspecwater2 = 0, randomspecwaterborder = 0;

	for (Uint8 i(0); i < sysinfo.map.mapSize / sysinfo.map.tileSize; i++)
	{
		for (Uint8 j(0); j < sysinfo.map.mapSize / sysinfo.map.tileSize; j++)
		{
			
			sysinfo.map.maps[i][j].indexX = i + (Uint8)((sysinfo.screen.screenWidth / 10) / sysinfo.map.tileSize);
			sysinfo.map.maps[i][j].indexY = j;
			sysinfo.map.maps[i][j].tile_x = sysinfo.map.tileSize * i + (sysinfo.screen.screenWidth / 10);
			sysinfo.map.maps[i][j].tile_y = sysinfo.map.tileSize * j;
			
			/*
				bord de la map
			*/
			if ((i == 0) || (i == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 1) || (j == 0) || ( j == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 1))
					tileAffectation(sysinfo.map.maps[i][j], deepwater, (std::string)"deepwater.bmp", specnothing, (std::string)"specnothing", 0, 0, 0);
			else if ((i ==  1) || (i == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 2) || (j == 1) || (j == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 2) ||
				(i == 2) || (i == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 3) || (j == 2) || (j == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 3) ||
				(i == 3) || (i == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 4) || (j == 3) || (j == (sysinfo.map.mapSize / sysinfo.map.tileSize) - 4))
			{
				
				randomspecwaterborder = rand() % 50 + 1;
				switch (randomspecwaterborder) {
				case 1:
					tileAffectation(sysinfo.map.maps[i][j], water, (std::string)"water.bmp", fish, (std::string)"fish.bmp", 3, 2, 1);
					break;
				case 2:
					tileAffectation(sysinfo.map.maps[i][j], water, (std::string)"water.bmp", petroleum, (std::string)"petroleum.bmp", 1, 3, 4);
					break;
				default:
					/* N/A */
					break;
				}
				if (randomspecwaterborder > 2)
					tileAffectation(sysinfo.map.maps[i][j], water, (std::string)"water.bmp", specnothing, (std::string)"specnothing", 1, 1, 1);
			}
			/*
				reste de la map
			*/
			else
			{
				randomground = rand() % 100 + 1;//the range 1 to 100
				if (randomground < 92)
				{
					sysinfo.map.maps[i][j].tile_ground = grass;
					sysinfo.map.maps[i][j].tile_stringground = "grass.bmp";
					randomspecgrass = rand() % 100 + 1;
					switch (randomspecgrass)
					{
					case 1:
						sysinfo.map.maps[i][j].tile_spec = coal;
						sysinfo.map.maps[i][j].tile_stringspec = "coal.png";
						break;
					case 2:
						sysinfo.map.maps[i][j].tile_spec = copper;
						sysinfo.map.maps[i][j].tile_stringspec = "copper.png";
						break;
					case 3:
						sysinfo.map.maps[i][j].tile_spec = iron;
						sysinfo.map.maps[i][j].tile_stringspec = "iron.png";
						break;
					case 4:
						sysinfo.map.maps[i][j].tile_spec = tree;
						sysinfo.map.maps[i][j].tile_stringspec = "tree1.bmp";
						break;
					case 5:
						sysinfo.map.maps[i][j].tile_spec = stone;
						sysinfo.map.maps[i][j].tile_stringspec = "stone.png";
						break;
					case 6:
						sysinfo.map.maps[i][j].tile_spec = uranium;
						sysinfo.map.maps[i][j].tile_stringspec = "uranium.png";
						break;
					case 7:
						sysinfo.map.maps[i][j].tile_spec = horse;
						sysinfo.map.maps[i][j].tile_stringspec = "horse.bmp";
						break;
					default:
						/* N/A */
						break;
					}
					if (randomspecgrass > 7 && randomspecgrass <= 32)
					{ // plus de chance d'avoir des arbres
						sysinfo.map.maps[i][j].tile_spec = tree;
						sysinfo.map.maps[i][j].tile_stringspec = "tree1.bmp";
						sysinfo.map.maps[i][j].food = 1;
						sysinfo.map.maps[i][j].work = 2;
						sysinfo.map.maps[i][j].gold = 1;
					}
					else if (randomspecgrass > 32)
					{
						sysinfo.map.maps[i][j].tile_spec = specnothing;
						sysinfo.map.maps[i][j].tile_stringspec = "specnothing";
						sysinfo.map.maps[i][j].food = 2;
						sysinfo.map.maps[i][j].work = 1;
						sysinfo.map.maps[i][j].gold = 1;
					}
					else if (randomspecgrass <= 7) 
					{
						sysinfo.map.maps[i][j].food = 1;
						sysinfo.map.maps[i][j].work = 2;
						sysinfo.map.maps[i][j].gold = 3;
					}
					else
					{
						/* N/A */
					}
				}
				else
				{
					/*
						g�n�ration de l'eau -> forme en L (3 cases)
					*/
					if (i > 2 && j > 2)
					{
						randomspecwater = rand() % 20 + 1;
						switch (randomspecwater)
						{
						case 1:
							tileAffectation(sysinfo.map.maps[i][j], water, (std::string)"water.bmp", fish, (std::string)"fish.bmp", 3, 2, 1);
							break;
						case 2:
							tileAffectation(sysinfo.map.maps[i][j], water, (std::string)"water.bmp", petroleum, (std::string)"petroleum.bmp", 1, 3, 4);
							break;
						default:
							/* N/A */
							break;
						}
						if (randomspecwater > 2)
							tileAffectation(sysinfo.map.maps[i][j], water, (std::string)"water.bmp", specnothing, (std::string)"specnothing", 1, 1, 1);
					}
					else
					{
						/* N/A */
					}

					if (sysinfo.map.maps[i - 1][j].tile_ground != deepwater)
					{
						randomspecwater1 = rand() % 10 + 1;
						switch (randomspecwater1)
						{
						case 1:
							tileAffectation(sysinfo.map.maps[i - 1][j], water, (std::string)"water.bmp", fish, (std::string)"fish.bmp", 3, 2, 1);
							break;
						case 2:
							tileAffectation(sysinfo.map.maps[i - 1][j], water, (std::string)"water.bmp", petroleum, (std::string)"petroleum.bmp", 1, 3, 4);
							break;
						default:
							/* N/A */
							break;
						}
						if (randomspecwater1 > 2)
							tileAffectation(sysinfo.map.maps[i - 1][j], water, (std::string)"water.bmp", specnothing, (std::string)"specnothing", 1, 1, 1);
					}
					else
					{
						/* N/A */
					}

					if (sysinfo.map.maps[i - 1][j - 1].tile_ground != deepwater)
					{
						randomspecwater2 = rand() % 10 + 1;
						switch (randomspecwater2)
						{
						case 1:
							tileAffectation(sysinfo.map.maps[i - 1][j - 1], water, (std::string)"water.bmp", fish, (std::string)"fish.bmp", 3, 2, 1);
							break;
						case 2:
							tileAffectation(sysinfo.map.maps[i - 1][j - 1], water, (std::string)"water.bmp", petroleum, (std::string)"petroleum.bmp", 1, 3, 4);
							break;
						default:
							/* N/A */
							break;
						}
						if (randomspecwater2 > 2)
							tileAffectation(sysinfo.map.maps[i - 1][j - 1], water, (std::string)"water.bmp", specnothing, (std::string)"specnothing", 1, 1, 1);
					}
					else
					{
						/* N/A */
					}
				}
			}
		}
	}
	IHM::logfileconsole("[INFO]___: Groundgen End");
}

/*
* NAME : tileAffectation
* ROLE : Affectation des caract�ristiques � une case
* INPUT  PARAMETERS : Tile& tile, : la case � affecter
* INPUT  PARAMETERS : Uint8 tile_ground, std::string tile_stringground,
* INPUT  PARAMETERS : Uint8 tile_spec, std::string tile_stringspec,
* INPUT  PARAMETERS : int8_t food, int8_t work, int8_t gold
* OUTPUT PARAMETERS : Affectation
* RETURNED VALUE    : void
*/
void GamePlay::tileAffectation(Tile& tile, Uint8 tile_ground, std::string tile_stringground, Uint8 tile_spec, std::string tile_stringspec, int8_t food, int8_t work, int8_t gold)
{
	tile.tile_ground = tile_ground;
	tile.tile_stringground = tile_stringground;
	tile.tile_spec = tile_spec;
	tile.tile_stringspec = tile_stringspec;
	tile.food = food;
	tile.work = work;
	tile.gold = gold;
}

/*
* NAME : newGameSettlerSpawn
* ROLE : Cr�ation des position pour les settlers de chaque joueurs
* INPUT  PARAMETERS : struct Sysinfo& : structure globale du programme
* OUTPUT PARAMETERS : position pour les settlers de chaque joueurs
* RETURNED VALUE    : void
*/
void GamePlay::newGameSettlerSpawn(Sysinfo& sysinfo)
{
	/*
		association des vecteurs de position (x,y) avec les settlers de d�part
	*/
	unsigned int selectunit(0);
	for (unsigned int p(0); p < sysinfo.var.s_player.tabUnit_Struct.size(); p++)
	{
		if (sysinfo.var.s_player.tabUnit_Struct[p].name.compare("settler") == 0)
		{
			selectunit = p;
			break;
		}
		else
		{
			/* N/A */
		}
	}

	std::vector<randomPos> tabRandom;
	for (unsigned int i(0); i < sysinfo.tabplayer.size(); i++)
	{
		try
		{
			makeRandomPosTab(sysinfo, tabRandom);
		}
		catch (const std::string& msg)
		{
			randomPos RandomPOS;
			makeRandomPos(RandomPOS, sysinfo.map.maps, sysinfo.map.toolBarSize, sysinfo.map.tileSize);
			tabRandom.push_back(RandomPOS);
			IHM::logfileconsole(msg);
			throw("[ERROR]___: [Catch]___: makeRandomPosTab, Too many Iterations : No Critical Error -> Continue");
		}
		sysinfo.tabplayer[i]->addUnit("settler", tabRandom[i].x, tabRandom[i].y,
			sysinfo.var.s_player.tabUnit_Struct[selectunit].life, sysinfo.var.s_player.tabUnit_Struct[selectunit].atq,
			sysinfo.var.s_player.tabUnit_Struct[selectunit].def, sysinfo.var.s_player.tabUnit_Struct[selectunit].movement,
			sysinfo.var.s_player.tabUnit_Struct[selectunit].level);
	}
}

/*
* NAME : makeRandomPosTab
* ROLE : Cr��r autant de vecteur de position (x,y) que de joueur initial
* INPUT  PARAMETERS : struct Sysinfo& : structure globale du programme
* INPUT  PARAMETERS : std::vector<randomPos>& : vecteurs de positions
* OUTPUT PARAMETERS : std::vector<randomPos>& : vecteurs de positions
* RETURNED VALUE    : void
*/
void GamePlay::makeRandomPosTab(Sysinfo& sysinfo, std::vector<randomPos>& tabRandom)
{
	/*
		cr��r autant de vecteur de position (x,y) que de joueur initial
	*/
	randomPos RandomPOS;
	bool continuer(true);
	unsigned int nbConditionCheck(0);
	unsigned int iteration(0);

	while (continuer)
	{
		iteration++;
		if (iteration > 10000)
		{
			throw("[ERROR]___: makeRandomPosTab, Too many Iterations");
		}
		else
		{
			/* N/A */
		}

		makeRandomPos(RandomPOS, sysinfo.map.maps, sysinfo.map.toolBarSize, sysinfo.map.tileSize);
		if (conditionground(sysinfo, RandomPOS))
		{
			if (tabRandom.size() > 1)
			{
				nbConditionCheck = 0;
				for (unsigned int i = 0; i < tabRandom.size(); i++)
				{
					if (conditionspace(RandomPOS, tabRandom, sysinfo.map.tileSize, i))
					{
						nbConditionCheck++;
						if (nbConditionCheck == tabRandom.size())
						{
							continuer = false;
							break;
						}
						else
						{
							/* N/A */
						}
					}
					else
					{
						/* N/A */
					}
				}
			}
			else
				continuer = false;
		}
		else
		{
			/* N/A */
		}
	}
	tabRandom.push_back(RandomPOS);
}

/*
* NAME : makeRandomPos
* ROLE : cr��r un vecteur de position (x,y) al�atoire respectant la taille de l'�cran
* INPUT  PARAMETERS : randomPos& RandomPOS : couple de positions
* INPUT  PARAMETERS : unsigned int toolBarSize: taille de la barre d'outil
* INPUT  PARAMETERS : unsigned int tileSize
* OUTPUT PARAMETERS : un vecteur de position
* RETURNED VALUE    : void
*/
void GamePlay::makeRandomPos(randomPos& RandomPOS, std::vector<std::vector<Tile>> maps, unsigned int toolBarSize, unsigned int tileSize)
{
	/*
		cr��r un vecteur de position (x,y) al�atoire respectant la taille de l'�cran
	*/
	int x((rand() % ((unsigned int)(maps.size() * tileSize) - (unsigned int)(toolBarSize * tileSize))) + (toolBarSize * tileSize));
	int y((rand() % (maps[0].size() * tileSize)));
	RandomPOS.x = (int)ceil(x / tileSize) * tileSize;
	RandomPOS.y = (int)ceil(y / tileSize) * tileSize;
}

/*
* NAME : conditionspace
* ROLE : condition pour valider les coordonn�es cr�es:
* ROLE : - etre en dehors d'un carr� d'influence (ici tileSize * 8) d'une autre entit�e
* INPUT  PARAMETERS : randomPos& RandomPOS : couple de positions
* INPUT  PARAMETERS : std::vector<randomPos>& : vecteurs de positions
* INPUT  PARAMETERS : unsigned int tileSize
* INPUT  PARAMETERS : couple de positions courant
* OUTPUT PARAMETERS : validation des positions
* RETURNED VALUE    : true -> condition de position valid�e / false -> non valide
*/
bool GamePlay::conditionspace(randomPos& RandomPOS, std::vector<randomPos>& tabRandom, unsigned int tileSize, unsigned int i)
{
	/*
		condition pour valider les coordonn�es cr�es:
					- etre en dehors d'un carr� d'influence (ici tileSize * 8) d'une autre entit�e
	*/
	int spaceBetweenSettler(tileSize * 16);

	if (RandomPOS.x < (tabRandom[i].x - spaceBetweenSettler)
		|| RandomPOS.x >(tabRandom[i].x + spaceBetweenSettler)
		|| RandomPOS.y < (tabRandom[i].y - spaceBetweenSettler)
		|| RandomPOS.y >(tabRandom[i].y + spaceBetweenSettler))
		return true;
	else
		return false;
}

/*
* NAME : conditionground
* ROLE : condition pour valider les coordonn�es cr�es:
* ROLE : - etre sur une tile poss�dant la caract�ristique d'etre du sol
* INPUT  PARAMETERS : struct Sysinfo& : structure globale du programme
* INPUT  PARAMETERS : std::vector<randomPos>& : vecteurs de positions
* OUTPUT PARAMETERS : validation des positions
* RETURNED VALUE    : true -> condition de position valid�e / false -> non valide
*/
bool GamePlay::conditionground(Sysinfo& sysinfo, randomPos& RandomPOS)
{
	/*
		condition pour valider les coordonn�es cr�es:
			- etre sur une tile poss�dant la caract�ristique d'etre du sol
	*/
	for (unsigned int i(0); i < sysinfo.map.maps.size(); i++) 
	{
		for (unsigned int j(0); j < sysinfo.map.maps[i].size(); j++) 
		{
			if (sysinfo.map.maps[i][j].tile_x == RandomPOS.x && sysinfo.map.maps[i][j].tile_y == RandomPOS.y) 
			{
				if (sysinfo.map.maps[i][j].tile_ground == grass)
					return true;
				return false;
			}
			else
			{
				/* N/A */
			}
		}
	}
	return false;
}

/* *********************************************************
 *			 END GamePlay::STATIC::NEW-GAME				   *
 ********************************************************* */


 
 
/* *********************************************************
 *			 START GamePlay::STATIC::NextTurn			   *
 ********************************************************* */


/*
* NAME : nextTurn
* ROLE : Action � r�aliser lors du passage � un nouveau tour 
* INPUT  PARAMETERS : struct Sysinfo& : structure globale du programme
* OUTPUT PARAMETERS : passage � un nouveau tour 
* RETURNED VALUE    : void
*/
void GamePlay::nextTurn(Sysinfo& sysinfo)
{
	for (unsigned int i(0); i < sysinfo.tabplayer.size(); i++) 
	{
		for (unsigned int j(0); j < sysinfo.tabplayer[i]->GETtabUnit().size(); j++) 
		{
			sysinfo.tabplayer[i]->GETtheUnit(j)->RESETmovement();
			sysinfo.tabplayer[i]->GETtheUnit(j)->heal(sysinfo.map.maps, i);
		}
		for (unsigned int j(0); j < sysinfo.tabplayer[i]->GETtabCity().size(); j++)
		{
			sysinfo.tabplayer[i]->GETtheCity(j)->foodNextTurn();
		}
	}
	sysinfo.var.nbturn++;
}


/* *********************************************************
 *				END GamePlay::STATIC::NextTurn			   *
 ********************************************************* */

/*
*	End Of File : GamePlay.cpp
*/

/*

	Civ_rob_2
	Copyright SAUTER Robin 2017-2020 (robin.sauter@orange.fr)
	last modification on this file on version:0.20.0.3
	file version : 1.10

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
/*

	civ_lib :
	Ce module est la librairie principale du projet civ_rob_2
	Celui-ci comprend par ordre d'apparition:
		- un appel aux diff�rentes librairies standard et celles de la SDL2.0.8
		- un appel aux diff�rentes librairies du projet civ_rob_2
		- la d�finition des constantes tr�s importantes
		- des �num�rations d'options ou sp�cifications
		- diff�rentes structures diff�renci�es par leur th�me
		- une structure englobant toutes les autres -> Sysinfo


*/

#ifndef civ_lib_H
#define civ_lib_H

/* *********************************************************
 *						Includes						   *
 ********************************************************* */

#include "LIB.h"

#include "Texture.h"
#include "LTimer.h"
#include "SaveReload.h"
#include "KeyboardMouse.h"
#include "GamePlay.h"
#include "Unit.h"
#include "City.h"
#include "Player.h"

/* *********************************************************
 *				Calcul des Constantes					   *
 ********************************************************* */

// Donne la fr�quence de rafraichissement de l'�cran en Hz
inline Uint8 getRefreshRate()
{
	DEVMODE screen;
	memset(&screen, 0, sizeof(DEVMODE));
	if (EnumDisplaySettings(NULL, 0, &screen))
	{
		return (Uint8)screen.dmDisplayFrequency;
	}
	return 0;
}
// fr�quence de rafraichissement de l'�cran en Hz
const Uint8 SCREEN_REFRESH_RATE = getRefreshRate();

/* *********************************************************
 *						Constantes						   *
 ********************************************************* */

const std::string configFilePath = "bin/config.xml";

#define NO_PLAYER_SELECTED -1
#define NO_UNIT_SELECTED -1
#define NO_CITIE_SELECTED -1

/* *********************************************************
 *						 Enum							   *
 ********************************************************* */

/* N/A */

/* *********************************************************
 *						Structures						   *
 ********************************************************* */

//---------------------- Structure niveau 2 ---------------------------------------------------------------------------------------------------------
struct SubcatPlayer
{

	// *** Index ***//

	// index de la cit� actuellement s�lectionn�e
	int selectCitie = NO_CITIE_SELECTED;

	// index du joueur actuellement s�lectionn�
	int selectplayer = NO_PLAYER_SELECTED;

	// index du joueur actuellement s�lectionn�, action : Attaquer
	int selectPlayerToAttack = NO_PLAYER_SELECTED;

	// index de l'unit� actuellement s�lectionn�e
	int selectunit = NO_UNIT_SELECTED;

	// index de l'unit� actuellement s�lectionn�e, action : Attaquer
	int selectUnitToAttack = NO_UNIT_SELECTED;

	// index de l'unit� actuellement s�lectionn�e, action : Cr�er
	unsigned int unitToCreate = 0;


	// *** Name ***//


	// nom de l'unit� � cr�er � partir du menu citieMap
	std::string toBuild;

	// nom de l'unit� � cr�er � partir du menu mainMap
	std::string unitNameToCreate;

	// nom de l'unit� � effectuer un mouvement � partir du menu mainMap
	std::string unitNameToMove;


	// *** Vector ***//

	// tableau des noms des citi� de tous les joueurs
	std::vector<std::string> tabCitieName;

	// tableau des noms des joueurs
	std::vector<std::string> tabPlayerName;

	// tableau des statistiques par d�fauts des unit�s
	std::vector<Unit_Template> tabUnit_Template;

	// nombre de joueur sans nom
	unsigned int nbNoNamePlayer = 0;

	// nombre de cit� maximal diff�rentes � cr�er 
	unsigned int citieNameMaxToCreate = 0;
};

struct CitieMap
{
	unsigned int ToolbarButtonsH = 0;
	unsigned int ToolbarButtonsW = 0;
};

//---------------------- Structure niveau 1 ---------------------------------------------------------------------------------------------------------
struct Screen
{
	// ptr sur la fenetre cr�e par la SDL
	SDL_Window *window = nullptr;

	// ptr sur le renderer cr�e par la SDL
	SDL_Renderer *renderer = nullptr;

	Uint16 screenWidth = 0;

	Uint16 screenHeight = 0;

	// autorise le d�but du compteur de fps
	bool enableFPS = false;

	LTimer fpsTimer;

	// fps moyen
	int avgFPS = 0;

	int countedFrames = 0;
};
struct File
{
	const std::string log = "bin/log/log.txt";

	std::string readme =		EMPTY_STRING;
	std::string Texte =			EMPTY_STRING;
	std::string BUILDING =		EMPTY_STRING;
	std::string CITIENAME =		EMPTY_STRING;
	std::string UNIT =			EMPTY_STRING;
	std::string SPECNAME =		EMPTY_STRING;

	std::string SaveInfo =		EMPTY_STRING;

	std::string SaveMaps =		EMPTY_STRING;
	std::string SavePlayer =	EMPTY_STRING;
};
struct Var
{

	/*** type primitif	***/

	int argc = 0;

	char** argv = nullptr;

	// variable permettant de quitter la boucle principale donc le jeu
	bool continuer = true;

	bool waitEvent = true;

	// nombre de tours pass� dans le jeu
	unsigned int nbturn = 0;

	/* Nb player */
	Uint8 nbPlayer = 0;

	std::string tempPlayerName = EMPTY_STRING;

	unsigned int tempX = 0;
	unsigned int tempY = 0;

	/*	
		�tat de la s�lection du joueur
		enum Select_Type : Uint8 
		{ 
			selectnothing,
			selectcreate,
			selectinspect,
			selectmove,
			selectmoveCitizen 
		};
	*/
	Select_Type select = Select_Type::selectnothing;

	/* 
		�tat de l'�cran du joueur
		enum State_Type : Uint8
		{ 
			STATEnothing,
			STATEtitleScreen,
			STATEscreennewgame,
			STATEreload,
			STATEmainmap,
			STATEscience,
			STATEcitiemap
		};
	*/
	State_Type statescreen = State_Type::error;

	/*
		�tat de l'entr�e clavier
		enum CinState_Type : Uint8
		{
			cinNothing,
			cinTitleScreen,
			cinScreenNewGameNbPlayer,
			cinScreenNewGameNamePlayer,
			cinMainMap,
		};
	*/
	CinState_Type cinState = CinState_Type::cinNothing;



	/*** type personnalis�	***/

	SubcatPlayer s_player;
	KeyboardMouse mouse;
	SaveReload save;
};
struct Map
{
	/*
		Attention config sp�ciale de visual studio 2017 pour d�passer 1Mo de donn�es dans un tableau
		propri�t� -> �diteur de lien -> syst�me -> taille de la r�serve de la pile -> mettre une valeur plus grande que 1Mo
	*/
	unsigned int mapSize = 0; // en pixels
	unsigned int tileSize = 0;
	unsigned int toolBarSize = 0;
	unsigned int screenOffsetXIndexMin = 0;
	unsigned int screenOffsetYIndexMin = 0;
	unsigned int screenOffsetXIndexMax = 0;
	unsigned int screenOffsetYIndexMax = 0;
	MatriceTile maps;
	CitieMap citieMap;
};
struct AllTextures
{
	TTF_Font *font[MAX_FONT];

	MapTexture ground;
	MapTexture groundSpec;
	MapTexture colorapp;
	MapTexture colorapptile;
	MapTexture barLife;

	MapTexture titleScreen;
	MapTexture unit;
	MapTexture citieMap;
};
struct AllTextes
{
	MapTexte number;

	MapTexte titleScreen;
	MapTexte newGame;
	MapTexte mainMap;
	MapTexte citieMap;
};
struct AllButtons
{
	MapButtonTexte titleScreen;
	MapButtonTexte reload;
	MapButtonTexte mainMap;
	MapButtonTexte citieMap;

	MapButtonTexte player; // init in GamePlay.cpp
};
//---------------------- Structure niveau 0 ---------------------------------------------------------------------------------------------------------
struct Sysinfo
{
	// contient les donn�es en rapport � la SDL 
	Screen screen;

	// contient les noms et le chemins des fichiers .txt 
	File file;

	// contient des variables non organis�es
	Var var;

	// contient toutes les images
	AllTextures allTextures;

	// contient tous les textes
	AllTextes allTextes;

	// contient tous les boutons
	AllButtons allButton;

	// contient les donn�es en rapport � la map
	Map map;

	// tableau de ptr sur les objets Player
	TabPlayer tabplayer;
};

#endif /* civ_lib_H */

/*
*	End Of File : civ_lib.h
*/
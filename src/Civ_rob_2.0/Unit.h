/*

	Civ_rob_2
	Copyright SAUTER Robin 2017-2020 (robin.sauter@orange.fr)
	last modification on this file on version:0.20.0.3
	file version : 1.9

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

	Units : 
	Cette classe est la repr�sentation de l'objet Unit
	Cet objet est d�crit par un nom qui sera li� par un objet de type Texture du meme nom lors de la cr�ation
	Cet objet a une position lors de la cr�ation qui correspond au coin haut gauche de la tile s�l�ctionn�e
	Les objets de type Unit ont des caract�ristiques de vie, d'attaque ...
	_blit et _show permettent de savoir lorsqu'il faut afficher l'unit� ou la faire clignoter



*/

#ifndef Unit_H
#define Unit_H

/* *********************************************************
 *						Includes						   *
 ********************************************************* */

#include "LIB.h"
#include "Texture.h"

/* *********************************************************
 *						Constantes						   *
 ********************************************************* */

/* N/A */

/* *********************************************************
 *						 Enum							   *
 ********************************************************* */

/* Define movement for the Unit in case of tryToMove */
enum class Move_Type : Uint8 
{ 
	cannotMove,		/* The Unit cannot move to the next Tile */
	canMove,		/* The Unit can move to the next Tile */
	attackMove		/* The Unit can move to the next Tile and attack the other Unit standing on the this Tile */
};

/* Define movement type of the Unit */
enum class Unit_Movement_Type : Uint8
{
	ground,			/* The Unit can move on ground (dirt,grass,...) */
	air,			/* The Unit can move on ground (dirt,grass,...) or on water */
	water,			/* The Unit can move on water */
	deepwater		/* The Unit can move on deepwater or on water */
};

/* *********************************************************
 *						 Structs						   *
 ********************************************************* */

struct Unit_Template
{

	// nom de l'unit� -> /bin/UNITNAME.txt
	std::string name;

	/*
		statistiques concernant l'unit� -> /bin/UNIT.txt
	*/
	Unit_Movement_Type type = Unit_Movement_Type::ground;
	unsigned int life = 0;
	unsigned int atq = 0;
	unsigned int def = 0;
	unsigned int movement = 0;
	unsigned int level = 0;
	unsigned int nbturnToBuild = 0;

};

/* *********************************************************
 *						 Classes						   *
 ********************************************************* */

class Unit
{
public:
	/* *********************************************************
	 *					Unit::STATIC						   *
	 ********************************************************* */


	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : searchunit																   */
	/* ROLE : S�lection du nom de l'unit� � partir de l'index du tableau				   */
	/* INPUT/OUTPUT : struct Map& : donn�es g�n�rale de la map : taille					   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static void searchUnit
	(
		SubcatPlayer&
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : searchUnitTile															   */
	/* ROLE : Cherche l'unit� �tant dans la case s�l�ctionn�							   */
	/* INPUT/OUTPUT : SubcatPlayer& s_player : structure concernant un joueur			   */
	/* INPUT : const KeyboardMouse& mouse : donn�es g�n�rale des entr�es utilisateur	   */
	/* INPUT/OUTPUT : std::vector<Player*>& tabplayer : Vecteur de joueurs				   */
	/* OUTPUT : Select_Type* select : type de s�lection									   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static bool searchUnitTile
	(
		SubcatPlayer& s_player,
		const KeyboardMouse& mouse,
		std::vector<Player*>& tabplayer,
		Select_Type* select
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : tryToMove																	   */
	/* ROLE : Recherche � faire bouger l'unit� selon le contexte						   */
	/* ROLE : Attention : contient un rappel r�cursif									   */
	/* INPUT : const std::vector<std::vector<Tile>>& : Matrice de la MAP				   */
	/* INPUT/OUTPUT : Struct SubcatPlayer& : structure concernant un joueur				   */
	/* INPUT/OUTPUT : std::vector<Player*>& : Vecteur de joueurs						   */
	/* INPUT : Select_Type : �tat de la s�lection du joueur (enum Select_Type : Uint8)	   */
	/* INPUT : int x : pos X															   */
	/* INPUT : int y : pos Y															   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static void tryToMove
	(
		const std::vector<std::vector<Tile>>& maps,
		SubcatPlayer& s_player,
		std::vector<Player*>& tabplayer,
		Select_Type select,
		int x,
		int y
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : searchToMove																   */
	/* ROLE : Recherche � faire bouger l'unit� selon le contexte						   */
	/* ROLE : Action conditionnelle (case libre, ennemi, amis)							   */
	/* INPUT : const std::vector<std::vector<Tile>>& : Matrice de la MAP			   	   */
	/* INPUT : Struct SubcatPlayer& : structure concernant un joueur					   */
	/* INPUT : const std::vector<Player*>& : Vecteur de joueurs							   */
	/* INPUT : int x : pos X														   	   */
	/* INPUT : int y : pos Y															   */
	/* RETURNED VALUE : Move_Type : / 0 : ne bouge pas / 1 : case libre : peut bouger	   */
	/* RETURNED VALUE : Move_Type : / 2 : ennemi : ne bouge pas							   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static Move_Type searchToMove
	(
		const std::vector<std::vector<Tile>>& maps,
		SubcatPlayer& s_player,
		const std::vector<Player*>& tabplayer,
		int x,
		int y
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : checkUnitNextTile															   */
	/* ROLE : Check des �qualit�s des positions des Units avec + x et +y				   */
	/* INPUT : const Unit* from : Unit avec un mouvement possible + x + y				   */
	/* INPUT : const Unit* from : Unit aux positions + x + y							   */
	/* INPUT : int x : delta horizontal tileSize en fonction du cardinal				   */
	/* INPUT : int y : delta vertical tileSize en fonction du cardinal					   */
	/* RETURNED VALUE : bool : false->position diff�rente / true->m�me positions		   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static bool checkUnitNextTile
	(
		const Unit* from,
		const Unit* to,
		int x,
		int y
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : checkNextTile																   */
	/* ROLE : Check des �qualit�s des positions des Units avec + x et +y				   */
	/* INPUT : const Unit* from : Unit aux positions + x + y							   */
	/* INPUT : const Tile& to : Tile � tester											   */
	/* INPUT : int x : delta horizontal tileSize en fonction du cardinal				   */
	/* INPUT : int y : delta vertical tileSize en fonction du cardinal					   */
	/* RETURNED VALUE : bool : false->position diff�rente / true->m�me positions		   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static bool checkNextTile
	(
		const Unit* from,
		const Tile& to,
		int x,
		int y
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : irrigate																	   */
	/* ROLE : 	TODO																	   */
	/* RETURNED VALUE : bool															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	static bool irrigate
	(
		Sysinfo&
	);

public:
	/* *********************************************************
	 *					Unit::METHODS						   *
	 ********************************************************* */

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : Unit																		   */
	/* ROLE : Constructeur par d�faut													   */
	/* INPUT : void																		   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	Unit();

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : Unit																		   */
	/* ROLE : Constructeur par complet													   */
	/* INPUT : const std::string &name : nom											   */
	/* INPUT : unsigned int x :	position en x sur la map								   */
	/* INPUT : unsigned int y : position en y sur la map								   */
	/* INPUT : unsigned int life : vie max												   */
	/* INPUT : unsigned int atq	: atq max												   */
	/* INPUT : unsigned int def	: def max												   */
	/* INPUT : unsigned int move : move max												   */
	/* INPUT : unsigned int level : level 1												   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	Unit
	(
		const std::string &name,
		unsigned int x,
		unsigned int y,
		Unit_Movement_Type movementType,
		unsigned int life,
		unsigned int atq,
		unsigned int def,
		unsigned int move,
		unsigned int level
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : ~Unit																		   */
	/* ROLE : Destructeur																   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	~Unit();

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : attack																	   */
	/* ROLE : Attaque la cible avec les dommages appliqu�s de l'unit�					   */
	/* INPUT/OUTPUT : Units* : pointeur vers l'unit� qui doit se d�fendre				   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void attack
	(
		Unit*
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : defend																	   */
	/* ROLE : D�fense de l'unit� face � une attaque										   */
	/* INPUT : unsigned int : dommage appliqu� par l'attaque							   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void defend
	(
		unsigned int dmg
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : move																		   */
	/* ROLE : Application du mouvement � l'unit�										   */
	/* ROLE : Si l'unit� n'a plus de mouvement disponible alors arret					   */
	/* INPUT : Uint8& : enum Select_Type												   */
	/* INPUT : int& : unit� s�l�ctionn�e												   */
	/* INPUT : int x : incrementation coor x											   */
	/* INPUT : int y : incrementation coor y											   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void move
	(
		Select_Type& select,
		int& selectunit,
		int x,
		int y
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : heal																		   */
	/* ROLE : Soigne l'unit� en fonction du territoire sur lequel elle se trouve		   */
	/* INPUT : const std::vector<std::vector<Tile>>& tiles : tableau de cases			   */
	/* INPUT : unsigned int : donn�es g�n�rale de la map : joueur s�lectionn�			   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void heal
	(
		const std::vector<std::vector<Tile>>& tiles,
		unsigned int selectplayer
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : levelup																	   */
	/* ROLE : Incr�mentation de 1 de level												   */
	/* INPUT : void																		   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void levelup();

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : RESETmovement																   */
	/* ROLE : Reset du nombre de mouvement disponible pour un tour						   */
	/* INPUT : void																	       */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void RESETmovement();

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : testPos																	   */
	/* ROLE : Test sur les positions de la souris et de l'unit�							   */
	/* INPUT : unsigned int mouse_x : position x										   */
	/* INPUT : unsigned int mouse_y : position y										   */
	/* RETURNED VALUE    : int : 0 : pas s�lection� / 1 : s�lectionn�					   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual bool testPos
	(
		unsigned int mouse_x,
		unsigned int mouse_y
	);


public:
	/* *********************************************************
	 *				Unit::METHODS::AFFICHAGE				   *
	 ********************************************************* */


	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : afficher																	   */
	/* ROLE : Affichage de la Texture de l'unit� ainsi que la barre de vie et couleur	   */
	/* INPUT/OUPUT : AllTextures& : Structure contenant toutes les Textures				   */
	/* INPUT : const struct Map& : donn�es g�n�rale de la map							   */
	/* INPUT : unsigned int iPlayer : joueur s�lectionn�								   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void afficher
	(
		AllTextures& allTextures,
		const Map& map,
		unsigned int iPlayer
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : afficherstat																   */
	/* ROLE : Affichage des statistiques de l'unit� (nom, x, y ...)						   */
	/* INPUT : const Map& map : donn�es de la map										   */
	/* INPUT : TTF_Font* font[] : tableau de ptr de font SDL							   */
	/* INPUT/OUTPUT : SDL_Renderer*& : ptr sur le renderer SDL							   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void afficherstat
	(
		const Map& map,
		TTF_Font* font[],
		SDL_Renderer*& renderer
	);

	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	/* NAME : cmpblit																	   */
	/* ROLE : Compteur permettant de faire clignoter l'unit�							   */
	/* ROLE : Attention : bas� sur SCREEN_REFRESH_RATE									   */
	/* INPUT : void																		   */
	/* RETURNED VALUE    : void															   */
	/* ----------------------------------------------------------------------------------- */
	/* ----------------------------------------------------------------------------------- */
	virtual void cmpblit();


public:
	/* *********************************************************
	 *				Unit::METHODS::GET/SET					   *
	 ********************************************************* */

	inline std::string GETname()				const { return _name; };
	inline unsigned int GETx()					const { return _x; };
	inline unsigned int GETy()					const { return _y; };
	inline Unit_Movement_Type GETmovementType() const { return _movementType; };
	inline unsigned int GETmaxlife()			const { return _maxlife; };
	inline unsigned int GETmaxatq()				const { return _maxatq; };
	inline unsigned int GETmaxdef()				const { return _maxdef; };
	inline unsigned int GETmaxmovement()		const { return _maxmovement; };
	inline unsigned int GETmaxlevel()			const { return _maxlevel; };
	inline unsigned int GETlife()				const { return _life; };
	inline unsigned int GETatq()				const { return _atq; };
	inline unsigned int GETdef()				const { return _def; };
	inline unsigned int GETmovement()			const { return _movement; };
	inline unsigned int GETlevel()				const { return _level; };
	inline bool GETalive()						const { return _alive; };
	inline unsigned int GETblit()				const { return _blit; };
	inline bool GETshow()						const { return _show; };
	inline bool GETshowStats()					const { return _showStats; };


	inline void SETname(const std::string &name)					{ _name = name; };
	inline void SETx(unsigned int x)								{ _x = x; };
	inline void SETy(unsigned int y)								{ _y = y; };
	inline void SETmovementType(Unit_Movement_Type movementType)	{ _movementType = movementType; };
	inline void SETlife(unsigned int life)							{ _life = life; };
	inline void SETatq(unsigned int atq)							{ _atq = atq; };
	inline void SETdef(unsigned int def)							{ _def = def; };
	inline void SETmovement(unsigned int movement)					{ _movement = movement; };
	inline void SETlevel(unsigned int level)						{ _level = level; };
	inline void SETalive(bool alive)								{ _alive = alive; };
	inline void SETblit(unsigned int blit)							{ _blit = blit; };
	inline void SETshow(bool show)									{ _show = show; };
	inline void SETshowStats(bool showStats)						{ _showStats = showStats; };
		
private:
	/* *********************************************************
	 *					Unit::ATTRIBUTS					       *
	 ********************************************************* */
	std::string _name;
	unsigned int _x;
	unsigned int _y;
	Unit_Movement_Type _movementType;

	unsigned int _maxlife;
	unsigned int _maxatq;
	unsigned int _maxdef;
	unsigned int _maxmovement;
	unsigned int _maxlevel;

	unsigned int _life;
	unsigned int _atq;
	unsigned int _def;
	unsigned int _movement;
	unsigned int _level;
	bool _alive;

	unsigned int _blit;
	bool _show;
	bool _showStats;
};



#endif /* Unit_H */

/*
*	End Of File : Unit.h
*/
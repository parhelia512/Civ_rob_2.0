/*

	Civ_rob_2
	Copyright SAUTER Robin 2017-2020 (robin.sauter@orange.fr)
	last modification on this file on version:0.16
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

#include "civ_lib.h"

enum Move_Type : Uint8 { cannotMove, canMove, attackMove};

class Unit
{
public:
	/* *********************************************************
	 *					Unit::STATIC						   *
	 ********************************************************* */

	/*
	* NAME : loadUnitAndSpec
	* ROLE : Chargement des informations concernant les unit�s � partir d'un fichier
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : tableau de nom et spec concernant les unit�s
	* RETURNED VALUE    : void
	*/
	static void loadUnitAndSpec(Sysinfo& sysinfo);

	/*
	* NAME : searchunit
	* ROLE : S�lection du nom de l'unit� � partir de l'index du tableau
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : nom de l'unit�
	* RETURNED VALUE    : void
	*/
	static void searchUnit(Sysinfo&);

	/*
	* NAME : searchUnittile
	* ROLE : Cherche l'unit� �tant dans la case s�l�ctionn�
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : index et nom de l'unit� s�lectionn�e
	* OUTPUT PARAMETERS : activation de la m�thode blit (clignotement)
	* RETURNED VALUE    : void
	*/
	static void searchUnitTile(Sysinfo&);

	/*
	* NAME : tryToMove
	* ROLE : Recherche � faire bouger l'unit� selon le contexte
	* ROLE : Attention : contient un rappel r�cursif
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : l'unit� reste � sa place ou bouge en fonction du contexte
	* RETURNED VALUE    : void
	*/
	static void tryToMove(Sysinfo&, int, int);

	/*
	* NAME : searchToMove
	* ROLE : Recherche � faire bouger l'unit� selon le contexte
	* ROLE : Action conditionnelle (case libre, ennemi, amis)
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : l'unit� reste � sa place ou bouge en fonction du contexte
	* RETURNED VALUE    : int : 0 : ne bouge pas / 1 : case libre : peut bouger / 2 : ennemi : ne bouge pas
	* 
	*/
	static Move_Type searchToMove(Sysinfo&, int, int);

	/*
	* NAME : irrigate
	* ROLE : 
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS :
	* RETURNED VALUE    : bool
	*/
	static bool irrigate(Sysinfo&);

public:
	/* *********************************************************
	 *					Unit::METHODS						   *
	 ********************************************************* */

	Unit();
	Unit(const std::string &name, unsigned int x, unsigned int y, unsigned int life, unsigned int atq,
		unsigned int def, unsigned int move, unsigned int level);
	~Unit();


	/*
	* NAME : attack
	* ROLE : Attaque la cible avec les dommages appliqu�s de l'unit� 
	* INPUT  PARAMETERS : Units* : pointeur vers l'unit� qui doit se d�fendre
	* OUTPUT PARAMETERS : Attaque d'une unit�
	* RETURNED VALUE    : void
	*/
	virtual void attack(Unit*);

	/*
	* NAME : defend
	* ROLE : D�fense de l'unit� face � une attaque
	* INPUT  PARAMETERS : unsigned int : dommage appliqu� par l'attaque
	* OUTPUT PARAMETERS : l'unit� peut prendre des dommage et/ou mourrir
	* RETURNED VALUE    : void
	*/
	virtual void defend(unsigned int dmg);

	/*
	* NAME : move
	* ROLE : Application du mouvement � l'unit�
	* ROLE : Si l'unit� n'a plus de mouvement disponible alors arret
	* INPUT  PARAMETERS : Uint8& : enum Select_Type
	* INPUT  PARAMETERS : int& : unit� s�l�ctionn�e
	* INPUT  PARAMETERS : int x : incrementation coor x
	* INPUT  PARAMETERS : int y : incrementation coor y
	* OUTPUT PARAMETERS : Application du mouvement � l'unit�
	* RETURNED VALUE    : void
	*/
	virtual void move(Uint8& select, int& selectunit, int x, int y);

	/*
	* NAME : heal
	* ROLE : Soigne l'unit� en fonction du territoire sur lequel elle se trouve
	* INPUT  PARAMETERS : std::vector<std::vector<Tile>>& tiles : tableau de cases
	* INPUT  PARAMETERS : unsigned int : donn�es g�n�rale de la map : joueur s�lectionn�
	* OUTPUT PARAMETERS : Soigne l'unit� en fonction du contexte
	* RETURNED VALUE    : void
	*/
	virtual void heal(std::vector<std::vector<Tile>>& tiles, unsigned int selectplayer);

	/*
	* NAME : levelup
	* ROLE : levelup
	* INPUT  PARAMETERS : void
	* OUTPUT PARAMETERS : levelup
	* RETURNED VALUE    : void
	*/
	virtual void levelup();

	/*
	* NAME : RESETmovement
	* ROLE : RESETmovement
	* INPUT  PARAMETERS : void
	* OUTPUT PARAMETERS : RESETmovement
	* RETURNED VALUE    : void
	*/
	virtual void RESETmovement();

	/*
	* NAME : testPos
	* ROLE : Test sur les positions de la souris et de l'unit�
	* INPUT  PARAMETERS : unsigned int mouse_x : position x
	* OUTPUT PARAMETERS : unsigned int mouse_y : position y
	* RETURNED VALUE    : int : 0 : pas s�lection� / 1 : s�lectionn�
	*/
	virtual bool testPos(unsigned int mouse_x, unsigned int mouse_y);


public:
	/* *********************************************************
	 *				Unit::METHODS::AFFICHAGE				   *
	 ********************************************************* */

	/*
	* NAME : afficher
	* ROLE : Affichage de la Texture de l'unit� ainsi que la barre de vie et couleur
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* INPUT  PARAMETERS : unsigned int iPlayer : joueur s�lectionn�
	* OUTPUT PARAMETERS : Affichage de l'unit�
	* RETURNED VALUE    : void
	*/
	virtual void afficher(Sysinfo&, unsigned int iPlayer);

	/*
	* NAME : afficherstat
	* ROLE : Affichage des statistiques de l'unit� (nom, x, y ...)
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : Affichage des statistiques de l'unit�
	* RETURNED VALUE    : void
	*/
	virtual void afficherstat(Sysinfo&);

	/*
	* NAME : cmpblit
	* ROLE : Compteur permettant de faire clignoter l'unit�
	* ROLE : Attention : bas� sur SCREEN_REFRESH_RATE
	* INPUT  PARAMETERS : struct Map& : donn�es g�n�rale de la map : taille
	* OUTPUT PARAMETERS : Compteur permettant de faire clignoter l'unit�
	* RETURNED VALUE    : void
	*/
	virtual void cmpblit();


public:
	/* *********************************************************
	 *				Unit::METHODS::GET/SET					   *
	 ********************************************************* */

	inline std::string GETname()			const { return _name; };
	inline unsigned int GETx()				const { return _x; };
	inline unsigned int GETy()				const { return _y; };
	inline unsigned int GETmaxlife()		const { return _maxlife; };
	inline unsigned int GETmaxatq()			const { return _maxatq; };
	inline unsigned int GETmaxdef()			const { return _maxdef; };
	inline unsigned int GETmaxmovement()	const { return _maxmovement; };
	inline unsigned int GETmaxlevel()		const { return _maxlevel; };
	inline unsigned int GETlife()			const { return _life; };
	inline unsigned int GETatq()			const { return _atq; };
	inline unsigned int GETdef()			const { return _def; };
	inline unsigned int GETmovement()		const { return _movement; };
	inline unsigned int GETlevel()			const { return _level; };
	inline bool GETalive()					const { return _alive; };
	inline bool GETblit()					const { return _blit; };

	inline void SETname(const std::string &name)		{ _name = name; };
	inline void SETx(unsigned int x)					{ _x = x; };
	inline void SETy(unsigned int y)					{ _y = y; };
	inline void SETlife(unsigned int life)				{ _life = life; };
	inline void SETatq(unsigned int atq)				{ _atq = atq; };
	inline void SETdef(unsigned int def)				{ _def = def; };
	inline void SETmovement(unsigned int movement)		{ _movement = movement; };
	inline void SETlevel(unsigned int level)			{ _level = level; };
	inline void SETalive(bool alive)					{ _alive = alive; };
	inline void SETblit(bool blit)						{ _blit = blit; };
		
private:
	/* *********************************************************
	 *					Unit::ATTRIBUTS					       *
	 ********************************************************* */
	std::string _name;
	unsigned int _x;
	unsigned int _y;

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
};



#endif /* Unit_H */

/*
*	End Of File : Unit.h
*/
/*

	Civ_rob_2
	Copyright SAUTER Robin 2017-2021 (robin.sauter@orange.fr)
	last modification on this file on version:0.22.0.0
	file version : 1.14

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


#ifndef Texture_H
#define Texture_H

/* *********************************************************
 *						Includes						   *
 ********************************************************* */

#include "LIB.h"

/* *********************************************************
 *						Constants						   *
 ********************************************************* */

//--- Constants related to the size of arrays ---------------------------------------------------------------------------------------------------

/* Define the max number of font */
const Uint8 MAX_FONT = 160;

/* Define a max angle to rotate Texture */
const Uint16 MAX_ANGLE = 360;

//--- Constants related to SDL  -----------------------------------------------------------------------------------------------------------------

/*
	SDL_Color name {Red, Green, Blue, Alpha (transparency)}
	Each parameter uses 8 bits -> Uint8  (from 0 to 255)
*/

const SDL_Color Black = { 0, 0, 0, 255 };
const SDL_Color White = { 255, 255, 255, 255 };
const SDL_Color Red = { 255, 0, 0, 255 };
const SDL_Color Green = { 0, 255, 0, 255 };
const SDL_Color Blue = { 0, 0, 255, 255 };
const SDL_Color Yellow = { 255, 255, 0, 255 };
const SDL_Color WriteColorButton = { 255, 64, 0, 255 }; // orange
const SDL_Color BackColorButton = { 64, 64, 64, 255 }; // gris
const SDL_Color NoColor = { 0, 0, 0, 0 };

// font use for the game
const std::string fontFile = "arial.ttf";

//--- Constants related to the screen  ----------------------------------------------------------------------------------------------------------

/* Define a pixel out of range of the screen on x */
#define SCREEN_MIN_X_OUT_OF_RANGE -1

/* Define a pixel out of range of the screen on y */
#define SCREEN_MIN_Y_OUT_OF_RANGE -1

/* *********************************************************
 *						 Enum							   *
 ********************************************************* */

// Define all State of the game, relate to screen
enum class State_Type : Uint8
{
	error,					/* ### Reserved on error detection ### */
	STATEnothing,			/* ### Reserved for fast writeTxt  ### */
	STATEtitleScreen,		/* Title screen : first screen selection */
	STATEscreenNewgame,		/* New game screen : selection of options of a new game */
	STATEreload,			/* Load screen : selection of saves to load */
	STATEmainMap,			/* Maip map screen */
	STATEscience,			/* Science screen ### Not implemented as of 0.20.0.3  ### */
	STATEcityMap			/* Citie map screen : Unit and buildings creation */
};

// Define all Selection in the game, relate to mouse click
enum class Select_Type : Uint8
{
	selectnothing,			/* Nothing is selected */
	selectcreate,			/* ### Reserved on debug as of 0.20.0.3 ### */
	selectinspect,			/* ### Reserved on debug as of 0.20.0.3 ### */
	selectmove,				/* Selection to move a Unit */
	selectmoveCitizen		/* ### Not use as of 0.20.0.3 ### */
};

//--- enum related to Texture -----------------------------------------------------------------------------------------------------------

/* Define Text type in SDL_ttf */
enum class Texte_Type : Uint8 
{
	blended,	/* No background color */
	shaded		/* With background color */
};

/* Define transparency type in SDL_ttf */
enum Transparance_Type : Uint8
{ 
	transparent = 0,			/* Total transparency */
	semiTransparent = 128,		/* Half transparency */
	nonTransparent = 255		/* No transparency  */
};

/* Define center type for Texture to print on the screen */
enum class Center_Type : Uint8
{
	nocenter,	/* Positions x and y are unchanged */
	center_x,	/* Positions y is unchanged and x is center depending on the length of the texte */
	center_y,	/* Positions x is unchanged and y is center depending on the height of the texte */
	center		/* Positions x and y are center depending on the length and the height of the texte */
};

/* 
 * Define rotation type to apply on Texture
 * The angle is in degrees (from 0 to 360)
 * ### TODO as of 0.20.0.3 : change type to double ###
 */
enum Rotation_Type : Uint16 
{
	no_angle,			/* No angle */
	inverse = 180		/* Rotate the Texture by 180 degrees */
};

/* Define index of sysinfo.AllTextes.staticIndexVectorTextes */
enum class Index_staticIndexVectorTextes : Uint8
{
	/* 0 - 50 : MainMap */

	FPS_MAIN_MAP						=	0,
	NB_TURN								=	1,

	PLAYER_GOLD							=	10,
	PLAYER_GOLD_BALANCE					=	11,
	PLAYER_INCOME						=	12,
	PLAYER_COST							=	13,
	PLAYER_TAX_INCOME					=	14,
	PLAYER_COMMERCE_INCOME				=	15,
	PLAYER_GOLD_CONVERSION_SURPLUS		=	16,
	PLAYER_ARMIES_COST					=	17,
	PLAYER_BUILDINGS_COST				=	18,

	UNIT_NAME							=	20,
	UNIT_X								=	21,
	UNIT_Y								=	22,
	UNIT_LIFE							=	23,
	UNIT_ATQ							=	24,
	UNIT_DEF							=	25,
	UNIT_MOVEMENT						=	26,
	UNIT_LEVEL							=	27,

	/* 51 - 100 : NewGame */

	NB_PLAYER							=	51,
	PLAYERS_NAME						=	52,
	PLAYER_NAME							=	53,

	/* 101 - 151 : TBD */

		/* none */

	/* 151 - 200 : TBD */

		/* none */

	/* 201 - 254 : TBD */

		/* none */

	/* ### Reserved ### */
	MAX_INDEX							=	255
};


/* *********************************************************
 *						 Classes						   *
 ********************************************************* */

  //--- Texture --------------------------------------------------------------------------------------------------------------------------------------
  /*
	  * La classe Texture permet de d'afficher des images sur la fenetre � partir de fichier image (.png, jpg, .bmp)
	  * Pour cr�er un objet Texture 2 m�thodes peuvent etre utilis�es :
	  *		1] utiliser LoadImage (recommand�)
	  *		2] directement utiliser le constructeur (pr�alable : d�ja avoir le ptr SDL_Texture)

  */
class Texture
{
public:
	/* *********************************************************
	 *					Texture::STATIC						   *
	 ********************************************************* */


	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : loadImage																			*/
	/* ROLE : Dynamic allocation of the Texture with specifications								*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* OUT : std::unordered_map<std::string, Texture*>& : map where the Texture will be stored  */
	/* IN : State_Type stateScreen : enum class State_Type										*/
	/* IN : Select_Type select : enum class Select_Type											*/
	/* IN : std::string path : Path to the picture											    */
	/* IN : std::string msg : Name of the Texture									  		    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : unsigned int w : length of the Texture (compute length of Picture if w == 0)	    */
	/* IN : unsigned int h : height of the Texture (compute height of Picture if h == 0)	    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : Uint8 cnt : enum class Center_Type											        */
	/* RETURNED VALUE    : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void loadImage
	(
		SDL_Renderer*& renderer,
		std::unordered_map<std::string, Texture*>& tabTexture,
		State_Type stateScreen,
		Select_Type select,
		std::string path,
		std::string msg,
		Transparance_Type alpha,
		int x,
		int y,
		unsigned int w,
		unsigned int h,
		Uint16 angle,
		Center_Type cnt = Center_Type::nocenter
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : assertRangeAngle																	*/
	/* ROLE : Limit the range on angle from 0.0 to 360.0										*/
	/* IN/OUT : Uint16* angle : rotation angle													*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void assertRangeAngle
	(
		Uint16* angle
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : centrage																			*/
	/* ROLE : Permet de centrer la Texture selon enum Center_Type								*/
	/* IN/OUT : int& xc : X position in pixels to center										*/
	/* IN/OUT : int& yc : Y position in pixels to center										*/
	/* IN : int iW : width of the Texture														*/
	/* IN : int iH : height of the Texture														*/
	/* IN : Uint8 cnt : enum class Center_Type ### default type : nocenter ###					*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void centrage
	(	
		int& xc,
		int& yc,
		int iW,
		int iH,
		Center_Type cnt = Center_Type::nocenter
	);

	
	
public:
	/* *********************************************************
	 *					Texture::METHODES					   *
	 ********************************************************* */

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : Texture																			*/
	/* ROLE : Default constructor by value														*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* IN : SDL_Texture* image : ptr on picture create by SDL									*/
	/* IN : std::string msg : Name of the Texture									  		    */
	/* IN : Uint8 stateScreen : enum class State_Type											*/
	/* IN : Uint8 select : enum class Select_Type												*/
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : unsigned int w : length of the Texture (compute length of Picture if w == 0)	    */
	/* IN : unsigned int h : height of the Texture (compute height of Picture if h == 0)	    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : Uint16 angle : enum Rotation_Type												    */
	/* IN : Uint8 cnt : enum class Center_Type											        */
	/* RETURNED VALUE    : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	Texture
	(	
		SDL_Renderer*& renderer,
		SDL_Texture* image,
		std::string msg,
		State_Type stateScreen,
		Select_Type select,
		unsigned int x,
		unsigned int y,
		int w,
		int h,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type center = Center_Type::nocenter
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : ~Texture																			*/
	/* ROLE : Default Destructor																*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual ~Texture();

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : rectangle																			*/
	/* ROLE : Create a SDL Rectangle with with x, y, w and h inputs dimensions					*/
	/* IN : int x : upper left corner pixel														*/
	/* IN : int y : bottom left corner pixel													*/
	/* IN : int w : width of the Texture													    */
	/* IN : int h : height of the Texture														*/
	/* RETURNED VALUE : SDL_Rect : Rectangle with x, y, w and h dimensions						*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	SDL_Rect rectangle
	(	
		int xc,
		int yc,
		int w, 
		int h
	);

	
public:
	/* *********************************************************
	 *				Texture::METHODES::Affichage			   *
	 ********************************************************* */

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : render																			*/
	/* ROLE : By default render with Texture positions											*/
	/* ROLE : If x or y are not equal to SCREEN_MIN_X_OUT_OF_RANGE or SCREEN_MIN_Y_OUT_OF_RANGE */
	/* ROLE : then change Texture positions and render											*/
	/* IN : int = SCREEN_MIN_X_OUT_OF_RANGE														*/
	/* IN : int = SCREEN_MIN_Y_OUT_OF_RANGE														*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual void render
	(	
		int = SCREEN_MIN_X_OUT_OF_RANGE,
		int = SCREEN_MIN_Y_OUT_OF_RANGE
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : renderTextureTestStates															*/
	/* ROLE : By default render with Texture positions if the input state and selection is		*/
	/* ROLE : equal to the Texture																*/
	/* IN : State_Type stateScreen : enum class State_Type										*/
	/* IN : Select_Type select : enum class Select_Type											*/
	/* IN : int = SCREEN_MIN_X_OUT_OF_RANGE	: x position to render in pixel # default value #	*/
	/* IN : int = SCREEN_MIN_Y_OUT_OF_RANGE	: y position to render in pixel # default value #	*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual bool renderTextureTestStates
	(	
		State_Type stateScreen,
		Select_Type select,
		int x = SCREEN_MIN_X_OUT_OF_RANGE,
		int y = SCREEN_MIN_Y_OUT_OF_RANGE
	);

	

public:
	/* *********************************************************
	 *				Texture::METHODES::GET/SET				   *
	 ********************************************************* */
	 
	 
	inline virtual SDL_Texture* GETtexture()const			{ return _texture; };
	inline virtual SDL_Texture* GETtextureNonConst()		{ return _texture; };
	inline virtual SDL_Rect GETdst()const					{ return _dst; };
	inline virtual SDL_Rect* GETdstPtr()					{ return &_dst; };
	inline virtual int GETdstx()const						{ return _dst.x; };
	inline virtual int GETdsty()const						{ return _dst.y; };
	inline virtual int GETdstw()const						{ return _dst.w; };
	inline virtual int GETdsth()const						{ return _dst.h; };
	inline virtual std::string GETname() const				{ return _name; };
	inline virtual State_Type GETstateScreen() const		{ return _stateScreen; };
	inline virtual Select_Type GETselect()const				{ return _select; };
	inline virtual Transparance_Type GETalpha()const		{ return _alpha; };
	inline virtual Center_Type GETcenter()const				{ return _center; };
	inline virtual Uint16 GETangle()const					{ return _angle; };

	inline virtual void SETtexture(SDL_Texture* texture)
	{
		if (_texture != texture)
		{
			if (_texture != nullptr)
			{
				SDL_DestroyTexture(_texture);
				_texture = nullptr;
			}
			_texture = texture;
		}
	};
	inline virtual void SETdstx(int x) { _dst.x = x; };
	inline virtual void SETdsty(int y) { _dst.y = y; };
	inline virtual void SETdstw(int w) { _dst.w = w; };
	inline virtual void SETdsth(int h) { _dst.h = h; };
	inline virtual void SETalpha(Transparance_Type alpha)
	{
		if (_alpha != alpha)
		{
			_alpha = alpha;
			if (SDL_SetTextureAlphaMod(_texture, _alpha) != 0)
				_alpha = (Transparance_Type)255;
		}
	}
	inline virtual void SETcenter(Center_Type cen)
	{
		if (_center != cen)
		{
			_center = cen;
			centrage(_dst.x, _dst.y, _dst.w, _dst.h, _center);
		}
	};
	inline virtual void SETangle(Uint16 angle) { _angle = angle; };

protected:
	inline SDL_Renderer *& GETrenderer() { return _renderer; };

	/* ### Not allowed to change name as of 0.21.3.1 ### */
	inline virtual void SETname(std::string msg) { _name = msg; };
	
private:
	/* *********************************************************
	 *				Texture::ATTRIBUTS						   *
	 ********************************************************* */

	// ptr sur le renderer cr�e par la SDL
	SDL_Renderer* _renderer;

	// ptr sur la SDL_Texture : image 
	SDL_Texture* _texture;

	// rectangle des positions en x et y et la longueur et hauteur
	SDL_Rect _dst;

	// nom de la Texture
	std::string _name;

	// ecran dans le quel la Texture s'affiche
	State_Type _stateScreen;

	// selection pour l'affichage
	Select_Type _select;

	// transparance de la Texture
	Transparance_Type _alpha;

	// centrage de la Texture (nocenter, center_x, center_y, center)
	Center_Type _center;

	// angle de rotation de la texture (0 = pas d'angle)
	Uint16 _angle;
};


//--- Texte ----------------------------------------------------------------------------------------------------------------------------------------

class Texte : public Texture
{


public:
	/* *********************************************************
	 *				Texte::STATIC							   *
	 ********************************************************* */

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : initializeStaticVectorTextes														*/
	/* ROLE : Clear and initialize the vector staticIndexVectorTextes					 		*/
	/* IN : void																				*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void initializeStaticVectorTextes();

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : deleteStaticVectorTextes															*/
	/* ROLE : Delete ptr in staticIndexVectorTextes then clear							 		*/
	/* IN : void																				*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void deleteStaticVectorTextes();
	 
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : createSDL_TextureFromTexte														*/
	/* ROLE : Create a pointer on an SDL_Texture compute by the inputs parameters				*/
	/* ROLE : Use to create SDL_Texture for Texte								 				*/
	/* IN: SDL_Renderer*& renderer : ptr on SDL_Renderer										*/
	/* IN : Uint8 type : enum class Texte_Type												    */
	/* IN : std::string message : Texte	to render												*/
	/* IN : SDL_Color color : Texte color														*/
	/* IN : SDL_Color colorback : background color											    */
	/* IN : TTF_Font* font : Size of the font in use										    */
	/* RETURNED VALUE : ptr on SDL_Texture														*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static SDL_Texture* createSDL_TextureFromTexte
	(	
		SDL_Renderer*& renderer,
		Texte_Type type,
		std::string message,
		SDL_Color color,
		SDL_Color colorback,
		TTF_Font* font
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : loadTexte																			*/
	/* ROLE : Dynamic allocation of the Texte with specifications								*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* IN : TTF_Font* font[] : array of ptr on font in use										*/
	/* IN : State_Type stateScreen : enum class State_Type										*/
	/* IN : Select_Type select : enum class Select_Type											*/
	/* OUT : std::unordered_map<std::string, Texte*>& : map where the Texte will be stored		*/
	/* IN : Texte_Type type : enum class Texte_Type											    */
	/* IN : std::string msg : Texte to render										  		    */
	/* IN : SDL_Color color : Texte color												  	    */
	/* IN : SDL_Color colorback : background color										  	    */
	/* IN : Uint8 size : size to render the Texte (use in TTF_Font* font[])				  	    */
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : Uint8 cnt : enum class Center_Type	### default Center_Type::nocenter ###		    */
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void loadTexte
	(	
		SDL_Renderer*& renderer,
		TTF_Font* font[],
		State_Type stateScreen,
		Select_Type select,
		std::unordered_map<std::string, Texte*>& tabTexte,
		Texte_Type type,
		std::string msg,
		SDL_Color color,
		SDL_Color backcolor,
		Uint8 size,
		int x,
		int y,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type cnt = Center_Type::nocenter
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : loadTexteStaticVector																*/
	/* ROLE : Dynamic allocation of the Texte with specifications in staticIndexVectorTextes	*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* IN : TTF_Font* font[] : array of ptr on font in use										*/
	/* IN : Index_staticIndexVectorTextes : index enum class Index_staticIndexVectorTextes		*/
	/* IN : Texte_Type type : enum class Texte_Type											    */
	/* IN : std::string msg : Texte to render										  		    */
	/* IN : SDL_Color color : Texte color												  	    */
	/* IN : SDL_Color colorback : background color										  	    */
	/* IN : Uint8 size : size to render the Texte (use in TTF_Font* font[])				  	    */
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : Uint8 cnt : enum class Center_Type	### default Center_Type::nocenter ###		    */
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void loadTexteStaticVector
	(
		SDL_Renderer*& renderer,
		TTF_Font* font[],
		Index_staticIndexVectorTextes index,
		Texte_Type type,
		std::string msg,
		SDL_Color color,
		SDL_Color backcolor,
		Uint8 size,
		int x,
		int y,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type cnt = Center_Type::nocenter
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : writeTexte																		*/
	/* ROLE : Fast load and render Texte														*/
	/* ROLE : ### Only works with enum class Index_staticIndexVectorTextes ###					*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* IN : TTF_Font* font[] : array of ptr on font in use										*/
	/* IN : Index_staticIndexVectorTextes : index enum class Index_staticIndexVectorTextes		*/
	/* IN : Texte_Type type : enum class Texte_Type											    */
	/* IN : std::string msg : Texte to render										  		    */
	/* IN : SDL_Color color : Texte color												  	    */
	/* IN : SDL_Color colorback : background color										  	    */
	/* IN : Uint8 size : size to render the Texte (use in TTF_Font* font[])				  	    */
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : Uint8 cnt : enum class Center_Type	### default Center_Type::nocenter ###		    */
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void writeTexte
	(	
		SDL_Renderer*& renderer,
		TTF_Font* font[],
		Index_staticIndexVectorTextes index,
		Texte_Type type,
		std::string msg,
		SDL_Color color,
		SDL_Color backcolor,
		Uint8 size,
		unsigned int x,
		unsigned int y,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type cnt = Center_Type::nocenter
	);



public:
	/* *********************************************************
	 *				Texte::METHODES							   *
	 ********************************************************* */


	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : Texte																				*/
	/* ROLE : Constructor by Value																*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* IN : TTF_Font* font[] : array of ptr on font in use										*/
	/* IN : SDL_Texture* : ptr on Texture of the Texte											*/
	/* IN : std::string msg : Texte to render										  		    */
	/* IN : State_Type stateScreen : enum class State_Type										*/
	/* IN : Select_Type select : enum class Select_Type											*/
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : unsigned int w : length of the Texture 											    */
	/* IN : unsigned int h : height of the Texture											    */
	/* IN : Texte_Type type : enum class Texte_Type											    */
	/* IN : SDL_Color color : Texte color												  	    */
	/* IN : SDL_Color colorback : background color										  	    */
	/* IN : Uint8 size : size to render the Texte (use in TTF_Font* font[])				  	    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : Uint8 cnt : enum class Center_Type	### default Center_Type::nocenter ###		    */
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	Texte
	(	
		SDL_Renderer*& renderer,
		TTF_Font* font[],
		SDL_Texture* image,
		std::string msg,
		State_Type stateScreen,
		Select_Type select,
		int x,
		int y,
		int w,
		int h,
		Texte_Type type,
		SDL_Color txtcolor,
		SDL_Color backcolor,
		Uint8 size,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type center = Center_Type::nocenter
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : ~Texte																			*/
	/* ROLE : Default Destructor																*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual ~Texte();

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : isSameColor																		*/
	/* ROLE : Compare 2 SDL_Color by the 4 parameters											*/
	/* IN : SDL_Color color1																	*/
	/* IN : SDL_Color color2																	*/
	/* RETURNED VALUE : bool : false -> The colors are differents								*/
	/* RETURNED VALUE : bool : true -> The colors are the same									*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual bool isSameColor
	(
		SDL_Color color1,
		SDL_Color color2
	)const;

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : resizeTexte																		*/
	/* ROLE : Resize the Texte																	*/
	/* IN : void																				*/
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual void resizeTexte();

	
public:
	/* *********************************************************
	 *				Texte::METHODES::GET/SET				   *
	 ********************************************************* */

	inline virtual Texte_Type GETtype()const				{ return _type; };
	inline virtual SDL_Color GETtxtcolor() const			{ return _txtcolor; };
	inline virtual SDL_Color GETbackcolor() const			{ return _backcolor; };
	inline virtual Uint8 GETsize()const						{ return _size; };

	virtual void SETtype(Texte_Type type);
	virtual void SETsize(Uint8 type);
	virtual void SETtxtcolor(SDL_Color txtcolor);
	virtual void SETbackcolor(SDL_Color backcolor);

protected:
	inline TTF_Font** GETfont() { return _font; };

	/* ### Not allowed to change name as of 0.21.3.1 ### */
	virtual void SETname
	(
		std::string msg
	);
	
	
private:
	/* *********************************************************
	 *					Texte::ATTRIBUTS					   *
	 ********************************************************* */
	
	
	// tableau des diff�rents polices de la font 
	TTF_Font* _font[MAX_FONT];

	/*
		* type de texte :
		*	-> blended : sans couleur de fond
		*	-> shaded : avec une couleur de fond
	*/
	Texte_Type _type;

	// couleur appliqu�e au texte
	SDL_Color _txtcolor;

	// couleur appliqu�e au sous texte
	SDL_Color _backcolor;

	// taile du texte (int 1 - 160)
	Uint8 _size;
};

//--- ButtonImage ---------------------------------------------------------------------------------------------------------------------------------------

class ButtonImage : public Texture
{

public:
	/* *********************************************************
	 *					ButtonImage::STATIC					   *
	 ********************************************************* */
	 

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : createButtonImage																	*/
	/* ROLE : Dynamic allocation of the ButtonImage with specifications							*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* OUT : std::unordered_map<std::string, ButtonImage*>& : unmap selected					*/
	/* IN : State_Type stateScreen : enum class State_Type										*/
	/* IN : Select_Type select : enum class Select_Type											*/
	/* IN : Texte_Type type : enum class Texte_Type											    */
	/* IN : std::string path : Path to the Image									  		    */
	/* IN : std::string msg : Texte to render										  		    */
	/* IN : Uint8 alpha : enum Transparance_Type										 	    */
	/* IN : Uint8 size : size to render the Texte (use in TTF_Font* font[])				  	    */
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : Uint8 cnt : enum class Center_Type	### default Center_Type::nocenter ###		    */
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	static void createButtonImage
	(	
		SDL_Renderer*& renderer,
		std::unordered_map<std::string,ButtonImage*>& tabButtonImage,
		State_Type stateScreen,
		Select_Type select,
		std::string path,
		std::string msg,
		Transparance_Type alpha,
		int x,
		int y,
		unsigned int w,
		unsigned int h,
		Uint16 angle,
		Center_Type cnt = Center_Type::nocenter
	);


public:
	/* *********************************************************
	 *					ButtonImage::METHODES				   *
	 ********************************************************* */


	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : ButtonImage																		*/
	/* ROLE : Constructor by Value																*/
	/* IN : SDL_Renderer*& renderer : Ptr on SDL_Renderer										*/
	/* IN : SDL_Texture* : ptr on Texture of the image											*/
	/* IN : std::string msg : Texte to render										  		    */
	/* IN : State_Type stateScreen : enum class State_Type										*/
	/* IN : Select_Type select : enum class Select_Type											*/
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* IN : unsigned int w : length of the Texture 											    */
	/* IN : unsigned int h : height of the Texture											    */
	/* IN : Transparance_Type alpha : enum Transparance_Type							 	    */
	/* IN : Uint16 angle : enum Rotation_Type : will compute modulo if over 360				    */
	/* IN : SDL_Texture* : ptr on Texture of the imageOn										*/
	/* IN : Center_Type cnt : enum class Center_Type	### default Center_Type::nocenter ###   */
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	ButtonImage
	(	
		SDL_Renderer*& renderer,
		SDL_Texture* image,
		const std::string& msg,
		State_Type stateScreen,
		Select_Type select,
		int x,
		int y, 
		int w, 
		int h,
		Transparance_Type alpha, 
		Uint16 angle,
		SDL_Texture* imageOn,
		Center_Type center = Center_Type::nocenter
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : ~ButtonImage																		*/
	/* ROLE : Default Destructor																*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual ~ButtonImage();

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : searchButtonImage																	*/
	/* ROLE : Search the position of ButtonImage / depends of State_Type						*/
	/* IN : State_Type stateScreen : State_Type to compare with the ButtonImage				    */
	/* IN : int x, int y : Positions before center of the Texture							    */
	/* RETURNED VALUE : bool : false -> x or/and y are not in the range	of the ButtonImage		*/
	/* RETURNED VALUE : bool : true -> x and y are in the range of the ButtonImage				*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual bool searchButtonImage
	(	
		State_Type stateScreen,
		signed int x,
		signed int y
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : renderButtonImage																	*/
	/* ROLE : Render the ButtonImage if the State_Type is identical								*/
	/* IN : State_Type stateScreen : State_Type to compare with the ButtonImage				    */
	/* RETURNED VALUE : bool : false -> ButtonImage	is not render								*/
	/* RETURNED VALUE : bool : true -> ButtonImage is render									*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual bool renderButtonImage
	(
		State_Type stateScreen
	);

	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	/* NAME : changeOn																			*/
	/* ROLE : Change bool state of _on															*/
	/* IN : void																			    */
	/* RETURNED VALUE : void																	*/
	/* ---------------------------------------------------------------------------------------- */
	/* ---------------------------------------------------------------------------------------- */
	virtual void changeOn();

	
public:
	/* *********************************************************
	 *			  ButtonImage::METHODES::GET/SET			   *
	 ********************************************************* */

	 
	inline virtual SDL_Texture* GETimageOn() const 	{return _imageOn;};
	inline virtual bool GETon() const				{return _on;};

	inline virtual void SETalpha(Transparance_Type alpha)
	{
		this->SETalpha(alpha);
		SDL_SetTextureAlphaMod(this->GETtextureNonConst(), this->GETalpha());
		SDL_SetTextureAlphaMod(_imageOn, this->GETalpha());
	};
	inline virtual void SETon(bool on) { _on = on; };
	
	


private:
	/* *********************************************************
	 *				ButtonImage::ATTRIBUTS					   *
	 ********************************************************* */

	// ptr sur la SDL_Texture : image du bouton activ�
	SDL_Texture* _imageOn;

	// bouton on/off : permet de changer la couleur du bouton
	bool _on;
};


//--- ButtonTexte ---------------------------------------------------------------------------------------------------------------------------------------
/*

	ButtonTexte :
	Cette classe est la repr�sentation d'un objet ButtonTextes qui est heriti� de la classe m�re Texte

	Un ButtonTextes est d�fini par une image et une imageOn qui sont contenu dans SDL_Texture* de la classe m�re et celle-ci
	Cet objet h�rite de tous les attributs de la classe Texture
	_on repr�sente l'�tat du bouton l'image est normal ou On

	searchButtonTexte permet de chercher le bouton en fonction de son nom ainsi que de l'ecran et de la position x,y
	renderButtonTexteTexte permet d'afficher le bouton avec l'aide de la fonction de la SDL2.0.6 SDL_RenderCopy
	resetOnstateScreen permet de reset l'image si l'on n'est plus sur la bonne s�l�ction ou sur le bon �cran
	resetOnPlayer permet de reset l'image si le joueur s�l�ctionner n'est plus le meme
	changeOn permet de changer entre l'imageOn et l'image

*/
class ButtonTexte : public Texte
{
public:
	/* *********************************************************
	 *				ButtonTexte::STATIC						   *
	 ********************************************************* */
	 

	/*
	 * NAME : createButtonTexte
	 * ROLE : cr�ation et ajout d'un objet ButtonTexte dans le tableau de ButtonTexte choisi
	 * INPUT : SDL_Renderer*& renderer : le ptr sur la variable contenant SDL_Renderer
	 * INPUT : TTF_Font* font[] : tableau de police de la font
	 * INPUT : Uint8 stateScreen, Uint8 select : les variables qui d�crivent les diff�rents �tat de l'�cran et les sp�cifications de la s�l�ction
	 * INPUT : std::vector<ButtonTexte*>& : le tableau dans lequel sera stock� la ButtonTexte (allocation dynamique)
	 * INPUT : Uint8 type : enum Texte_Type
     * INPUT : std::string msg : le nom qui permettra d'identifier la Texture dans le tableau
	 * INPUT : SDL_Color color : couleur du Texte
	 * INPUT : SDL_Color colorback : couleur du fond du Texte
	 * INPUT : Uint8 : la taille du Texte
	 * INPUT : int x, int y	: les valeurs en pixel de la future position
	 * INPUT : Uint8 alpha : la valeur de transparance de la Texture -> enum Transparance_Type
	 * INPUT : Uint16 angle : enum Uint16
	 * INPUT : Uint8 cnt : le type de centrage -> enum Center_Type
	 * OUTPUT : cr�ation et ajout d'un objet ButtonTexte
	 * RETURNED VALUE    : void
	 */
	static void createButtonTexte
	(	
		SDL_Renderer*& renderer,
		TTF_Font* font[],
		State_Type stateScreen,
		Select_Type select,
		std::unordered_map<std::string, ButtonTexte*>& tabButtonTexte,
		Texte_Type type,
		std::string msg,
		SDL_Color color,
		SDL_Color backcolor,
		Uint8 size,
		int x,
		int y,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type centerButtonTexte = Center_Type::nocenter
	);

	static void createButtonTexte
	(
		SDL_Renderer*& renderer,
		TTF_Font* font[],
		State_Type stateScreen,
		Select_Type select,
		DequeButtonTexte& tabButtonTexte,
		Texte_Type type,
		std::string msg,
		SDL_Color color,
		SDL_Color backcolor,
		Uint8 size,
		int x,
		int y,
		Transparance_Type alpha,
		Uint16 angle,
		Center_Type centerButtonTexte
	);



public:
	/* *********************************************************
	 *				ButtonTexte::METHODES					   *
	 ********************************************************* */

	ButtonTexte
	(	
		SDL_Renderer *renderer,
		TTF_Font *font[],
		SDL_Texture* image,
		std::string msg,
		State_Type stateScreen,
		Select_Type select,
		int x,
		int y,
		int w,
		int h,
		Texte_Type type,
		SDL_Color txtcolor,
		SDL_Color backcolor,
		Uint8 size,
		Transparance_Type alpha,
		Uint16 angle,
		SDL_Texture* imageOn,
		Center_Type center = Center_Type::nocenter
	);


	virtual ~ButtonTexte();


	/*
	 * NAME : searchButtonTexte
	 * ROLE : Recherche le bouton aux coordonn�es en entr�e
	 * INPUT  PARAMETERS : Uint8 stateScreen :  enum State_Type
	 * INPUT  PARAMETERS : signed int x, signed int y : coordonn�es en entr�e
	 * OUTPUT PARAMETERS : Validation ou non
	 * RETURNED VALUE    : bool : false = non valide / true = valide
	 */
	virtual bool searchButtonTexte
	(	
		State_Type stateScreen,
		signed int x,
		signed int y
	);


	/*
	 * NAME : resetOnstateScreen
	 * ROLE : Reset l'�tat _on des boutons
	 * INPUT  PARAMETERS : Uint8 select, unsigned int selectnothing : enum State_Type
	 * OUTPUT PARAMETERS : Reset l'�tat _on des boutons
	 * RETURNED VALUE    : void
	 */
	virtual void resetOnstateScreen
	(	
		Select_Type select
	);


	/*
	 * NAME : resetOnPlayer
	 * ROLE : Reset l'�tat _on des boutons
	 * INPUT  PARAMETERS : unsigned int : joueur s�lectionn�
	 * INPUT  PARAMETERS : std::vector<std::string> : tableau de nom des joueurs
	 * OUTPUT PARAMETERS : Initialisation de map.screen et map.maps
	 * RETURNED VALUE    : void
	 */
	virtual void resetOnPlayer
	(	
		unsigned int selectplayer,
		std::vector<std::string> tabPlayerName
	);


	/*
	 * NAME : renderButtonTexte
	 * ROLE : Affichage du ButtonTexte en fonction du contexte
	 * INPUT  PARAMETERS : Uint8 stateScreen :  enum State_Type
	 * INPUT  PARAMETERS : int x = -1, int y = -1 : positions optionnelles
	 * INPUT  PARAMETERS : Uint8 center = nocenter : enum Center_Type
	 * OUTPUT PARAMETERS : Initialisation de map.screen et map.maps
	 * RETURNED VALUE    : bool : false = non afficher / true = affichage
	 */
	virtual bool renderButtonTexte
	(
		State_Type stateScreen,
		int x = SCREEN_MIN_X_OUT_OF_RANGE,
		int y = SCREEN_MIN_Y_OUT_OF_RANGE,
		Center_Type center = Center_Type::nocenter
	);


	/*
	 * NAME : changeOn
	 * ROLE : alterne l'attribut bool�en _on
	 * INPUT  PARAMETERS : void
	 * OUTPUT PARAMETERS : alterne l'attribut bool�en _on
	 * RETURNED VALUE    : void
	 */
	virtual void changeOn();



public:
	/* *********************************************************
	 *			ButtonTexte::METHODES::GET/SET				   *
	 ********************************************************* */


	virtual SDL_Texture* GETimageOn() const {return _imageOn;};
	virtual bool GETon() const 				{return _on;};

	virtual void SETalpha(Transparance_Type alpha)
	{
		this->SETalpha(alpha);
		SDL_SetTextureAlphaMod(this->GETtextureNonConst(), this->GETalpha());
		SDL_SetTextureAlphaMod(_imageOn, this->GETalpha());
	};
	virtual void SETon(bool on) { _on = on;};
	



private:
	/* *********************************************************
	 *				ButtonTexte::ATTRIBUTS					   *
	 ********************************************************* */

	// ptr sur la SDL_Texture : image du bouton activ�
	SDL_Texture* _imageOn;

	// bouton on/off : permet de changer la couleur du bouton
	bool _on;
};
#endif /* Texture_H */

/*
*	End Of File : Texture.h
*/
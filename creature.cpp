/************************************************************************************
** Program Filename: creature.cpp
** Author: Nathan J Mize
** Date: 6/8/2014
** Description: A basic survival game, collect enough food for winter
** Required files:
**			SurvivalGame.cpp
**			hunter.cpp
**			world.cpp
**			creature.cpp
**			resources.h
**			makefile
** Input: user selections from integer list and [y/n] options
** Output: Survival Game.
************************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
using std::cin;
using std::cout;
using std::string;
using std::endl;

#include "resources.h"

namespace SurvivalGame
{
	// this class represents the animals encountered by the player
	
// constructors

/*********************************REQUIREMENT 14*************************************/
/*********************************REQUIREMENT 33*************************************/
	creature::creature()
	{
		health = 1;
		evasion = 1;
		food_value = 1;
		name = "placeholder";
	}

	creature::creature(int new_health, int new_evasion, int new_food, string new_name)
	{
		health = new_health;
		evasion = new_evasion;
		food_value = new_food;
		name = new_name;
	}

// mutators	 
	void creature::set_health(int new_health)
	{
		health = new_health;
	}

	void creature::set_evasion(int new_evasion)
	{
		evasion = new_evasion;
	}

	void creature::set_food(int new_food)
	{
		food_value = new_food;
	}

	void creature::set_name(string new_name)
	{
		name = new_name;
	}

	void creature::lose_health(int damage)
	{
		health -= damage;
	}

// accessors
	int creature::get_health()
	{
		return health;
	}

	int creature::get_evasion()
	{
		return evasion;
	}

	int creature::get_food()
	{
		return food_value;
	}

	string creature::get_name()
	{
		return name;
	}

	int creature::get_damage()
	{
		return 0;
	}

	// returns true if player strength > creature health
	bool creature::auto_kill(int player_str)
	{
		if (player_str >= health)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	// returns true if player can find animal
	bool creature::caught()
	{
		int pursuit = rand() % 10;
		if (evasion <= pursuit)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// subclass of creature
	hostile::hostile()
	{
		health = 1;
		evasion = 1;
		food_value = 1;
		name = "placeholder";
		stalking = 1;
	}

	hostile::hostile(int new_health, 
					int new_evasion, 
					int new_food, 
					string new_name,
					int new_stalking)
	{
		creature(new_health, new_evasion, new_food, new_name);
		stalking = new_stalking;
	}

	bool hostile::player_stalked()
	{
		int pursuit = rand() % 10;
		if (stalking <= pursuit)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int hostile::get_damage()
	{
		int damage = 1 + rand() % 10;
		cout << "The " << name << " attacks you for " << damage << " damage." << endl << endl;
		return damage;
	}

	// subclass of creature, if aggressive = true will fight if atacked if agressive = false will flee
	peaceful::peaceful()
	{
		health = 1;
		evasion = 9;
		food_value = 1;
		name = "mouse";
		aggressive = false;
	}

	peaceful::peaceful(int new_health, 
					   int new_evasion, 
					   int new_food, 
					   string new_name, 
					   bool is_aggressive)
	{
		creature(new_health, new_evasion, new_food, new_name);
		aggressive = is_aggressive;
	}

	bool peaceful::is_aggressive()
	{
		return aggressive;
	}

	int peaceful::get_damage()
	{
		cout << "The cornered "<< name << " cowers in fear" << endl << endl;
		return 0;
	}
// hostile creatures, might attack first, will not run
	wolf::wolf()
	{
		health = 60;
		evasion = 6;
		food_value = 40;
		name = "wolf";
		stalking = 4;
	}

	int wolf::get_damage()
	{
		int damage = 1 + rand() % 20;
		cout << "The " << name << " claws and bites you for " << damage << " damage." << endl << endl;
		return damage;
	}
	
	bear::bear()
	{
		health = 300;
		evasion = 3;
		food_value = 400;
		name = "bear";
		stalking = 2;
	}

	int bear::get_damage()
	{
		int damage = 20 + rand() % 20;
		cout << "The "<< name << " mauls you for " << damage << " damage." << endl << endl;
		return damage;
	}

	wolverine::wolverine()
	{
		health = 20;
		evasion = 5;
		food_value = 30;
		name = "wolverine";
		stalking = 8;
	}

	int wolverine::get_damage()
	{
		int damage = 16 + rand() % 5;
		cout << "The "<< name << " hits you like a tornado for " << damage << " damage." << endl << endl;
		return damage;
	}

	rattle_snake::rattle_snake()
	{
		health = 10;
		evasion = 3;
		food_value = 20;
		name = "rattle snake";
		stalking = 8;
	}

	int rattle_snake::get_damage()
	{
		int damage = 19 + rand() % 2;
		cout << "The " << name << " hits you like a tornado of teeth and claws, doing " << damage << " damage." << endl << endl;
		return damage;
	}

// aggressive peaceful animals, won't attack first but will not run	
	bull_moose::bull_moose()
	{
		health = 200;
		evasion = 3;
		food_value = 300;
		name = "bull moose";
		aggressive = true;
	}

	int bull_moose::get_damage()
	{
		int damage = 10 + rand() % 30;
		cout << "The " << name << " head butts you for " << damage << " damage" << endl <<endl;
		return damage;
	}

	llama::llama()
	{
		health = 80;
		evasion = 3;
		food_value = 100;
		name = "llama";
		aggressive = true;
	}

	int llama::get_damage()
	{
		int damage = 10 + rand() % 30;
		cout << "The " << name << " spits in your eye and kicks you, doing " << damage << " damage" << endl <<endl;
		return damage;
	}

	ram::ram()
	{
		health = 80;
		evasion = 4;
		food_value = 100;
		name = "ram";
		aggressive = true;
	}

	int ram::get_damage()
	{
		int damage = 10 + rand() % 10;
		cout << "The " << name << " knocks you down, doing " << damage << " damage" << endl <<endl;
		return damage;
	}

// non-aggressive peaceful animals, tries to flee, will not fight back
	doe::doe()
	{
		health = 50;
		evasion = 4;
		food_value = 50;
		name = "doe";
		aggressive = false;
	}

	rabbit::rabbit()
	{
		health = 20;
		evasion = 8;
		food_value = 20;
		name = "rabbit";
		aggressive = false;
	}

	sheep::sheep()
	{
		health = 50;
		evasion = 2;
		food_value = 50;
		name = "sheep";
		aggressive = false;
	}
	
	quail::quail()
	{
		health = 10;
		evasion = 9;
		food_value = 10;
		name = "quail";
		aggressive = false;
	}
	
	sloth::sloth()
	{
		health = 30;
		evasion = 0;
		food_value = 30;
		name = "sloth";
		aggressive = false;
	}
	
} // end of namespace








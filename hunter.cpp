/************************************************************************************
** Program Filename: hunter.cpp
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
#include <vector>
#include <fstream>
/*********************************REQUIREMENT 32*************************************/
#include <algorithm>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::vector;

#include "resources.h"



namespace SurvivalGame
{
	// constructor
	hunter::hunter()
	{
		latitude = 3;		// latitude and longitude 3,3 are the players starting position
		longitude = 3;		
		food = 20;			// a little food to allow player to travel or fail in hunting/gathering
		strength = 10;
		weapon = "hands";  
		health = 100;
		inventory.push_back(pack());  	// put a new struct pack in inventory then fill it
		inventory[0].food_gained = 20;
		inventory[0].source = "Starting Food";
	}
	
	// decreases latitude by one, moving hunter up
	void hunter::north()
	{
		longitude --;
		// loop prevents hunter from being moved outside game world
		if (longitude < 1)
		{
			cout << "You can't go that way!" << endl;
			longitude = 1;
		}
	}

	// increases latitude by one, moving hunter down
	void hunter::south()
	{
		longitude ++;
		// loop prevents hunter from being moved outside game world
		if (longitude > 5)
		{
			cout << "You can't go that way!" << endl;
			longitude = 5;
		}
	}

	// increases longitude by one, moving hunter right
	void hunter::east()
	{
		latitude ++;
		// loop prevents hunter from being moved outside game world
		if (latitude > 5)
		{
			cout << "You can't go that way!" << endl;
			latitude = 5;
		}
	}

	// decreases longitude by one, moving hunter left
	void hunter::west()
	{
		latitude --;
		// loop prevents hunter from being moved outside game world
		if (latitude < 1)
		{
			cout << "You can't go that way!" << endl;
			latitude = 1;
		}
	}

	// increase food 
	void hunter::add_food(int more_food)
	{
		food += more_food;
	}
	
	// decrease food
	void hunter::lose_food(int less_food)
	{
		food -= less_food;
	}

	// set food to new value
	void hunter::set_food(int new_food)
	{
		food = new_food;
	}
	
	// decrease health
	void hunter::lose_health(int less_health)
	{
		health -= less_health;
	}

	// set health to new value
	void hunter::set_health(int new_health)
	{
		health = new_health;
	}
	
	// new weapon with a new strength
	void hunter::set_strength(int new_strength, string new_weapon)
	{
		strength = new_strength;
		weapon = new_weapon;
	}

	// add a new entry to inventory
	void hunter::add_inventory(int food, string from)
	{
		inventory.push_back(pack());		// put a new struct pack in inventory then fill it
		int i = inventory.size()-1;
		inventory[i].food_gained = food;
		inventory[i].source = from;
	}

	// outputs inventory to screen
	void hunter::print_inventory()
	{
		int food_gathered = 0;	// food gathered so far, totaled as inventory is output
		int food_consumed;		// food player has used
		double efficiency;		// how much of the food gathered has been consumed
/*********************************REQUIREMENT 32*************************************/
		// sorts inventory so that smallest food values are printed first
		sort(inventory.begin(), inventory.end());
		
		// print player's weapon
		cout << "Your current weapon is " << weapon << "." << endl << endl;
		
		// print food gains and total food collected
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			cout << inventory[i].food_gained << " food from " << inventory[i].source << endl;
			food_gathered += inventory[i].food_gained;
		}
		
/*********************************requirement 3***************************************/		
		// explicit type cast lets us get the ratio precisely from two integers
		// otherwise it would be zero 
		food_consumed = food_gathered - food;
		efficiency = double ( food_consumed ) / food_gathered;
/*************************************************************************************/		
		// output food totals
		cout << endl << "You have eaten " << food_consumed << " food, leaving you with " 
			 << food << " food for the upcoming winter." << endl << endl;
		
		// set output to two digits after the decimal
		cout.setf(std::ios::fixed);
		cout.setf(std::ios::showpoint);
		cout.precision(2);
		
		// output ratio of food consumed as percentage of food collected
		cout << "You have consumed " << efficiency*100 
			 << "% of what you have gathered" << endl << endl;
		
	}
/*********************************REQUIREMENT 31*************************************/
	// outputs inventory to file
	void hunter::file_inventory(string output)
	{
		int food_gathered = 0; // food gathered so far, totaled as inventory is output
		int food_consumed;	   // food player has used
		double efficiency;	   // how much of the food gathered has been consumed
		
		// sorts inventory so that smallest food values are printed first
		sort(inventory.begin(), inventory.end());
		
		// set output stream and open file
		std::ofstream ofs;
		ofs.open(output.c_str());
		
		// print player's weapon
		ofs << "Your current weapon is " << weapon << "." << endl << endl;
		
		// print food gains and total food collected
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			ofs << inventory[i].food_gained << " food from " << inventory[i].source << endl;
			food_gathered += inventory[i].food_gained;
		}
		
		food_consumed = food_gathered - food; // shows how much has been consumed
		 
		// explicit type cast lets us get the ratio precisely from two integers
		// otherwise it would be zero 
		efficiency = double ( food_consumed ) / food_gathered;
		
		// output food totals
		ofs << endl << "You have eaten " << food_consumed << " food, leaving you with " 
			 << food << " food for the upcoming winter." << endl << endl;
		
		// set output to two digits after the decimal
		ofs.setf(std::ios::fixed);
		ofs.setf(std::ios::showpoint);
		ofs.precision(2);
		
		// output ratio of food consumed as percentage of food collected
		ofs << "You have consumed " << efficiency*100 
			 << "% of what you have gathered" << endl << endl;
	}
	
	// returns food
	int hunter::get_food()
	{
		return food;
	}

	// returns health
	int hunter::get_health()
	{
		return health;
	}

	// returns strength
	int hunter::get_strength()
	{
		return strength;
	}

	// returns weapon name
	string hunter::get_weapon()
	{
		return weapon;
	}

	// returns latitude
	int hunter::lat()
	{
		return latitude;
	}

	// returns latitude
	int hunter::lon()
	{
		return longitude;
	}
/****************************requirement 5***************************************/	
	// returns true if the player has died (0 health or food)
	bool hunter::dead()
	{
		if ((health <= 0) || (food <= 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


}
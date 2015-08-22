/************************************************************************************
** Program Filename: SurvivalGame.cpp
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
#include <cctype>
#include <ctime>
#include <climits>
#include <cstring>

using namespace std;

#include "resources.h"
using namespace SurvivalGame;

class game
{
	public:
		game();								// default constructor
		game(int new_food_target);			// constructor, takes new food_target
		void gameplay();					// starts the game
		
	private:
		void intro();						// displays introduction to user
		void display();						// displays the main menu screen, with world map
		void clear_screen();				// clears screen
		void direction(string ordinal);		// outputs what is in the direction entered for display()
		void selection();					// gets user input and calls appropriate action
		void go_north();					// moves player north
		void go_east();						// moves player east
		void go_south();					// moves player south
		void go_west();						// moves player west
		void gather();						// randomly determines outcome of foraging for food and weapons
		void find_food(int food_value, string food_type);		// adds the food and inventories it
		void find_weapon(int new_strength, string new_weapon);	// adds new weapon to player if stronger than current weapon
		void pause();						// ask user to press enter to continue
		void hunt();						// determines if player finds tracks
		void track();						// determines what sort of creature player encounters
		void fight_hostile(hostile *quarry);	// start a fight for creature sub class hostile
		void fight_peaceful(peaceful *quarry);  // start a fight for creature sub class peaceful
		void fight(creature *quarry);			// resolve a fight for creature
		bool pursue();							// asks user if they will pursue creature tracked
		void dress_kill(creature *quarry);		// give user food and update inventory after creature defeated
		void check_pack();						// display inventory, option to output to file
		void hermit();							// go to the crazy hermit for healing
		int fibonacci(int n);					// recursively determine nth Fibonacci number
		void end_game();						// determine and display if player won or lost
		bool is_integer();						// false if input is not an integer
		bool in_range(int val, int min, int max); // false if inputout of range
		int get_integer(int min, int max);		// gets integer from user
		
		// member variables
		hunter player;			// the player
		world map;				// the world map
		int food_target;		// how much food needed to win
};

// input verification for argv
bool is_valid(char **argv);
/*********************************REQUIREMENT 19*************************************/
// takes single command line argument of an integer
int main(int argc, char **argv)
{
	srand(time(NULL));
	int victory = 500;
	
	if (argc == 2)
	{
		if (is_valid(argv))
		{
			victory = atoi(argv[1]);
		}
	}

	game game1(victory);
	game1.gameplay();
	
}

/******************************requirement 9*****************************/
/******************************requirement 10****************************/
// input verification for argv being a positive integer
bool is_valid(char **argv)
{
	for (unsigned int i = 0; i < strlen(argv[1]) ; i++)
	{
		if (!isdigit(argv[1][i]))
			return false;
	}
	return true;
}

// default constructor
game::game()
{
	// hunter player, and world map are handled by their default constructors
	food_target = 500;
}

// constructor, takes new food_target
game::game(int new_food_target)
{
	// hunter player, and world map are handled by their default constructors
	food_target = new_food_target;
}

// starts and controls the game structure
void game::gameplay()
{
	intro();
	// do loop for turns, no time limit loop ends when player has 0 food, 0 health, or enough food
	do
	{
		display();
		selection();
	}while ( (!player.dead()) || (player.get_food() >= food_target ) );
	end_game();
}

// displays introduction to user
void game::intro()
{
	clear_screen();
/****************************requirement 1******************************************/
	cout << "You have become lost in a crater with no hope of" << endl      
		 << "rescue until next spring. It is up to you to " << endl
		 << "collect enough food to survive until then." << endl << endl
		 << "Good Luck...    you'll need it." << endl << endl;
/***********************************************************************************/
	pause();
}

// displays the main menu screen, with world map
void game::display()
{
	
	if ((player.dead()) || (player.get_food() == food_target))
	{
		return; 	// skip everything else if player has already won or lost
	}
	
	// present world map
	clear_screen();
	cout << "                       MAP" << endl;
	map.print(player.lat(), player.lon());
	
	// display current player stats
	cout << "Food collected:  " << player.get_food() << " (" << food_target 
		 << " food needed for winter)"  << endl; 
	cout << "Attack Power:    " << player.get_strength() << " (" << player.get_weapon() << ")" << endl;
	cout << "Health Points:   " << player.get_health() << "/100" << endl << endl;
	
	// display current location
	cout << "You are currently in the " << map.get_name(player.lat(), player.lon()) << "." << endl;
	
	// display options
	cout << "1. ";
	direction("north");
	cout << "2. ";
	direction("east");
	cout << "3. ";
	direction("south");
	cout << "4. ";
	direction("west");
	cout << "5. Forage for food and tools (costs 2 food)" << endl;
	cout << "6. Hunt for food (costs 2 food)" << endl;
	cout << "7. Check your pack" << endl;
	cout << "8. Visit the old hermit" << endl;
	cout << "0. Give up and die (exit game)" << endl;
	cout << endl;
	cout << "your choice? ";
	
}

/*************************REQUIREMENT 11*******************************/
// clears screen
void game::clear_screen()
{
	for (int i = 0; i < 50; i++)
		cout << endl;
}

// outputs what is in the direction entered for display()
void game::direction(string ordinal)
{
	string terrain;
	int cost;
	if (ordinal == "north")
	{
		terrain = map.get_north(player.lat(), player.lon());
		cost = map.get_cost(player.lat(), player.lon()-1);
	}
	else if (ordinal == "east")
	{
		terrain = map.get_east(player.lat(), player.lon());
		cost = map.get_cost(player.lat()+1, player.lon());
	}
	else if (ordinal == "south")
	{
		terrain = map.get_south(player.lat(), player.lon());
		cost = map.get_cost(player.lat(), player.lon()+1);
	}
	else if (ordinal == "west")
	{
		terrain = map.get_west(player.lat(), player.lon());
		cost = map.get_cost(player.lat()-1, player.lon());
	}
	if (terrain == "impassable")
	{
		cout << "The " << ordinal << " is blocked by impassable cliffs" << endl;
	}
	else
	{
		cout << "Go " << ordinal <<" to the " << terrain 
			 << " (costs " << cost << " food)" << endl;
	}
}

// gets user input and calls appropriate action
void game::selection()
{
	if ((player.dead()) || (player.get_food() == food_target))
	{
		return; // skip everything else if player has already won or lost
	}
	int pick = get_integer(0,8); //gets integer with verification
	cin.clear();
	switch (pick)
	{
		case 1:
			go_north();
			break;
		case 2:
			go_east();
			break;
		case 3:
			go_south();
			break;
		case 4:
			go_west();
			break;
		case 5:
			gather();
			break;
		case 6:
			hunt();
			break;
		case 7:
			check_pack();
			break;
		case 8:
			hermit();
			break;
		default:					// set to default rather than 0 to catch any bad
			player.set_food(0);		//  input that gets past get_integer
			return;
	}
}

// moves player north
void game::go_north()
{
	player.north();
	map.set_visited(player.lat(), player.lon(), true);
	if (map.get_north(player.lat(), player.lon()) != "impassable")
	{
		player.lose_food(map.get_cost( player.lat(), player.lon() ) );
	}
}

// moves player east
void game::go_east()
{
	player.east();
	map.set_visited(player.lat(), player.lon(), true);
	if (map.get_east(player.lat(), player.lon()) != "impassable")
	{
		player.lose_food(map.get_cost( player.lat(), player.lon() ) );
	}
	return;
}

// moves player south
void game::go_south()
{
	player.south();
	map.set_visited(player.lat(), player.lon(), true);
	if (map.get_south(player.lat(), player.lon()) != "impassable")
	{
		player.lose_food(map.get_cost( player.lat(), player.lon() ) );
	}
}

// moves player west
void game::go_west()
{
	player.west();
	map.set_visited(player.lat(), player.lon(), true);
	if (map.get_west(player.lat(), player.lon()) != "impassable")
	{
		player.lose_food(map.get_cost( player.lat(), player.lon() ) );
	}
}

// randomly determines outcome of foraging for food and weapons
void game::gather()
{
	char repeat;
	repeat = 'n';
	do // do loop to allow player to keep foraging with out going back to menu
	{
		clear_screen();
		int found;
		found = rand() % 40;
		switch (found)
		{
			case 0:
			case 1:
				find_food(10, "apples");
				break;
			case 2:
			case 3:
				find_food(8, "squash");
				break;
			case 4:
				find_food(30, "tacos");
				cout << "Yes, really, wild tacos growing on a taco tree, look it up." << endl << endl;
				break;
			case 5:
			case 6:
				find_food(4, "a Watermelon");
				break;
			case 7:
			case 8:
				find_food(5, "nuts");
				break;
			case 9:
			case 10:
				find_food(3, "strawberries");
				break;
			case 11:
			case 12:
				find_food(3, "strawberries");
				break;
			case 13:
			case 14:
				find_food(3, "wild berries");
				break;
			case 15:
			case 16:
				find_food(5, "snails");
				break;
			case 17:
			case 18:
				find_food(20, "crayfish");
				break;
			case 19:
			case 20:
				find_food(10, "grapes");
				break;
			case 21:
				find_food(100, "old survival rations");
				cout << "This is a real life saver, but then again, the man who" << endl
					 << "was carrying them didn't fair so well..." << endl << endl;
				break;
			case 22:
			case 23:
				find_food(15, "bacon");
				cout << "'Bacon?!?' bacon never needs a reason" << endl << endl;
				break;
			case 24:
				cout << "Oops! those berries were poison. Your stomach is full" << endl
					 << "but you lose 10 health" << endl << endl;
				player.lose_health(10);
				break;
			case 25:
			case 26:
				find_weapon(20, "heavy rock");
				break;
			case 27:
			case 28:
				find_weapon(30, "pointy stick");
				break;
			case 29:
				find_weapon(35, "knife");
				break;
			case 30:
				find_weapon(40, "bow");
				break;
			case 31:
				find_weapon(50, "rusty sword");
				break;
			case 32:
				find_weapon(60, "pistol");
				break;	
			case 33:
				find_weapon(100, "rifle");
				break;
			case 34:
				find_weapon(500, "bazooka");
				break;
			default:
				player.lose_food(2);
				cout << "you found nothing, but ate 2 food while searching" << endl << endl;

		}
		pause();
		clear_screen();
		if ((player.dead()) || (player.get_food() == food_target))
		{
			return;
		}
		cout << endl << "You have " << player.get_food() << " food and " 
		     << player.get_health() << " health" << endl << endl
			 << "Would you like to forage some more? [y/n] ";
		cin >> repeat;
	}while ((repeat == 'y') || (repeat == 'Y'));
	
}

// adds the food and inventories it
void game::find_food(int food_value, string food_type)
{
	player.add_food(food_value-2);
	cout << "You find " << food_type << " worth " << food_value
		 << " food, but ate 2 food while searching" << endl << endl;
	player.add_inventory(food_value, food_type);
}

// adds new weapon to player if stronger than current weapon
void game::find_weapon(int new_strength, string new_weapon)
{
	cout << "While foraging, you found a " << new_weapon; 
	if (player.get_strength() < new_strength)
	{
		cout << "." << endl << "It will work better than your " << player.get_weapon() 
		<< ". So, you pick it up" << endl << endl;
		player.set_strength(new_strength, new_weapon);
	}
	else
	{
		cout << "." << endl << "Too bad your " << player.get_weapon() << " is better." << endl << endl;
	}
	cout << "while foraging you ate 2 food." << endl << endl;
}

// ask user to press enter to continue
void game::pause()
{
	string pause;
	cout << "press ENTER to continue";
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	getline(cin, pause);
	return;
}

// determines if player finds tracks
void game::hunt()
{
	char repeat;
	repeat = 'n';
	do		// do loop to allow player to keep hunting with out going back to menu
	{
		clear_screen();
		int found;
		found = 1 + rand() % 4; // 75% chance of finding tracks
		if (found < 4)  
		{
			track(); // determine what animal player is tracking
		}
		else
		{
			player.lose_food(2);
			cout << "you found nothing, but ate 2 food while searching" << endl << endl;
			pause();
		}

		if ((player.dead()) || (player.get_food() == food_target))
		{
			return; // skip everything else if player has already won or lost the game
		}
		
		cout << endl << "You have " << player.get_food() << " food and " << player.get_health() << " health" << endl << endl
			 << "Would you like to hunt some more? [y/n] ";
		cin >> repeat;
	}while ((repeat == 'y') || (repeat == 'Y'));
	return;
}

// determines what sort of creature player encounters
void game::track()
{
	int type = 1+ rand() % 100;
/*******************************requirement 4**********************************/	
	// creatures passed by pointer to avoid slicing problem
	if (type > 95)	// wolf
	{
		wolf beast;
		hostile *quarry;
		quarry = &beast;
		fight_hostile(quarry);
	}
	
	else if (type > 90)	// bear		
	{
		bear beast;
		hostile *quarry;
		quarry = &beast;
		fight_hostile(quarry);
	}
	
	else if (type > 85) // wolverine
	{
		wolverine beast;
		hostile *quarry;
		quarry = &beast;
		fight_hostile(quarry);
	}
	
	else if (type > 80) // rattle snake
	{
		rattle_snake beast;
		hostile *quarry;
		quarry = &beast;
		fight_hostile(quarry);
	}
	
	else if (type > 70) // bull moose
	{
		bull_moose beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else if (type > 60) // llama
	{
		llama beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else if (type > 50) // ram
	{
		ram beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else if (type > 40) // doe
	{
		doe beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else if (type > 30) // rabbit
	{
		rabbit beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else if (type > 20) // sheep
	{
		sheep beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else if (type > 10) // quail
	{
		quail beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
	
	else				// sloth
	{
		sloth beast;
		peaceful *quarry;
		quarry = &beast;
		fight_peaceful(quarry);
	}
}

/*********************************REQUIREMENT 24*************************************/
// start a fight for creature sub class hostile
void game::fight_hostile(hostile *quarry)
{
	cout << "You come across the tracks of a " << quarry->get_name() << endl << endl;
	if (pursue()) // determine if player pursues the creature
	{
		if (quarry->player_stalked()) // determine if creature attacks player first
		{
			cout << "The " << quarry->get_name() << " was stalking you!" << endl << endl; 
			player.lose_health(quarry->get_damage());
			if (player.get_health() <= 0)
			{
				return;
			}
		}
		else if (quarry->caught()) // determine if player finds creature to attack
		{
			cout << "You find the " << quarry->get_name() << " and attack." << endl << endl;
		}
		else // if creature does not attack and remains unfound, end the fight
		{
			cout << "You are unable to catch the " << quarry->get_name() 
			 << ". The hunt took 2 food." << endl << endl;
			player.lose_food(2);
			return;
		}
		
		pause();
		clear_screen();
		fight(quarry); 
	}
	else
	{
		cout << "You give up on pursuing the " << quarry->get_name() 
				 << ". The hunt took 2 food.";
		player.lose_food(2);
		pause();
	}
	return;
}

// start a fight for creature sub class peaceful
void game::fight_peaceful(peaceful *quarry)
{
	cout << "You come across the tracks of a " << quarry->get_name() << endl << endl;
	do
	{
		if (pursue()) // determine if player pursues the creature
		{
			if (quarry->caught()) // determine if player finds creature to attack
			{
				if (quarry->is_aggressive()) // aggressive creatures will fight back until dead
				{
					fight(quarry);
					return;
				}
				
				// non aggressive creatures attempt to run if not killed outright
				cout << "You catch the " << quarry->get_name() 
						<< " and attack with your " << player.get_weapon() << endl << endl;
				
				if (quarry->auto_kill(player.get_strength())) 
				{
					dress_kill(quarry);
					return;
				}
				else
				{
					quarry->lose_health(player.get_strength());
					cout << "The " << quarry->get_name() << " is wounded, but flees";
					if (quarry->get_name() == "sloth")
					{
						cout << endl << " very, very, slowly";
					}
				}
			}
			else
			{
				cout << "You are unable to catch the " << quarry->get_name() 
				 << ". The hunt took 2 food." << endl << endl;
				player.lose_food(2);
				return;
			}
		}
		else
		{
			cout << "You give up on pursuing the " << quarry->get_name() 
				 << ". The hunt took 2 food." << endl << endl;
				 player.lose_food(2);
				 return;
		}
	} while (true);
}

// resolve a fight for creature
void game::fight(creature *quarry)
{
	do
	{
		cout << "You attack the " << quarry->get_name() 
			 << " with your " << player.get_weapon() << endl << endl;
		quarry->lose_health(player.get_strength());		// player attacks
		pause();

		if (quarry->get_health() <= 0) // end fight if quarry dead
		{
			dress_kill(quarry);
			pause();
			clear_screen();
			return;
		}
		player.lose_health(quarry->get_damage());		// quarry attacks
		cout << endl << endl;
		if (player.get_health() <= 0) // end fight if player dead
		{
			return;
		}
		pause();

	}while (true);
}

// asks user if they will pursue creature tracked
bool game::pursue()
{
	char pursuit;
	cout << " do you pursue? [y/n] ";
	cin >> pursuit;
	cout << endl;
	if ((pursuit == 'y') || (pursuit == 'Y'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// give user food and update inventory after creature defeated
void game::dress_kill(creature *quarry)
{
	player.add_food(quarry->get_food());
	cout << "The " << quarry->get_name() << " dies and yields " << quarry->get_food() 
		 << " food." << endl << endl;
	player.add_inventory(quarry->get_food(),quarry->get_name());
}

// display inventory, option to output to file
void game::check_pack()
{
	char print;
	string file_name;
	clear_screen();
	player.print_inventory();
	cout << "would you like to output these details to a file? [y/n] ";
	cin >> print;
	cin.clear();
	if ((print == 'Y') || (print == 'y'))
	{
		cout << endl << "file name please: ";
		cin >> file_name;
		player.file_inventory(file_name.c_str());
		cout << endl << "output to file complete" << endl << endl;
		pause();
	}

}

// go to the crazy hermit for healing
void game::hermit()
{
	int guess;
	string ordinal;
	int n = 1 + rand() % 12;
	if (n == 1)				// conditional prints ordinal number correctly, 1st, 2nd etc
	{
		ordinal = "st";
	}
	else if (n == 2)
	{
		ordinal = "nd";
	}
	else if (n == 3)
	{
		ordinal = "rd";
	}
	else
	{
		ordinal = "th";
	}
	clear_screen();
	cout << "A crazy old hermit will heal your injuries" << endl
		 << "if you can tell him what the " << n << ordinal << " number" << endl
		 << "in the Fibonacci sequence is." << endl << endl
		 << "So, what is it? ";
	cin >> guess;
	clear_screen();
	if (guess == fibonacci(n))
	{
		cout << "you were right, the old hermit also makes you play" << endl
		     << "a few rounds of pin the tail on the donkey, but" << endl
			 << "he heals you as promised." << endl << endl;
		player.set_health(100);
	}
	else
	{
		cout << "'You're wrong!' the hermit screams and whacks you over" << endl
			 << "the head with his cane, costing you two health, 'The" << endl 
			 << "answer is " << fibonacci(n) << ".' he yells as you run away" << endl << endl;
		player.lose_health(2);
	}
	pause();
}


/*********************************REQUIREMENT 16*************************************/
// recursively determine nth Fibonacci number
int game::fibonacci(int n)
{
	if (n <= 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else
	{
		return (fibonacci(n-1) + fibonacci(n-2));
	}
}

// determine and display if player won or lost
void game::end_game()
{
	clear_screen();
	if (player.get_food() >= food_target)
	{
		cout << "You have collected enough food to last through the winter! You win" << endl << endl;
	}
	else if (player.get_food() <= 0)
	{
		cout << "You have died of starvation. You lose" << endl << endl;
	}
	else if (player.get_health() <= 0)
	{
		cout << "You have died of your injuries. You lose" << endl << endl;
	}
}

// return true if most recent input is an integer
bool game::is_integer()
{
	if((!cin) || (cin.peek() != '\n'))		
	{
		cin.clear(); 						
		cin.ignore(INT_MAX, '\n');  
		return false;
	}
	else
	{
		return true;
	}
}
	
// verify most recent input is in a given range
bool game::in_range(int val, int min, int max)
{
	return (min <= val && max >= val);
}

// gets integer from user, with verification
/******************************requirement 2*****************************/
int game::get_integer(int min, int max)
{
	int integer;
	do
	{
		cin >> integer;
		if ((!is_integer()) || (!in_range(integer,min,max)))
			cout << "not an option, try again: ";
	} while ((!is_integer()) || (!in_range(integer,min,max)));
	return integer;
}
/**************************************************************************/




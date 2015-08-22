/************************************************************************************
** Program Filename: resources.h
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

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::vector;

/*********************************REQUIREMENT 25*************************************/
namespace SurvivalGame
{	
/*********************************REQUIREMENT 16*************************************/
// This struct is used to store enteries in the vector inventory
	struct pack
	{
		int food_gained; // the amount of food player added
		string source;   // were the food came from

/*********************************REQUIREMENT 30*************************************/		
		// this overloaded operator allows the structs to be sorted by 
		// the size of food_gained when they are in a vector
		bool operator < (const pack& object) const
		{
			return (food_gained < object.food_gained);
		}	
	};

// This is the player and their statistics for the Survival Game
	class hunter
	{
		public:
			// constructor
			hunter();
			
			// mutators
			void north(); 		// decreases latitude by one, moving hunter up
			void south(); 		// increases latitude by one, moving hunter down
			void east();  		// increases longitude by one, moving hunter right
			void west(); 		// decreases longitude by one, moving hunter left
			
			void add_food(int more_food);    	// increase food 
			void lose_food(int less_food);		// decrease food
			void set_food(int new_food);		// set food to new value
			
			void lose_health(int less_health);	// decrease health
			void set_health(int new_health);	// set health to new value
			
			void set_strength(int new_strength, string new_weapon);		// new weapon with a new strength
			
			void add_inventory(int food, string from); 	// add a new entry to inventory	
			
			// accessors
			void print_inventory();				// outputs inventory to screen
			void file_inventory(string output); // outputs inventory to file
			
			int get_food();			// returns food
			int get_health();		// returns health
			int get_strength();		// returns strength
			string get_weapon();	// returns weapon name
			int lat();				// returns latitude
			int lon();				// returns latitude
			bool dead();			// returns true if the player has died (0 health or food)
			
		private:
			int latitude;			// player's position on vertical axis
			int longitude;			// player's position on horizontal axis
			int food;				// how much food the player has
			int health;				// how much health is left
			int strength;			// how much damage the player does when hunting/combat
/********************************REQUIREMENT 15***********************************************/
			string weapon;			// weapon the player is holding
/*********************************REQUIREMENT 28*************************************/
			vector<pack> inventory; // where player has gotten their food
			
	};

// This struct describes each set of coordinates in the class world
	struct location
	{
		string name;	// name of the territory, describes the terrain
		int food_cost;	// how much food it takes to enter the location
		bool visited;	// true if player has been to this location
	};
	
// this class describes the game world
	class world
	{
		public:
			world();						// constructor
			world(const world& old_world);	// copy constructor
			~world();						// deconstructor
			
			// mutators alter details of location at coordinates lat,lon
			void set_name(int lat, int lon, string new_name);
			void set_cost(int lat, int lon, int new_cost);
			void set_visited(int lat, int lon, bool been_here);
			
			// accessors return members of location at coordinates lat,lon
			string get_name(int lat, int lon);
			int get_cost(int lat, int lon);
			bool been_there(int lat, int lon);
			
			string get_north(int lat, int lon);  // return name of location just north of lat,lon
			string get_south(int lat, int lon);  // return name of location just south of lat,lon
			string get_east(int lat, int lon);	 // return name of location just east of lat,lon	
			string get_west(int lat, int lon);	 // return name of location just west of lat,lon
			
			void print(int hunter_lat, int hunter_lon);		
				// prints the map to screen showing position of the hunter
			
			
		private:
			location **map; // pointer to an array of pointers to locations 
							// forming the map of the game world
							// dynamically declared
			
			void check_coord(int lat, int lon);  // function throws an exception exiting the 
												 // program if lat,lon is outside the array
			
	};

// this class represents the animals encountered by the player
	class creature
	{
		public:
			// constructors
			creature();
			creature(int new_health, 
					 int new_evasion, 
					 int new_food, 
					 string new_name);
					 
			// mutators	 
			void set_health(int new_health);
			void set_evasion(int new_evasion);
			void set_food(int new_food);
			void set_name(string new_name);
			void lose_health(int damage);
			
			// accessors
			int get_health();
			int get_evasion();
			int get_food();
			string get_name();
			virtual int get_damage();		// virtual function insures proper version is called
			
			// other
			bool auto_kill(int player_str); // returns true if player strength > creature health
			bool caught();					// returns true if player can find animal
			
		protected:
			int health;						// animal hit points
			int evasion;					// 0-10 chance of player catching animal, 
											// 0: always caught  10+: never caught
			int food_value;					// player will gain this much food by killing beast
			string name;					// name of the creature
	};

	class hostile : public creature //may attack if pursued, will attack if injured
	{
		public:
			hostile();
			hostile(int new_health, 
					int new_evasion, 
					int food_value, 
					string new_name,
					int new_stalking);		
			bool player_stalked();			// returns true if animal is stalking player
			virtual int get_damage();			
		protected:
			int stalking;					// 0-10 chance of player being stalked animal, 
											// 0: never stalked  10+: always stalked
	};

	class peaceful : public creature // will not attack unless attacked
	{
		public:
			peaceful();
			peaceful(int new_health,
					 int new_evasion,
					 int new_food, 
					 string new_name,
					 bool is_aggressive);
			bool is_aggressive();
			virtual int get_damage();
		protected:
			bool aggressive;
	};

// hostile creatures, might attack first, will not run
	class wolf : public hostile 
	{
		public:
			wolf();
			virtual int get_damage();
	};
	
	class bear : public hostile
	{
		public:
			bear();
			virtual int get_damage();
	};
	
	class wolverine : public hostile
	{
		public:
			wolverine();
			virtual int get_damage();
	};

	class rattle_snake : public hostile
	{
		public:
			rattle_snake();
			virtual int get_damage();
	};
	
// aggressive peaceful animals, won't attack first but will not run
	class bull_moose : public peaceful
	{
		public:
			bull_moose();
			virtual int get_damage();
	};

	class llama : public peaceful
	{
		public:
			llama();
			virtual int get_damage();
	};
	
	class ram : public peaceful
	{
		public:
			ram();
			virtual int get_damage();
	};
// non-aggressive peaceful animals, tries to flee, will not fight back
	class doe : public peaceful
	{
		public:
			doe();
	};
	
	class rabbit : public peaceful
	{
		public:
			rabbit();
	};
	
	class sheep : public peaceful
	{
		public:
			sheep();
	};
	
	class quail : public peaceful
	{
		public:
			quail();
	};
	
	class sloth : public peaceful
	{
		public:
			sloth();
	};
}
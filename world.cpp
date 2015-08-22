/************************************************************************************
** Program Filename: world.cpp
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
#include <ctime>

using std::cin;
using std::cout;
using std::string;
using std::endl;

#include "resources.h"
namespace SurvivalGame
{
/*********************************REQUIREMENT 29*************************************/
	// constructor
	world::world()
	{
/*********************************REQUIREMENT 17*************************************/
/*********************************REQUIREMENT 18*************************************/
/*********************************REQUIREMENT 22*************************************/
/*********************************REQUIREMENT 23*************************************/
		// dynamically declare the array
		map = new location*[7];
		for (int i = 0; i < 7; i++)
		{
			map[i] = new location[7]; 
		}
		
		int pick;
		srand(time(NULL));
		for (int lon = 0; lon < 7; lon++)
		{
			for (int lat = 0; lat < 7; lat++)
			{
				// the edges of the array are "impassable" this creates a boundary for 
				// class game to recognize
				if ((lat == 0) || (lon == 0) || (lat == 6) || (lon == 6))
				{
					map[lat][lon].name = "impassable";
					map[lat][lon].food_cost = 0;
					map[lat][lon].visited = false;
				}
				else
				// each location in the inner 5x5 array has one of four teritory types
				// these currently do not affect game play beyond differences in food cost
				// but could later be used to control what sort of creatures are encountered
				// when hunting or things found when foraging.
				{
/*****************************requirement 7****************************************/  
					pick = rand() % 4;   
					switch (pick)
					{
						// ***Important note: 
							// the names of locations currently need to be 6 characters 
							// long to work with the print function.
						case 0:
							map[lat][lon].name = "forest";
							map[lat][lon].food_cost = 2;
							map[lat][lon].visited = false;
							break;
						case 1:
							map[lat][lon].name = "hills "; 
							map[lat][lon].food_cost = 2;
							map[lat][lon].visited = false;
							break;
						case 2:
							map[lat][lon].name = "plains";
							map[lat][lon].food_cost = 1;
							map[lat][lon].visited = false;
							break;
						case 3:
							map[lat][lon].name = "desert";
							map[lat][lon].food_cost = 3;
							map[lat][lon].visited = false;
							break;
					}
				}
			}
		}
		map[3][3].visited = true; //this is the player start point
	}
	
	// copy constructor needed since array is declared dynamically
	
/************************REQUIREMENT 13******************************************/
	world::world(const world& old_world)
	{
		map = new location*[7];
		for (int i = 0; i < 7; i++)
		{
			map[i] = new location[7]; 
		}
		for (int lon = 0; lon < 7; lon++)
		{
			for (int lat = 0; lat < 7; lat++)
			{
				map[lat][lon] = old_world.map[lat][lon];
			}
		}
	}
	
	// deconstructor needed since array is declared dynamically
	world::~world()
	{
		for (int i = 0; i < 7; i++)
		{
			delete [] map[i];
		}
		delete [] map;
	}
	
	// alter name of location at coordinates lat,lon
	void world::set_name(int lat, int lon, string new_name)
	{
		check_coord(lat, lon);
		map[lat][lon].name = new_name;
	}

	// alter food_cost of location at coordinates lat,lon
	void world::set_cost(int lat, int lon, int new_cost)
	{
		check_coord(lat, lon);
		map[lat][lon].food_cost = new_cost;
	}

	// alter visited of location at coordinates lat,lon
	void world::set_visited(int lat, int lon, bool been_here)
	{
		check_coord(lat, lon);
		map[lat][lon].visited = been_here;
	}
	
	// return name of location at coordinates lat,lon	
	string world::get_name(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat][lon].name;
	}

	// return food_cost of location at coordinates lat,lon
	int world::get_cost(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat][lon].food_cost;
	}

	// return true if player has been to location at coordinates lat,lon
	bool world::been_there(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat][lon].visited;
	}


	// return name of location just north of lat,lon
	string world::get_north(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat][lon-1].name;
	}

	// return name of location just south of lat,lon
	string world::get_south(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat][lon+1].name;
	}

	// return name of location just east of lat,lon
	string world::get_east(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat+1][lon].name;
	}

	// return name of location just west of lat,lon
	string world::get_west(int lat, int lon)
	{
		check_coord(lat, lon);
		return map[lat-1][lon].name;
	}

	// prints the map to screen showing position of the hunter
	void world::print(int hunter_lat, int hunter_lon)
	{
		string row[5];
		for (int lon = 1; lon < 6; lon++)
		{
			for (int lat = 1; lat < 6; lat++)
			{
				if (  (map[lat][lon].visited) || 		// this conditional ensures that if a location
						(map[lat-1][lon].visited) || 	// has been visited or if it is adjacent to one
						(map[lat+1][lon].visited) || 	// that has, it is printede on screen in the map 
						(map[lat][lon+1].visited) || 
						(map[lat][lon-1].visited) )
				{
					row[lat-1] = map[lat][lon].name;
				}
				else  
				{
					row[lat-1] = " ???? ";  // player doesn't know what is at this location
				}
				
				// the hunter is output at his position
				if ((hunter_lat == lat) && (hunter_lon == lon))
				{
					row[lat-1] = "HUNTER";
				}
			}
			
			cout << " --------  --------  --------  --------  -------- " << endl
				 << "|        ||        ||        ||        ||        |" << endl
				 << "| " << row[0] << " || " << row[1] << " || " << row[2] << " || " 
				 << row[3] << " || " << row[4] << " |" << endl
				 << "|        ||        ||        ||        ||        |" << endl
				 << " --------  --------  --------  --------  -------- " << endl;
		}
	}
/*********************************REQUIREMENT 35*************************************/	
	// function throws an exception exiting the program if lat,lon is outside the array
	void world::check_coord(int lat, int lon)
	{
		try
		{
			if ((lat < 0) || (lat > 7) || (lon < 0) || (lon > 7))
			{
				throw 1;
			}
		}
		catch (int a)
		{
			cout << "This location is beyond the edge of the map! " << lat << "," << lon << endl
				 << "Terminating program..." << endl << endl;
			exit(1);
		}
	}
}
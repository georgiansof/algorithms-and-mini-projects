#ifndef __FUNCTION_DECL_H
#define __FUNCTION_DECL_H

#include <stdbool.h>
#include "globals.h"

/**
 * @brief determines fastest trip path between two stations
 * @param starting_station a string representing the departure station
 * @param arriving_station a string representing the arriving station
 * @param stations the list of stations
*/
void determine_fastest_path(char *starting_station, char *arriving_station, struct stations_list *stations);

/**
 * @brief determines cheapest trip between two stations - cost and path, prints it to stdout
*/

void determine_cheapest_path(char *starting_station, char *arriving_station, struct stations_list *stations);

/**
 * @brief truncates and lowercases the string for comparation compatibility inside the program
 * @param str the string to be modified
*/
void normalize_string(char *str);

/**
 * @brief checks if given departure station exists
 * @param station a string representing the station name
 * @param stations all stations list
*/
bool station_exists_departure(char *station,struct stations_list *stations);

/**
 * @brief checks if a given arrival station exists in relation with any departure station
 * @param station the arrival station to be searched
 * @param stations the list of stations
*/
bool arrival_station_exists(char *station, struct stations_list *stations);

/**
 * @brief checks if there is a path from a station to another
 * @param station starting station
 * @param stations all stations list
 * @param arriving station
*/
bool arrival_station_exists_departure_specific(char *station, struct stations_list *stations, char *arriving_station);

/**
 * @brief checks if a given station exists at all in the database
 * @param station given station
 * @param stations list of all stations
*/
bool station_exists_any(char *station, struct stations_list *stations);

/**
 * @brief parse the file containing station linkings, the file must be in the correct format! (See function header comments for format style)
 * @param file_name the name of the file to be parsed
 * @param stations table of station name indexing
 * @param nr_stations the number of stations registered already
 * 
 * 
 * @relatedalso
 * commented lines are marked using // as in C
 * 
 * there can only exist one route from station A to station B ( only first values for required time and cost will be recorded ).
 * 
 * every line must be formatted as: "station1" "station2" travel_time cost
 * where station1, station2 are read as whitespace normalized strings, travel_time and cost are read as integers.
 * 
 * lines are allowed to have multiple whitespaces, but cannot exceed character buffer limit ( 2 * STATION_NAME_MAX + 10 + 10 + 10). 
 * i.e (size of two integers up to 20 characters and 15 characters for quotes, spaces, endline and null).
 * be careful to extra spaces at the end of the line (those also count).
*/
void parse_station_file(char* file_name,struct stations_list **stations,int *nr_stations);

/**
 * @brief add a station
 * @param station station to be added
 * @param stations list of stations with their lines
 * @param nr_stations pointer to the number of stations
*/
struct stations_list* add_station(char *station, struct stations_list **stations, int *nr_stations);

/**
 * @brief returns pointer to the given station. Returns nullptr if not found
 * @param station seeked station
 * @param stations the linked list of stations
 * @param nr_stations the total number of stations
*/
struct stations_list* station_seek(char *station, struct stations_list *stations);



/**
 * @brief uses hsearch to map the stations to numbers 
*/
int map_stations(char **map, struct stations_list *stations);


/**
 * @brief adds a line 
 * @param stations linked list of stations
 * @param station1 name of the departing station
 * @param station2 name of the arriving station
 * @param travel_time time needed to travel
 * @param cost money needed to travel
*/
void add_line(struct stations_list **stations, int *nr_stations, char *station1, char *station2, int travel_time, float cost);

/**
 * @brief seeks for a destination in a station's lines
 * @param station the destination to be found
 * @param destinations the list of destinations of station ``station``
*/
struct station_link* destination_seek(char *station, struct station_link *destinations);

/**
 * @brief adds a destination in a station's lines
 * @param station the destination to be added
 * @param travel_time time needed to travel
 * @param cost money needed to travel
*/
struct station_link* add_destination(char *station, struct station_link **destinations, int travel_time, float cost);

void print_path(int parent, int *parentsvec, char **map);

#endif
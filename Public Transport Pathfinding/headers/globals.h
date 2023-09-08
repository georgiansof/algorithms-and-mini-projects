#ifndef __GLOBALS_H
#define __GLOBALS_H

#define STATION_NAME_MAX 50 /// maximum length of a station name
#define MAX_STATIONS 500
#define PARSE_BUFFER_MAX 2*STATION_NAME_MAX + 10 + 10 + 15
#define NOT_FOUND -1
#define STRING_MATCH 0
#define nullptr NULL

struct station_link {
    char arriving_station[STATION_NAME_MAX+1];
    int travel_time; /// in minutes
    float cost;
    struct station_link *next;
};

struct stations_list {
    char starting_station[STATION_NAME_MAX+1];
    struct station_link *destinations;
    struct stations_list *next;
};

const char currency[]="lei";

#endif
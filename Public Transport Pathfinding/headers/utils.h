#ifndef __UTILS_H
#define __UTILS_H

#include "function_decl.h"
#include <stdbool.h>
#include <ctype.h>
#include "globals.h"

void normalize_string(char *str) {
    char separators[]=" ._\"\n";
    char *p = strtok(str,separators);

    char aux[STATION_NAME_MAX] = "";
    while(p) {
        strcat(aux, p);
        strcat(aux," ");
        p=strtok(0,separators);
    }
    aux[(int)strlen(aux)-1]='\0';
    for(int i=0;aux[i];++i)
        if(i==0 || aux[i-1]==' ') aux[i]=toupper(aux[i]);
        else aux[i]=tolower(aux[i]);
         
    strcpy(str,aux);
}

void parse_station_file(char* file_name,struct stations_list **stations,int *nr_stations) { 
    FILE *fin;
    fin=fopen(file_name,"r");
    if(!fin) {
        fprintf(stderr,"Couldn't open stations file! File existance or requirement of elevated permissions to access the file could be at fault.\n");
        exit(EXIT_FAILURE);
    }
    
    char buff[PARSE_BUFFER_MAX]="",separators[]=" \n,;";
    while(fgets(buff,PARSE_BUFFER_MAX,fin)) {
        if(buff[0]=='/' && buff[1]=='/') continue;
        if(buff[1]=='\n') continue;
        if(buff[0]=='\0') continue;
        char station1[STATION_NAME_MAX+1],station2[STATION_NAME_MAX+1];
        char *p = strtok(buff,separators);
        if(p[0]=='\"') {
                // parse first station

                if(p[1]=='\"') {
                    fprintf(stderr,"Can't accept empty string as station name!\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(station1,p+1);
                while(station1[(int)strlen(station1)-1]!='\"') {
                    p=strtok(0,separators);
                    strcat(station1," ");
                    strcat(station1, p);
                }
                station1[(int)strlen(station1)-1]='\0';

                // parse second station
                p=strtok(0,separators);
                if(p[0]=='\"') {
                    if(p[1]=='\"') {
                        fprintf(stderr,"Can't accept empty string as station name!\n");
                        exit(EXIT_FAILURE);
                    }
                    strcpy(station2,p+1);
                    while(station2[(int)strlen(station2)-1]!='\"') {
                        p=strtok(0,separators);
                        strcat(station2," ");
                        strcat(station2, p);
                    }
                    station2[(int)strlen(station2)-1]='\0';

                    // parse costs

                    p=strtok(0,separators);
                    if(p==nullptr) {
                        fprintf(stderr, "Parsing error, number expected.");
                        exit(EXIT_FAILURE);
                    }
                    int travel_time=atoi(p);
                    p=strtok(0,separators);
                    if(p==nullptr) {
                        fprintf(stderr, "Parsing error, number expected.");
                        exit(EXIT_FAILURE);
                    }
                    float cost=atof(p);
                    normalize_string(station1);
                    normalize_string(station2);
                    if(strcmp(station1,station2)==STRING_MATCH) {
                        fprintf(stderr,"Stations can't match!\n");
                        exit(EXIT_FAILURE);
                    }
                    add_line(stations,nr_stations, station1,station2,travel_time,cost);
                }
                else {
                    fprintf(stderr,"Stations file is not in the correct format.\n");
                    exit(EXIT_FAILURE);
                }
        }
        else {
            fprintf(stderr,"Stations file is not in the correct format.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void add_line(struct stations_list **stations, int *nr_stations, char *station1, char *station2, int travel_time, float cost) {
    struct stations_list *p;
    p = station_seek(station1,*stations);
    if(p==nullptr) 
        p = add_station(station1,stations,nr_stations);
    struct station_link *q;
    bool ok=0;

    q = destination_seek(station2,p->destinations);
    if(q==nullptr)
        add_destination(station2,&p->destinations,travel_time,cost); 
}

struct station_link* add_destination(char *station, struct station_link **destinations, int travel_time, float cost) {
    if(*destinations==nullptr) {
        *destinations = (struct station_link*) malloc(sizeof(struct station_link));
        strcpy((*destinations)->arriving_station,station);
        (*destinations)->cost = cost;
        (*destinations)->travel_time = travel_time;
        (*destinations)->next = nullptr;
        return *destinations;
    }
    struct station_link *p;
    for(p=*destinations; p->next ; p=p->next) {
    }
    p->next = (struct station_link*) malloc(sizeof(struct station_link));
    strcpy(p->next->arriving_station,station);
    p->next->cost = cost;
    p->next->travel_time = travel_time;
    p->next->next = nullptr;
    return p->next;
}

struct station_link* destination_seek(char *station, struct station_link *destinations) {
    if(destinations==nullptr) 
        return nullptr;
    struct station_link *p;
    for(p=destinations; p ; p=p->next)
        if(strcmp(p->arriving_station,station)==STRING_MATCH) return p;
    return nullptr;
}

struct stations_list* station_seek(char *station, struct stations_list *stations) {
    if(stations==nullptr) return nullptr;
    struct stations_list *p;
    for(p=stations; p!=nullptr; p=p->next)  {
        if(strcmp(p->starting_station,station)==STRING_MATCH) return p;
    }
    return nullptr;
}

struct stations_list* add_station(char *station, struct stations_list **stations, int *nr_stations) {
    if(*stations==nullptr) {
        *stations = (struct stations_list*) malloc(sizeof(struct stations_list));
        strcpy((*stations)->starting_station,station);
        (*stations)->destinations = nullptr;
        (*stations)->next = nullptr;
        *nr_stations = 1;
        return *stations;
    }
    struct stations_list *p;
    for(p=*stations; p->next!=nullptr; p=p->next) {
    }
    p->next = (struct stations_list*) malloc(sizeof(struct stations_list));
    p->next->next=nullptr;
    strcpy(p->next->starting_station,station);
    p->next->destinations=nullptr;
    *nr_stations += 1;
    return p->next;
}

#endif
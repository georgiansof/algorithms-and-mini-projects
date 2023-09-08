// @georgiansof proiect Programarea Calculatoarelor 2022 Sem. 1

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <search.h>

#include "headers/globals.h"
#include "headers/function_decl.h"
#include "headers/priority_queue.h"
#include "headers/utils.h"


/// aplicatie de calculare traseu intre statii

int main(int argc, char *argv[]) {
    if(argc<2) {
        fprintf(stderr,"No database provided! Usage: %s DATABASE_FILE\n",argv[0]);
        return EXIT_FAILURE;
    }
    struct stations_list *stations=nullptr;
    int nr_stations = 0;
    parse_station_file(argv[1],&stations,&nr_stations);


    enum {CALCULATE_TRIP=1, SHOW_STATIONS, STATION_EXISTS_DEPARTURE, STATION_EXISTS_ARRIVAL, STATION_EXISTS_ANY};
    short task,cnt;
    printf("Public transport trip calculator:\n");
    printf("[1] Calculate trip\n");
    printf("[2] Show all stations and their immediately linked stations\n");
    printf("[3] Check if given station exists in the departure database.\n");
    printf("[4] Check if given station exists in the arrival database.\n");
    printf("[5] Check if given station exists in any database.\n");
    printf("Your action > ");
    cnt=scanf("%hd",&task);
    if(cnt!=1) {
        fprintf(stderr,"Incorrect format!\n");
        return EXIT_FAILURE;
    }
    switch(task) {
        case CALCULATE_TRIP: {
            char starting_station[STATION_NAME_MAX + 1] , arriving_station[STATION_NAME_MAX + 1];

            printf("Insert starting station > ");
            getc(stdin);
            fgets(starting_station,STATION_NAME_MAX+1,stdin);
            normalize_string(starting_station);
            if(!station_exists_departure(starting_station,stations)) {
                fprintf(stderr,"Station \"%s\" does not exist as departure.\n",starting_station);
                return EXIT_FAILURE;
            }

            printf("Insert arriving station > ");
            fgets(arriving_station,STATION_NAME_MAX+1,stdin);
            normalize_string(arriving_station);

            if(!station_exists_any(arriving_station,stations)) {
                fprintf(stderr, "Station \"%s\" does not exist.\n",arriving_station);
                return EXIT_FAILURE;
            }
            int subtask;
            printf("Calculate by optimum - [1] time, [2] cost > ");
            scanf("%d",&subtask);
            if(subtask==1) {
                char depart_choice[10];
                short hour,minutes;
                struct tm *day_time;
                determine_fastest_path(starting_station,arriving_station, stations); 
            }
            else if(subtask==2) {
                determine_cheapest_path(starting_station,arriving_station, stations);
            }
            else {
                fprintf(stderr, "Incorrect request");
                return EXIT_FAILURE;
            }
            break;
        }
        case SHOW_STATIONS: {
            struct stations_list *p;
            int i,j;
            for(p = stations,i=1; p ; p=p->next,++i) {
                printf("%d: %s > ",i,p->starting_station);
                struct station_link *q;
                for(q = p->destinations,j=1 ; q ; q=q->next,++j)
                    printf("[%d]: \"%s\"; ",j,q->arriving_station);

                printf("\n");
            }
            break;
        }
        case STATION_EXISTS_DEPARTURE: {
            char station[STATION_NAME_MAX+1];
            printf("Type station name > ");
            fgets(station,STATION_NAME_MAX,stdin); // flush the \n character
            fgets(station,STATION_NAME_MAX,stdin);
            normalize_string(station);
            if(station_exists_departure(station,stations))
                printf("\"%s\" exists in the departure database.\n",station);
            else
                printf("\"%s\" does not exist in the departure database.\n",station);
            break;
        }
        case STATION_EXISTS_ARRIVAL: {
            char station[STATION_NAME_MAX+1];
            printf("Type station name > ");
            fgets(station,STATION_NAME_MAX,stdin); // flush the \n character
            fgets(station,STATION_NAME_MAX,stdin);
            normalize_string(station);

            printf("\"%s\"%s", station, arrival_station_exists(station,stations) ? " exists in the arrival database.\n" : " exists in the arrival database.\n");
            break;
        }
        case STATION_EXISTS_ANY: {
            char station[STATION_NAME_MAX+1];
            printf("Type station name > ");
            fgets(station,STATION_NAME_MAX,stdin); // flush the \n character
            fgets(station,STATION_NAME_MAX,stdin);
            normalize_string(station);

            printf("\"%s\"%s", station, station_exists_any(station,stations) ? " exists in the database.\n" : " does not exist in the database.\n");
            break;
        }
        default:
            fprintf(stderr,"Not a valid request.\n");
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool station_exists_departure(char *station, struct stations_list *stations) {
    return station_seek(station,stations) != nullptr;
}

bool arrival_station_exists(char *station, struct stations_list *stations) {
    struct stations_list *p;
    if(!stations) return false;
    for(p=stations; p ; p=p->next)
        if(destination_seek(station,p->destinations)!=nullptr)
            return true;
    return false;
}

bool arrival_station_exists_departure_specific(char *station, struct stations_list *stations, char *arriving_station) {
    struct stations_list *p=station_seek(station,stations);
    if(p==nullptr) return false;

    return destination_seek(arriving_station,p->destinations)!=0;
}

bool station_exists_any(char *station, struct stations_list *stations) {
    if(station_exists_departure(station,stations))
        return true;
    if(arrival_station_exists(station,stations))
        return true;
    return false;
}


void determine_fastest_path(char *starting_station, char *arriving_station, struct stations_list *stations) {
    char** map = (char**) calloc(MAX_STATIONS+1 , sizeof(char*));
    for(int i=0;i<=MAX_STATIONS;++i)
        map[i] = (char*) calloc(STATION_NAME_MAX+2,sizeof(char));

    int n = map_stations(map,stations);
    int *parent = (int*) calloc(n+2, sizeof(int));
    float total_cost = 0;
    float *d = (float*) calloc(n , sizeof(float));
    unsigned char *viz = (unsigned char*) malloc(n);
    int start;

    ENTRY e;
    ENTRY *eptr;
    e.key = starting_station;
    eptr = hsearch(e, FIND);
    start = eptr->data;

    for(int i=0;i<n;++i)
        d[i] = -INFINITY, viz[i] = 0;
    d[start] = 0;

    priority_queue pq;
    pq.heap = init_minheap(n+5);
    priority_queue_push(&pq,-d[start],start);

    while(!priority_queue_empty(&pq)) {
        int u = pq_peek(&pq);
        pq_pop(&pq);
        if(!viz[u]) {
            viz[u]=1;
            int j=1;
            struct stations_list *p;
            for(p = stations; p!=nullptr && j!=u; p=p->next, ++j)
                ;
            if(p!=nullptr) {
                for(struct station_link *q = p->destinations; q!=nullptr; q=q->next) {
                    e.key = q->arriving_station;
                    eptr = hsearch(e,FIND);
                    int index = eptr -> data;
                    if(d[index] < d[u] - q->cost) {
                        d[index] = d[u] - q->cost;
                        parent[index] = u;
                        priority_queue_push(&pq,-d[index],index);
                    }
                }
            }
        }
    }

    e.key = arriving_station;
    eptr = hsearch(e,FIND);
    int dest = (int) eptr->data;
    printf("\nEstimated time to complete trip is %.2f\n",-d[dest]);
    
    print_path(parent[dest],parent,map);
    printf("%s\n\n", map[dest]); /// ultima statie se printeaza separat ca sa nu apara o sageata in plus la afisare

    for(int i=1;i<=n;++i)
        free(map[i]);
    free(map);
    free(viz);
    free(d);
    free(parent);
    hdestroy();
    free_minheap(pq.heap);
}

void print_path(int node, int *parentsvec, char **map) {
    if(node!=0) {
        print_path(parentsvec[node],parentsvec,map);
        printf("%s -> ", map[node]);
    }
}

void determine_cheapest_path(char *starting_station, char *arriving_station, struct stations_list *stations) {
    char** map = (char**) calloc(MAX_STATIONS+1 , sizeof(char*));
    for(int i=0;i<=MAX_STATIONS;++i)
        map[i] = (char*) calloc(STATION_NAME_MAX+2,sizeof(char));

    int n = map_stations(map,stations);
    int *parent = (int*) calloc(n+2, sizeof(int));
    float total_cost = 0;
    float *d = (float*) calloc(n , sizeof(float));
    unsigned char *viz = (unsigned char*) malloc(n);
    int start;

    ENTRY e;
    ENTRY *eptr;
    e.key = starting_station;
    eptr = hsearch(e, FIND);
    start = eptr->data;

    for(int i=0;i<n;++i)
        d[i] = INFINITY, viz[i] = 0;
    d[start] = 0;

    priority_queue pq;
    pq.heap = init_minheap(n+5);
    priority_queue_push(&pq,d[start],start);
    while(!priority_queue_empty(&pq)) {
        int u = pq_peek(&pq);
        pq_pop(&pq);
        if(!viz[u]) {
            viz[u]=1;
            int j=1;
            struct stations_list *p;
            for(p = stations; p!=nullptr && j!=u; p=p->next, ++j)
                ;
            if(p!=nullptr) {
                for(struct station_link *q = p->destinations; q!=nullptr; q=q->next) {
                    e.key = q->arriving_station;
                    eptr = hsearch(e,FIND);
                    int index = eptr -> data;
                    if(d[index] > d[u] + q->cost) {
                        d[index] = d[u] + q->cost;
                        parent[index] = u;
                        priority_queue_push(&pq,d[index],index);
                    }
                }
            }
        }
    }

    e.key = arriving_station;
    eptr = hsearch(e,FIND);
    int dest = (int) eptr->data;
    printf("\nCost of trip is %.2f %s\n",d[dest],currency);
    
    print_path(parent[dest],parent,map);
    printf("%s\n\n", map[dest]); /// ultima statie se printeaza separat ca sa nu apara o sageata in plus la afisare

    for(int i=1;i<=n;++i)
        free(map[i]);
    free(map);
    free(viz);
    free(d);
    free(parent);
    hdestroy();
    free_minheap(pq.heap);
}

int map_stations(char **map,struct stations_list *stations) {
    int mapper=1;
    hcreate(MAX_STATIONS);
    ENTRY e;
    for(struct stations_list *p = stations; p!=nullptr; p=p->next) {
        e.key = p->starting_station;
        e.data = mapper;
        strcpy(map[mapper],p->starting_station);
        ++mapper;
        hsearch(e,ENTER);
    }
    for(struct stations_list *p = stations; p!=nullptr; p=p->next) {
        for(struct station_link *q = p->destinations; q!=nullptr; q=q->next) {
            e.key = q->arriving_station;
            e.data = mapper;
            strcpy(map[mapper],q->arriving_station);
            if(hsearch(e,FIND)!=NULL) continue;
            hsearch(e,ENTER);
            ++mapper;
        }
    }
    return mapper-1;
}
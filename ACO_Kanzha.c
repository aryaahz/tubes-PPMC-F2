#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h> // Untuk clock()

#define EARTH_RADIUS 6371.0
#define ALPHA 1.0
#define BETA 2.0 
#define RHO 0.5
#define Q 100
#define INITIAL_PHEROMONE 0.1
#define M_PI  3.14159265358979323846

typedef struct {
    char name[50];
    double latitude;
    double longitude;
    int visited;
} City;

typedef struct {
    double pheromone; 
} Edge;

Edge pheromone[100][100];

double haversine_distance(City city1, City city2) {
    double dlat = (city2.latitude - city1.latitude) * M_PI / 180.0;
    double dlon = (city2.longitude - city1.longitude) * M_PI / 180.0;
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(city1.latitude * M_PI / 180.0) * cos(city2.latitude * M_PI / 180.0) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

void initialize_pheromone(int num_cities) {
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            pheromone[i][j].pheromone = INITIAL_PHEROMONE;
        }
    }
}

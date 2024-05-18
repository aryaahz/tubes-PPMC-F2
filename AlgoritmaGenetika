#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_CITIES 15
#define POP_SIZE 100
#define GEN_MAX 1000
#define MUT_RATE 0.1
#define EARTH_RADIUS 6371.0

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    char name[50];
    double latitude;
    double longitude;
} City;

City cities[MAX_CITIES];
int cityCount = 0;

typedef struct {
    int route[MAX_CITIES];
    double fitness;
} Individual;

Individual population[POP_SIZE];
Individual newPopulation[POP_SIZE];

double toRadians(double degree) {
    return degree * (M_PI / 180.0);
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

double calculateRouteDistance(int* route) {
    double totalDistance = 0.0;
    for (int i = 0; i < cityCount - 1; i++) {
        totalDistance += calculateDistance(
            cities[route[i]].latitude, cities[route[i]].longitude,
            cities[route[i + 1]].latitude, cities[route[i + 1]].longitude
        );
    }
    totalDistance += calculateDistance(
        cities[route[cityCount - 1]].latitude, cities[route[cityCount - 1]].longitude,
        cities[route[0]].latitude, cities[route[0]].longitude
    );
    return totalDistance;
}

void initializePopulation() {
    for (int i = 0; i < POP_SIZE; i++) {
        for (int j = 0; j < cityCount; j++) {
            population[i].route[j] = j;
        }
        for (int j = 1; j < cityCount; j++) {
            int r = rand() % (cityCount - 1) + 1;
            int temp = population[i].route[j];
            population[i].route[j] = population[i].route[r];
            population[i].route[r] = temp;
        }
        population[i].fitness = 1.0 / calculateRouteDistance(population[i].route);
    }
}

Individual selectParent() {
    double totalFitness = 0.0;
    for (int i = 0; i < POP_SIZE; i++) {
        totalFitness += population[i].fitness;
    }
    double randValue = ((double) rand() / RAND_MAX) * totalFitness;
    double partialSum = 0.0;
    for (int i = 0; i < POP_SIZE; i++) {
        partialSum += population[i].fitness;
        if (partialSum >= randValue) {
            return population[i];
        }
    }
    return population[POP_SIZE - 1];
}

void crossover(int* parent1, int* parent2, int* offspring) {
    int start = rand() % cityCount;
    int end = rand() % cityCount;
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    int visited[MAX_CITIES] = {0};
    for (int i = start; i <= end; i++) {
        offspring[i] = parent1[i];
        visited[parent1[i]] = 1;
    }
    int currentIndex = (end + 1) % cityCount;
    for (int i = 0; i < cityCount; i++) {
        int city = parent2[i];
        if (!visited[city]) {
            offspring[currentIndex] = city;
            currentIndex = (currentIndex + 1) % cityCount;
        }
    }
}

void mutate(int* route) {
    for (int i = 1; i < cityCount; i++) {
        if (((double) rand() / RAND_MAX) < MUT_RATE) {
            int j = rand() % (cityCount - 1) + 1;
            int temp = route[i];
            route[i] = route[j];
            route[j] = temp;
        }
    }
}

void evolvePopulation() {
    for (int i = 0; i < POP_SIZE; i++) {
        Individual parent1 = selectParent();
        Individual parent2 = selectParent();
        crossover(parent1.route, parent2.route, newPopulation[i].route);
        mutate(newPopulation[i].route);
        newPopulation[i].fitness = 1.0 / calculateRouteDistance(newPopulation[i].route);
    }
    for (int i = 0; i < POP_SIZE; i++) {
        population[i] = newPopulation[i];
    }
}

void readCitiesFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File tidak dapat dibuka.\n");
        exit(EXIT_FAILURE);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^,],%lf,%lf", cities[cityCount].name, &cities[cityCount].latitude, &cities[cityCount].longitude) == 3) {
            cityCount++;
        } else {
            printf("Format file salah.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

int findCityIndex(char* cityName) {
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i].name, cityName) == 0) {
            return i;
        }
    }
    return -1;
}

void printBestRoute() {
    Individual best = population[0];
    for (int i = 1; i < POP_SIZE; i++) {
        if (population[i].fitness > best.fitness) {
            best = population[i];
        }
    }
    printf("Best route found: ");
    for (int i = 0; i < cityCount; i++) {
        printf("%s -> ", cities[best.route[i]].name);
    }
    printf("%s\n", cities[best.route[0]].name);
    printf("Best route distance: %.6f km\n", 1.0 / best.fitness);
}

int main() {
    char filename[100];
    char startCity[50];

    printf("Enter list of cities file name: ");
    scanf("%s", filename);
    printf("Enter starting point: ");
    scanf("%s", startCity);

    readCitiesFromFile(filename);

    int startIndex = findCityIndex(startCity);
    if (startIndex == -1) {
        printf("Kota keberangkatan tidak ditemukan dalam daftar kota.\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    initializePopulation();
    clock_t startTime = clock();

    for (int gen = 0; gen < GEN_MAX; gen++) {
        evolvePopulation();
    }

    clock_t endTime = clock();
    printBestRoute();
    printf("Time elapsed: %.10f s\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);

    return 0;
}

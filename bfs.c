#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define MAX_CITIES 16
#define INITIAL_QUEUE_SIZE 1410065408
#define PI 3.14159265358979323846
#define R 6371 // Radius of the Earth in km

typedef struct {
    char name[100];
    double latitude;
    double longitude;
} City;

typedef struct {
    int cities[MAX_CITIES];
    int count;
    double distance;
} Route;

double toRadians(double degree) {
    return degree * (PI / 180);
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);
    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

void readCities(const char* fileName, City cities[], int* numCities) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    *numCities = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        strcpy(cities[*numCities].name, token);
        token = strtok(NULL, ",");
        cities[*numCities].latitude = atof(token);
        token = strtok(NULL, ",");
        cities[*numCities].longitude = atof(token);
        (*numCities)++;
    }
    fclose(file);
}

void printRoute(Route* route, City cities[]) {
    for (int i = 0; i < route->count; i++) {
        printf("%s -> ", cities[route->cities[i]].name);
    }
    printf("%s\n", cities[route->cities[0]].name); // Loop back to start
}

void bfs(City cities[], int numCities, int startCity) {
    int queueSize = INITIAL_QUEUE_SIZE;
    Route *queue = malloc(queueSize * sizeof(Route));
    int front = 0, rear = 0;

    Route initialRoute;
    initialRoute.cities[0] = startCity;
    initialRoute.count = 1;
    initialRoute.distance = 0.0;
    queue[rear++] = initialRoute;

    double minDist = DBL_MAX;
    Route bestRoute;

    while (front < rear) {
        Route currentRoute = queue[front++];
        if (currentRoute.count == numCities) {
            double returnDist = haversine(cities[currentRoute.cities[numCities - 1]].latitude, cities[currentRoute.cities[numCities - 1]].longitude,
                                          cities[startCity].latitude, cities[startCity].longitude);
            currentRoute.distance += returnDist;
            if (currentRoute.distance < minDist) {
                minDist = currentRoute.distance;
                bestRoute = currentRoute;
            }
        } else {
            for (int i = 0; i < numCities; i++) {
                int alreadyVisited = 0;
                for (int j = 0; j < currentRoute.count; j++) {
                    if (currentRoute.cities[j] == i) {
                        alreadyVisited = 1;
                        break;
                    }
                }
                if (!alreadyVisited) {
                    Route newRoute = currentRoute;
                    newRoute.cities[newRoute.count] = i;
                    newRoute.count++;
                    newRoute.distance += haversine(cities[currentRoute.cities[currentRoute.count - 1]].latitude, cities[currentRoute.cities[currentRoute.count - 1]].longitude,
                                                   cities[i].latitude, cities[i].longitude);
                    if (rear >= queueSize) {
                        queueSize *= 2;
                        queue = realloc(queue, queueSize * sizeof(Route));
                        if (queue == NULL) {
                            perror("Unable to allocate memory for the queue");
                            exit(EXIT_FAILURE);
                        }
                    }
                    queue[rear++] = newRoute;
                }
            }
        }
    }

    printf("Best route found:\n");
    printRoute(&bestRoute, cities);
    printf("Best route distance: %.5f km\n", minDist);

    free(queue);
}

int main() {
    City cities[MAX_CITIES];
    int numCities;

    char fileName[256];
    char startCityName[100];
    int startCity = -1;

    printf("Enter list of cities file name: ");
    scanf("%s", fileName);
    readCities(fileName, cities, &numCities);

    printf("Enter starting point: ");
    scanf("%s", startCityName);

    for (int i = 0; i < numCities; i++) {
        if (strcmp(cities[i].name, startCityName) == 0) {
            startCity = i;
            break;
        }
    }

    if (startCity == -1) {
        printf("Starting city not found in the list.\n");
        return 1;
    }

    clock_t begin = clock();
    bfs(cities, numCities, startCity);
    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.10f s\n", timeSpent);


    return 0;
}

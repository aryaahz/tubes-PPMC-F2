/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Modul            : 9 - Tugas Besar
*Nama (NIM)       : Kanzha Haiqa Arsytya (18322011)
*Asisten (NIM)    : Emmanuella Pramudita Rumanti (13220031)
*Deskripsi        : Membuat program untuk menyelesaikan masalah Travelling Salesman Problem dengan menggunakan
pendekatan algoritma Ant Colony Optimization
*/

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

typedef struct {
    char name[50];
    double latitude;
    double longitude;
} City;

typedef struct {
    double pheromone;
} Edge;

Edge pheromone[100][100]; // Array untuk menyimpan nilai pheromone

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

double calculate_probability(int city1, int city2, City cities[], int num_cities, int visited_cities[]) {
    double pheromone_factor = pow(pheromone[city1][city2].pheromone, ALPHA);
    double distance_factor = pow(1 / haversine_distance(cities[city1], cities[city2]), BETA);
    double total = 0.0;

    for (int i = 0; i < num_cities; i++) {
        if (visited_cities[i] == 0) {
            total += pow(pheromone[city1][i].pheromone, ALPHA) * pow(1 / haversine_distance(cities[city1], cities[i]), BETA);
        }
    }

    return (pheromone_factor * distance_factor) / total;
}

int select_next_city(int current_city, City cities[], int num_cities, int visited_cities[]) {
    double max_probability = -1.0;
    int selected_city = -1;

    for (int i = 0; i < num_cities; i++) {
        if (visited_cities[i] == 0) {
            double probability = calculate_probability(current_city, i, cities, num_cities, visited_cities);
            if (probability > max_probability) {
                max_probability = probability;
                selected_city = i;
            }
        }
    }

    return selected_city;
}

void update_pheromone(int num_cities, int route[], double total_distance) {
    for (int i = 0; i < num_cities; i++) {
        int city1 = route[i];
        int city2 = route[(i + 1) % num_cities];
        pheromone[city1][city2].pheromone += Q / total_distance;
        pheromone[city2][city1].pheromone += Q / total_distance;
    }

    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            pheromone[i][j].pheromone *= (1 - RHO);
        }
    }
}

void aco_algorithm(City cities[], int num_cities, int best_route[], double *best_distance, double *time_elapsed, int start_city) {
    initialize_pheromone(num_cities);
    int num_ants = num_cities;
    int routes[num_ants][num_cities];
    double distances[num_ants];

    clock_t start_time = clock(); // Catat waktu awal

    for (int iteration = 0; iteration < 100; iteration++) {

        for (int ant = 0; ant < num_ants; ant++) {
            int visited_cities[num_cities];
            memset(visited_cities, 0, sizeof(visited_cities));

            int current_city = start_city;
            visited_cities[current_city] = 1;
            routes[ant][0] = current_city;

            double total_distance = 0;
            for (int i = 1; i < num_cities; i++) {
                int next_city = select_next_city(current_city, cities, num_cities, visited_cities);
                visited_cities[next_city] = 1;
                routes[ant][i] = next_city;
                total_distance += haversine_distance(cities[current_city], cities[next_city]);
                current_city = next_city;
            }
            total_distance += haversine_distance(cities[current_city], cities[start_city]);
            distances[ant] = total_distance;

            if (*best_distance == -1 || total_distance < *best_distance) {
                *best_distance = total_distance;
                memcpy(best_route, routes[ant], num_cities * sizeof(int));
            }
        }

 
        for (int ant = 0; ant < num_ants; ant++) {
            update_pheromone(num_cities, routes[ant], distances[ant]);
        }
    }

    clock_t end_time = clock(); // Catat waktu akhir
    *time_elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Hitung selisih waktu
}

int main() {
    FILE *file;
    char filename[50];
    printf("Masukkan nama file input (*.csv): ");
    scanf("%s", filename);

    // Baca file input
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return 1;
    }

    // Membaca data kota dari file CSV
    int num_cities = 0;
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        num_cities++;
    }
    rewind(file); // Kembalikan posisi file ke awal

    // Inisialisasi array of struct untuk menyimpan data kota
    City cities[num_cities];

    // Membaca data kota dari file CSV dan memasukkannya ke dalam array of struct
    for (int i = 0; i < num_cities; i++) {
        fgets(line, sizeof(line), file);
        sscanf(line, "%[^,],%lf,%lf", cities[i].name, &cities[i].latitude, &cities[i].longitude);
    }
    fclose(file);

    // Meminta pengguna memasukkan kota keberangkatan
    char departure_city[50];
    printf("Masukkan kota keberangkatan: ");
    scanf("%s", departure_city);

    // Cari indeks kota keberangkatan
    int departure_index = -1;
    for (int i = 0; i < num_cities; i++) {
        if (strcmp(cities[i].name, departure_city) == 0) {
            departure_index = i;
            break;
        }
    }
    if (departure_index == -1) {
        printf("Kota keberangkatan tidak ditemukan.\n");
        return 1;
    }

    // Menjalankan algoritma ACO
    srand(time(NULL)); 
    int best_route[num_cities];
    double best_distance = -1;
    double time_elapsed = 0;

    aco_algorithm(cities, num_cities, best_route, &best_distance, &time_elapsed, departure_index);

    // Print best route
    printf("Best Route Found:\n");
    for (int i = 0; i < num_cities; i++) {
        printf("%s -> ", cities[best_route[i]].name);
    }
    printf("%s\n", cities[best_route[0]].name); // Kembali ke kota keberangkatan
    printf("Best Route Distance: %.2f km\n", best_distance);
    printf("Time Elapsed: %.6f seconds\n", time_elapsed);

    return 0;
}

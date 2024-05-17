#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

#define PI 3.14159265358979323846
#define RADIUS 6371.0 // Radius of the Earth in kilometers
#define MAX_CITIES 20

typedef struct {
    char nama[50];
    double latitude;
    double longitude;
} Kota;

// Function to convert degrees to radians
double to_radians(double degree) {
    return degree * PI / 180.0;
}

// Function to calculate distance between two cities using Haversine formula
double hitung_jarak(Kota kota1, Kota kota2) {
    double lat1 = to_radians(kota1.latitude);
    double lon1 = to_radians(kota1.longitude);
    double lat2 = to_radians(kota2.latitude);
    double lon2 = to_radians(kota2.longitude);

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return RADIUS * c;
}

// Function to print the route
void print_route(int *route, int n, Kota *kota) {
    for (int i = 0; i < n; i++) {
        printf("%s -> ", kota[route[i]].nama);
    }
    printf("%s\n", kota[route[0]].nama); // Return to the start city
}

// Recursive function to find the shortest route using dynamic programming
double tsp(int start, int set, int n, double **dist, double **dp, int **path) {
    if (set == 0)
        return dist[start][0];

    if (dp[start][set] != -1)
        return dp[start][set];

    double res = DBL_MAX;
    for (int i = 0; i < n; i++) {
        if (set & (1 << i)) {
            double temp = dist[start][i] + tsp(i, set & ~(1 << i), n, dist, dp, path);
            if (temp < res) {
                res = temp;
                path[start][set] = i;
            }
        }
    }
    dp[start][set] = res;
    return res;
}

int main() {
    char file_name[100];
    printf("Enter list of cities file name: ");
    scanf("%s", file_name);

    FILE *file = fopen("C:\\Users\\Mutia\\Documents\\JarakKota.csv", "r");
    if (file == NULL) {
        printf("File tidak dapat dibuka.\n");
        return 1;
    }

    // Count the number of cities
    int jumlah_kota = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        jumlah_kota++;
    }
    jumlah_kota--; // Because of header row

    rewind(file);

    // Read city data from the file
    Kota kota[jumlah_kota];
    fscanf(file, "%*[^\n]\n"); // Skip header
    for (int i = 0; i < jumlah_kota; i++) {
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            sscanf(buffer, "%[^;];%lf;%lf", kota[i].nama, &kota[i].latitude, &kota[i].longitude);
        }
    }

    fclose(file);

    // Display available cities
    printf("\nKota yang tersedia:\n");
    for (int i = 0; i < jumlah_kota; i++) {
        printf("%d. %s\n", i + 1, kota[i].nama);
    }

    // Input starting city
    int kota_keberangkatan;
    printf("\nEnter starting point: ");
    scanf("%d", &kota_keberangkatan);
    kota_keberangkatan--;

    // Create distance matrix
    double **dist = (double **)malloc(jumlah_kota * sizeof(double *));
    for (int i = 0; i < jumlah_kota; i++) {
        dist[i] = (double *)malloc(jumlah_kota * sizeof(double));
        for (int j = 0; j < jumlah_kota; j++) {
            dist[i][j] = hitung_jarak(kota[i], kota[j]);
        }
    }

    // Create DP table and path table
    int N = (1 << jumlah_kota);
    double **dp = (double **)malloc(jumlah_kota * sizeof(double *));
    int **path = (int **)malloc(jumlah_kota * sizeof(int *));
    for (int i = 0; i < jumlah_kota; i++) {
        dp[i] = (double *)malloc(N * sizeof(double));
        path[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            dp[i][j] = -1;
            path[i][j] = -1;
        }
    }

    // Solve TSP
    double min_dist = tsp(kota_keberangkatan, (1 << jumlah_kota) - 2, jumlah_kota, dist, dp, path);

    // Reconstruct the path
    int *route = (int *)malloc((jumlah_kota + 1) * sizeof(int));
    int set = (1 << jumlah_kota) - 2;
    int current = kota_keberangkatan;
    route[0] = kota_keberangkatan;
    for (int i = 1; i < jumlah_kota; i++) {
        route[i] = path[current][set];
        set &= ~(1 << route[i]);
        current = route[i];
    }
    route[jumlah_kota] = kota_keberangkatan;

    // Print the shortest route
    printf("\nBest route found:\n");
    print_route(route, jumlah_kota + 1, kota);

    printf("\nBest route distance: %.5f km\n", min_dist);

    // Free allocated memory
    for (int i = 0; i < jumlah_kota; i++) {
        free(dist[i]);
        free(dp[i]);
        free(path[i]);
    }
    free(dist);
    free(dp);
    free(path);
    free(route);

    return 0;
}

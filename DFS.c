#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 255

typedef struct {
    char kota[MAX];
    double lintang;
    double bujur;
} Peta;

float distance(Peta kota1, Peta kota2) {
    float dl = 0, db = 0;
    dl = kota1.lintang - kota2.lintang;
    db = kota1.bujur - kota2.bujur;
    return sqrt((dl*dl) + (db*db));
}

void tsp(Peta kota[MAX], float jarak[MAX][MAX], bool visited[], int currPos, int jumkota, int count, float cost, float *ans, int path[], int *minpath){
    if(count == jumkota && jarak[currPos][0]){
        if (*ans > cost + jarak[currPos][0]){
            *ans = cost + jarak[currPos][0];
            for(int i = 0; i < jumkota; i++){
                minpath[i] = path[i];
            }
        }
        return;
    }

    for (int i = 0; i < jumkota; i++){
        if(!visited[i] && jarak[currPos][i]){
            visited[i] = true;
            path[count] = i;
            tsp(kota, jarak, visited, i, jumkota, count + 1, cost + jarak[currPos][i], ans, path, minpath);
            visited[i] = false;
        }
    }
}

int main() {
    // Waktu
    clock_t start_time = clock();


    // Buka file, cek validitas file
    char namafile[MAX];
    printf("Enter list of cities file name: ");
    scanf("%s", namafile);
    FILE *file = fopen(namafile, "r");

    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return -1;
    }

    Peta peta[MAX];
    char line[MAX];
    int jumKota = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token;
        token = strtok(line, ",");
        strcpy(peta[jumKota].kota, token);

        token = strtok(NULL, ",");
        peta[jumKota].lintang = atof(token);

        token = strtok(NULL, ",");
        peta[jumKota].bujur = atof(token);

        jumKota++;
    }

    fclose(file);


    // Jarak antar kota
    float jarak[MAX][MAX], completed[MAX];
    char City[MAX][MAX];

    for(int j = 0; j < jumKota; j++) {
        strcpy(City[j], peta[j].kota);
        for(int k = 0; k < jumKota; k++) { //Jarak dari kota 1 ke kota 1 lgsg dijadiin 0 aja di matriks
            jarak[j][k] = distance(peta[j], peta[k]);
        }
        completed[j] = 0;
    }


    bool visited[MAX];

    for (int i = 0; i < jumKota; i++){
        visited[i] = false;
    }


    // Posisi awal
    char startCity [MAX];
    int position;

    printf("Enter starting point: ");
    scanf("%s",startCity);

    for (int i = 0; i < jumKota; i++){
        if (strcmp(startCity, City[i]) == 0){
            position = i;
            break;
        }
    }


    // Inti kode
    visited[position] = true;
    float ans = INT_MAX;
    int path[MAX];
    int minpath[MAX];

    path[0] = position;

    tsp(peta, jarak, visited, position, jumKota, 1, 0, &ans, path, minpath);


    // Output 
    printf("Best route found:\n");

    for (int i = 0; i < jumKota; i++){
        printf("%s -> ", peta[minpath[i]].kota);
    }

    printf("%s\n", peta[minpath[0]].kota);
    printf("Best route distance: %.4f\n", ans);


    // Waktu
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Time elapsed: %.4f s", elapsed_time);

    return 0;
}

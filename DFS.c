/**Tugas Besar EL2008 Praktikum Pemecahan Masalah dengan C 2023/2024
 * Nama (NIM)       : Ronaldo (18322007)
 * Kelompok         : F2
 * Asisten (NIM)    : Emmanuella Pramudita Rumanti (13220031)
 * Nama File        : DFS.c
 * Deskripsi        : Program penyelesaian travelling salesman dengan algoritma dfs 
 * Maksimal input   : 14 kota (15 kota keatas > 900 sec)
*/

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 255
#define M_PI 3.14159265358979323846

typedef struct {
    char kota[MAX];
    double lintang;
    double bujur;
} Peta;

float distance(Peta kota1, Peta kota2) {
    float l1, b1, l2, b2, a;
    //Langsung ke radian
    l1 = kota1.lintang*M_PI/180; b1 = kota1.bujur*M_PI/180;
    l2 = kota2.lintang*M_PI/180; b2 = kota2.bujur*M_PI/180;
    
    a = 0.5*(1-cos(l2-l1)+cos(l1)*cos(l2)*(1-cos(b2-b1)));
    //Formula Haversine
    return 6371*2*asin(sqrt(a));
}

void tsp(Peta kota[MAX], float jarak[MAX][MAX], bool visited[], int currPos, int jumkota, int count, float cost, float *ans, int path[], int *minpath, double *time_elapsed){
    // Waktu
    clock_t start_time = clock();


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
            tsp(kota, jarak, visited, i, jumkota, count + 1, cost + jarak[currPos][i], ans, path, minpath, time_elapsed);
            visited[i] = false;
        }
    }
    

    // Waktu
    clock_t end_time = clock();


    *time_elapsed = (double)(end_time - start_time)/ CLOCKS_PER_SEC;
}

int main() {
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

    srand(time(NULL));
    path[0] = position;
    double time_elapsed = 0;


    tsp(peta, jarak, visited, position, jumKota, 1, 0, &ans, path, minpath, &time_elapsed);


    // Output 
    printf("Best route found:\n");

    for (int i = 0; i < jumKota; i++){
        printf("%s -> ", peta[minpath[i]].kota);
    }

    printf("%s\n", peta[minpath[0]].kota);
    printf("Best route distance: %.4f\n", ans);


    printf("Time elapsed: %.lf s", time_elapsed);

    return 0;
}

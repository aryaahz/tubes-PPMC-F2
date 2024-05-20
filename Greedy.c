/**Tugas Besar EL2008 Praktikum Pemecahan Masalah dengan C 2023/2024
 * Nama (NIM)       : Audrey Ahzab (18322010)
 * Kelompok         : F2
 * Asisten (NIM)    : Emmanuella Pramudita Rumanti (13220031)
 * Nama File        : Greedy.c
 * Deskripsi        : Program 'penyelesaian' Traveling Salesman Problem dengan algoritma Greedy 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX 255
#define M_PI 3.14159265358979323846

//Struct yang menyimpan data kota
typedef struct {
    char kota[MAX];
    double lintang;
    double bujur;
} Peta;

//Fungsi perhitungan jarak antarkota
float distance(Peta kota1, Peta kota2) {
    float l1, b1, l2, b2, a;
    //Langsung ke radian
    l1 = kota1.lintang*M_PI/180; b1 = kota1.bujur*M_PI/180;
    l2 = kota2.lintang*M_PI/180; b2 = kota2.bujur*M_PI/180;
    
    a = 0.5*(1-cos(l2-l1)+cos(l1)*cos(l2)*(1-cos(b2-b1)));
    //Formula Haversine
    return 6371*2*asin(sqrt(a));
}

//Fungsi pemilihan kota dengan jarak terdekat dari node kota awal (beserta penyetoran jarak ke jarakTotal) 
int pilihTerdekat(int indeksKota, int jumKota, float jarak[jumKota][jumKota], int completed[jumKota], float *jarakTotal) {
    int i, nextKota = -1;
    float min = 999999.0;

    for(i = 0; i < jumKota; i++) {
        if (completed[i] == 0 && jarak[indeksKota][i] < min) {
            min = jarak[indeksKota][i];
            nextKota = i;
        }
    }

    if (nextKota != -1) {
        *jarakTotal += min;
    }

    return nextKota;
}

//Fungsi rekursif pemilihan kota hingga semua kota telah dikunjungi
void minJarak(int indeksKota, int jumKota, int completed[jumKota], Peta peta[jumKota], float jarak[jumKota][jumKota], float *jarakTotal) {
    int nextKota;
    completed[indeksKota] = 1;

    //Print path
    printf("%s ---> ", peta[indeksKota].kota);
    nextKota = pilihTerdekat(indeksKota, jumKota, jarak, completed, jarakTotal);

    //Base case
    if (nextKota == -1) {
        nextKota = 0;
        printf("%s", peta[nextKota].kota);
        *jarakTotal += jarak[indeksKota][nextKota];
        return;
    }

    minJarak(nextKota, jumKota, completed, peta, jarak, jarakTotal);
}

int main() {
    //Buka file, cek validitas file
    char namafile[MAX];
    printf("Masukkan nama file: ");
    scanf("%s", namafile);
    FILE *file = fopen(namafile, "r");

    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return -1;
    }

    Peta peta[MAX];
    char line[MAX];
    int jumKota = 0;
    float jarakTotal = 0;

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

    //Print database kota
    printf("\nData kota pada struct:");
    for (int j = 0; j < jumKota; j++) {
        printf("Kota: %s, Latitude: %.4f, Longitude: %.4f\n", peta[j].kota, peta[j].lintang, peta[j].bujur);
    }

    //Buat matriks jarak antarkota dan print
    float jarak[jumKota][jumKota];
    int completed[jumKota];
    printf("\nMatriks jarak antarkota:");
    for(int j = 0; j < jumKota; j++) {
        for(int k = 0; k < jumKota; k++) { //Jarak dari kota 1 ke kota 1 lgsg dijadiin 0 aja di matriks
            jarak[j][k] = distance(peta[j], peta[k]);
        }
        completed[j] = 0;
    }

    for(int i = 0; i < jumKota; i++) {
        printf("\n\t");

        for(int j = 0; j < jumKota; j++) {
            if (i == 0) printf("%.4s\t", peta[j].kota);
        }

        printf("\n");
        printf("%.4s", peta[i].kota);        

        for (int k = 0; k < jumKota; k++) {
            printf("\t%.2f", jarak[i][k]);
        }
    }

    //Pelaksanaan algoritma Greedy dan perekaman waktu eksekusinya
    clock_t t; 
    t = clock();
    
    printf("\n\nRute optimal menurut Greedy Algorithm:\n");
    minJarak(0, jumKota, completed, peta, jarak, &jarakTotal);
    printf("\nJarak tempuh: %.5f km", jarakTotal);

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; //dalam detik
    printf("\nGreedy took %f seconds to execute \n", time_taken);
    return 0;
}

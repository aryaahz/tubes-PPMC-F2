/*
Program Parsing File .csv ke Struct Peta dan Matriks Jarak untuk TSP
Semua variabel telah diinisialisasi saat parsing file.
Berikut file yang dapat digunakan langsung oleh algoritma:
struct: Peta peta (dengan data nama kota, lintang, dan bujur, akses dengan indeks 0 s.d. (jumKota-1))
int: jumKota (jumlah Kota);
float: jarak[jumKota][jumKota] (matriks jarak antarkota dalam km, akses dengan indeks sesuai matriks yang di-print program)

File dengan nilai nol untuk menyimpan progres algoritma:
int: completed[jumKota] (untuk merekam apakah kota telah di-visit atau belum);
float: jarakTotal (untuk merekam jarak tiap path);
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

//LANJUT FUNGSI-FUNGSI AS NEEDED

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

    // Print database kota
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

    //Printingnya pake header buat baris dan kolom, tapi matriks jaraknya tetep cuma jarak ya isinya
    //Jadi nama kota gaada di matriks
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
    
    clock_t t; 
    t = clock();
    
    printf("\n\nRute optimal menurut Algoritma:\n");
    //INSERT FUNCTION HERE
    
    printf("\nJarak tempuh: ... km"); //Jarak total

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("\Algorithm took %f seconds to execute \n", time_taken);
    return 0;
}

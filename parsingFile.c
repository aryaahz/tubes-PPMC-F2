#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    float jarak[jumKota][jumKota], completed[jumKota];
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

    return 0;
}

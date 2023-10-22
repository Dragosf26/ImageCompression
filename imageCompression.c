/*Fromea Dragos-Florin - 312CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
}colors;

typedef struct node{
    unsigned char tipul_nodului;
    unsigned char valoare_pentru_rosu;   
    unsigned char valoare_pentru_verde; 
    unsigned char valoare_pentru_albastru;
    struct node* stanga_sus;
    struct node* dreapta_sus;
    struct node* dreapta_jos;
    struct node* stanga_jos;
}node;

//functia de calculare a scorului similaritatii
int calculateMean(colors **matrix, int x, int y, int size, node **root,
unsigned long long *red, unsigned long long *green, unsigned long long *blue) {
    unsigned long long i, j;
    unsigned long long mean = 0;
    *red = *green = *blue = 0;

    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            *red = *red + matrix[i][j].R;
            *green = *green + matrix[i][j].G;
            *blue = *blue + matrix[i][j].B;
        }
    }

    *red = *red / (size * size);
    *green = *green / (size * size);
    *blue = *blue / (size * size);

    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            mean = mean + (*red - matrix[i][j].R) * (*red - matrix[i][j].R);
            mean = mean + (*green - matrix[i][j].G) * (*green - matrix[i][j].G);
            mean = mean + (*blue - matrix[i][j].B) * (*blue - matrix[i][j].B);
        }
    }

    return mean / ((size * size) * 3);
}

//functia de creare a arborelui cuaternar
void createTree(node **root, int x, int y, int size, int tol, colors **matrix, int *nr) {
    unsigned long long red, green, blue;

    if (calculateMean(matrix, x, y, size, &(*root), &red, &green, &blue) > tol) {
        *root = malloc(sizeof(node));
        (*root)->tipul_nodului = 0;
        (*root)->stanga_sus = (*root)->dreapta_sus = (*root)->dreapta_jos = (*root)->stanga_jos = NULL;
        createTree(&((*root)->stanga_sus), x, y, size / 2, tol, matrix, nr);
        createTree(&((*root)->dreapta_sus), x, y + size / 2, size / 2, tol, matrix, nr);
        createTree(&((*root)->dreapta_jos), x + size / 2, y + size / 2, size / 2, tol, matrix, nr);
        createTree(&((*root)->stanga_jos), x + size / 2, y, size / 2, tol, matrix, nr);
    } else if(calculateMean(matrix, x, y, size, &(*root), &red, &green, &blue) <= tol) {
        (*nr)++;
        *root = malloc(sizeof(node));
        (*root)->tipul_nodului = 1;
        (*root)->valoare_pentru_rosu = (unsigned char)red;
        (*root)->valoare_pentru_verde = (unsigned char)green;
        (*root)->valoare_pentru_albastru = (unsigned char)blue;
        (*root)->stanga_sus = (*root)->dreapta_sus = (*root)->dreapta_jos = (*root)->stanga_jos = NULL;
    }
}

//functia de calculare a inaltimii maxime
int maxDepth(node* radacina) {
    if (radacina == NULL) {
        return 0;
    }
    int inaltime_stanga_sus = maxDepth(radacina->stanga_sus);
    int inaltime_dreapta_sus = maxDepth(radacina->dreapta_sus);
    int inaltime_dreapta_jos = maxDepth(radacina->dreapta_jos);
    int inaltime_stanga_jos = maxDepth(radacina->stanga_jos);
    int inaltime_maxima_fii = inaltime_stanga_sus;
    if (inaltime_dreapta_sus > inaltime_maxima_fii) {
        inaltime_maxima_fii = inaltime_dreapta_sus;
    }
    if (inaltime_dreapta_jos > inaltime_maxima_fii) {
        inaltime_maxima_fii = inaltime_dreapta_jos;
    }
    if (inaltime_stanga_jos > inaltime_maxima_fii) {
        inaltime_maxima_fii = inaltime_stanga_jos;
    }
    return 1 + inaltime_maxima_fii;
}

//functia de calculare a inaltimii minime
int minDepth(node* radacina) {
    if (radacina == NULL) {
        return 0;
    }

    int inaltime_stanga_sus = minDepth(radacina->stanga_sus);
    int inaltime_dreapta_sus = minDepth(radacina->dreapta_sus);
    int inaltime_dreapta_jos = minDepth(radacina->dreapta_jos);
    int inaltime_stanga_jos = minDepth(radacina->stanga_jos);
    int inaltime_minima_fiu = inaltime_stanga_sus;

    if (inaltime_minima_fiu > inaltime_dreapta_sus) {
        inaltime_minima_fiu = inaltime_dreapta_sus;
    }
    if (inaltime_minima_fiu > inaltime_dreapta_jos) {
        inaltime_minima_fiu = inaltime_dreapta_jos;
    }
    if (inaltime_minima_fiu > inaltime_stanga_jos) {
        inaltime_minima_fiu = inaltime_stanga_jos;
    }
    return 1 + inaltime_minima_fiu;
}

//functia de printare pe nivele
void printLevel(node* root, int level, FILE *fp) {
    if (root == NULL) {
        return;
    }
    if (level == 1) {
        if(root->tipul_nodului == 0) {
            fwrite(&(root->tipul_nodului), sizeof(unsigned char), 1, fp); 
        } else if(root->tipul_nodului == 1) {
            fwrite(&(root->tipul_nodului), sizeof(unsigned char), 1, fp);
            fwrite(&(root->valoare_pentru_rosu), sizeof(unsigned char), 1, fp);
            fwrite(&(root->valoare_pentru_verde), sizeof(unsigned char), 1, fp);
            fwrite(&(root->valoare_pentru_albastru), sizeof(unsigned char), 1, fp);
        }
    } else if (level > 1) {
        printLevel(root->stanga_sus, level - 1, fp);
        printLevel(root->dreapta_sus, level - 1, fp);
        printLevel(root->dreapta_jos, level - 1, fp);
        printLevel(root->stanga_jos, level - 1, fp);
    }
}

//functia de parcurgere pe nivele
void levelOrderTraversal(node* root, FILE *fp) {
    int h = maxDepth(root);
    int i;

    for (i = 1; i <= h; i++){
        printLevel(root, i, fp);
    }
}

//functia de eliberarea a memoriei alocate de matrice
void eliberare_memorie_matrice(colors **matrix, unsigned int dimensiune) {
    int i;
    for (i = 0; i < dimensiune; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

//functia de eliberare a memoriei alocate pentru arbore
void eliberare_memorie_arbore(node* radacina) {
    if (radacina == NULL) {
        return;
    }
    eliberare_memorie_arbore(radacina->stanga_sus);
    eliberare_memorie_arbore(radacina->dreapta_sus);
    eliberare_memorie_arbore(radacina->dreapta_jos);
    eliberare_memorie_arbore(radacina->stanga_jos);
    free(radacina);
}

//functia de citire din fisier
void citire(unsigned int *dimensiune, FILE *fp, colors ***matrix){
    char *type, buffer;
    int maxval, i, j, m;

    type = malloc(11 * sizeof(char));
    fgets(type, 10, fp);
    fscanf(fp, "%d", dimensiune);
    fscanf(fp, "%d", &m);
    fscanf(fp, "%d", &maxval);
    fscanf(fp, "%c",&buffer);

    *matrix = malloc(*dimensiune * sizeof(colors *));
    for (i = 0; i < *dimensiune; i++) {
        (*matrix)[i] = malloc(*dimensiune * sizeof(colors));
    }

    for (i = 0; i < (*dimensiune); i++ ){
        for ( j = 0; j < (*dimensiune); j++){
            fread( &((*matrix)[i][j]), sizeof(colors), 1, fp);
        }
    }
    free(type);
}

int main(int argc, char const *argv[]) {

    node* root = NULL;
    int tol = atoi(argv[argc - 3]);
    int nr = 0;
    colors **matrix;
    unsigned int dimensiune;

    FILE *fp = fopen(argv[argc - 2], "rb");

    //apelarea functiei de citire
    citire(&dimensiune, fp, &matrix);

    fclose(fp);

    //apelarea functiei de creare a arborelui
    createTree(&root, 0, 0, dimensiune, tol, matrix, &nr);

    //printarea raspunsurilor in functie de cerinta
    if (strncmp(argv[argc - 4], "-c1", 3) == 0) {
        fp = fopen(argv[argc - 1], "w");

        fprintf(fp, "%d\n", maxDepth(root));
        fprintf(fp, "%d\n", nr);
        fprintf(fp, "%d\n", dimensiune / (int)(pow(2, minDepth(root) - 1)));

        fclose(fp);
    }
    else if (strncmp(argv[argc - 4], "-c2", 3) == 0) {
        fp = fopen(argv[argc - 1], "wb");
        
        fwrite(&dimensiune, sizeof(unsigned int), 1, fp);
        levelOrderTraversal(root, fp);

        fclose(fp);

    }

    //apelarea functiei de eliberare a memoriei
    eliberare_memorie_matrice(matrix, dimensiune);
    eliberare_memorie_arbore(root);

    return 0;
}

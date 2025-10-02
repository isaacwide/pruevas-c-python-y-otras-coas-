#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// Declaración adelantada de la función
float numeros_aleatorios();
int es_delimitador(char *palabra);

int es_delimitador(char *palabra) {
    // Delimitadores exactos
    const char *delims[] = {
        "DOCUMENT1", "DOCUMENT2", "DOCUMENT3", "DOCUMENT4", "DOCUMENT5",
        "DOCUMENT6", "DOCUMENT7", "DOCUMENT8", "DOCUMENT9", "DOCUMENT10",
        "DOCUMENT11", "DOCUMENT12", "DOCUMENT13", "DOCUMENT14", "DOCUMENT15",
        "DOCUMENT16", "DOCUMENT17", "DOCUMENT18", "DOCUMENT19", "DOCUMENT20",
        "DOCUMENT21", "DOCUMENT22", "DOCUMENT23", "DOCUMENT24", "DOCUMENT25",
        "DOCUMENT26", "DOCUMENT27",
        "document1", "document2", "document3", "document4", "document5",
        "document6", "document7", "document8", "document9", "document10",
        "document11", "document12", "document13", "document14", "document15",
        "document16", "document17", "document18", "document19", "document20",
        "document21", "document22", "document23", "document24", "document25",
        "document26", "document27"
    };
    
    for(int i = 0; i < 54; i++) {
        if(strcmp(palabra, delims[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void word_in_topic(char *filename1, char *filename2) {
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");
    int mtx_1[27][300] = {0};

    if(file1 == NULL || file2 == NULL) {
        printf("Error al abrir los archivos.\n");
        if(file1) fclose(file1);
        if(file2) fclose(file2);
        return;
    }

    double rangos[301] = {0};
    for(int i = 0; i < 300; i++){
        rangos[i+1] = rangos[i] + 0.00333;
    }

    char palabra[100];
    int l = 0;
    
    while (fscanf(file1, "%s", palabra) != EOF) {
        if (es_delimitador(palabra)){
            l++;
        } else {
            if(l > 0 && l <= 27) {
                float probabilidad = numeros_aleatorios();
                for(int j = 0; j < 300; j++){
                    if (probabilidad >= rangos[j] && probabilidad < rangos[j+1]){
                        mtx_1[l-1][j]++;
                        break;
                    }
                }
            }
        }
    }

    printf("\nMatriz mtx_1 (todos los valores):\n");
    for(int i = 0; i < 27; i++) {
        for(int j = 0; j < 300; j++) {
            printf("%d ", mtx_1[i][j]);
        }
        printf("\n");
    }
    
    fclose(file1);
    fclose(file2);
}

void dic_in_topic(char *filename2, char *filename3) {
    // Implementación pendiente
}

float numeros_aleatorios() {
    return (float)rand() / RAND_MAX;
}

int main() {
    srand(time(NULL));
    
    char filename1[] = "principito_lemas.txt";
    char filename2[] = "topicos.txt";
    char filename3[] = "dic.txt";

    word_in_topic(filename1, filename2);
    dic_in_topic(filename2, filename3);
    
    return 0;
}
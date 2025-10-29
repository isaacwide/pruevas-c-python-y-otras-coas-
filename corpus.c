#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// Declaración adelantada de la función
float numeros_aleatorios();
int es_delimitador(char *palabra);
float numeros_aleatorios() {
    return (float)rand() / RAND_MAX;
}
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

int** word_in_topic(char *filename1, char *filename2) {
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");
    
    // Asignar memoria dinámica para la matriz
    float **mtx_1 = (float**)malloc(13 * sizeof(float*));
    for(int i = 0; i < 13; i++) {
        mtx_1[i] = (float*)calloc(300, sizeof(float)); // calloc inicializa en 0.0
    }

    if(file1 == NULL || file2 == NULL) {
        printf("Error al abrir los archivos.\n");
        if(file1) fclose(file1);
        if(file2) fclose(file2);
        // Liberar memoria antes de retornar
        for(int i = 0; i < 27; i++) {
            free(mtx_1[i]);
        }
        free(mtx_1);
        return NULL;
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
            if(l > 0 && l <= 13) {
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

    FILE *output = fopen("matrices.txt", "w");

    if (output == NULL) {
        printf("Error al abrir el archivo de salida.\n");
        fclose(file1);
        fclose(file2);
        // Liberar memoria antes de retornar
        for(int i = 0; i < 27; i++) {
            free(mtx_1[i]);
        }
        free(mtx_1);
        return NULL;
    }
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 300; j++){
            fprintf(output, "%d ", mtx_1[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);
    fclose(file1);
    fclose(file2);
    return mtx_1;
}

int** dic_in_topic(char *filename2, char *filename3) {
    FILE *file_dic = fopen(filename3, "r");
    if (file_dic == NULL) {
        printf("Error al abrir el archivo diccionario: %s\n", filename3);
        return NULL;
    }

    char diccionario[1300][100];
    int num_palabras_dic = 0;
    while (fscanf(file_dic, "%s", diccionario[num_palabras_dic]) != EOF && num_palabras_dic < 1300) {
        num_palabras_dic++;
    }
    fclose(file_dic);
    printf("Diccionario cargado con %d palabras.\n", num_palabras_dic);

    double rangos[301] = {0};
    for(int i = 0; i < 300; i++){
        rangos[i+1] = rangos[i] + (1.0 / 300.0);
    }

    // Asignar memoria dinámica para la matriz
    float **mtx_2 = (float**)malloc(num_palabras_dic * sizeof(float*));
    for(int i = 0; i < num_palabras_dic; i++) {
        mtx_2[i] = (float*)calloc(300, sizeof(float));
    }

    for (int i = 0; i < num_palabras_dic; i++) {
        float probabilidad = numeros_aleatorios();
        for (int j = 0; j < 300; j++) {
            if (probabilidad >= rangos[j] && probabilidad < rangos[j+1]) {
                mtx_2[i][j] = 1;
                break;
            }
        }
    }

    FILE *out2 = fopen("matriz2.txt","w");
    if (out2 == NULL) {
        printf("Error al abrir matriz2.txt para escritura\n");
        // Liberar memoria antes de retornar
        for(int i = 0; i < num_palabras_dic; i++) {
            free(mtx_2[i]);
        }
        free(mtx_2);
        return NULL;
    }
    
    for (int i = 0; i < num_palabras_dic; i++) {
        for (int j = 0; j < 300; j++) {
            fprintf(out2,"%d ", mtx_2[i][j]);
        }
        fprintf(out2,"\n");
    }
    fclose(out2);
    return mtx_2;
}

int ** new_positions(filename1, filename2, filename3,l) {
    float betha = 50/300;
    float alfa = 0.013;
    float **mtx_1 = word_in_topic(filename1, filename2);
    float **mtx_2 = dic_in_topic(filename2, filename3);

    // calcularemos los nm suma de la m-esimafila de mtx_1 y la n-esima fila de mtx_2
    float* n_m =(float*)calloc(13, sizeof(float*));
    float* n_k =(float*)calloc(1296, sizeof(float*));
    
    for(int i = 0; i < 13; i++) {
        n_m[i] = 0;
    }
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < 300; j++){
            n_m[i] += mtx_1[i][j] ;
        }
        
    } 
    for(int i = 0; i < 1296; i++){
        for(int j = 0; j < 300; j++){
            n_m[i] += mtx_1[i][j] ;
        }
        
    } 

    float **aux = (float**)malloc(13 * sizeof(float*));
            for(int i = 0; i < 13; i++) {
                aux[i] = (float*)calloc(300, sizeof(float)); // calloc inicializa en 0.0
            }

    do
    {
        //auxiliar va a tener los nuevos valores de mtx_1
        // igual a mtx_1
        // mtx_1 = aux
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 300; j++){
                 mtx_1[i][j]= aux[i][j] ;
            }
        }

        // limpiazmos aux
        for(int i = 0; i < 13; i++) {
            for(int j = 0; j < 300; j++){
                 aux[i][j]= 0 ;
            }
        }


        l--;
    } while(l>=0);
    




    

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
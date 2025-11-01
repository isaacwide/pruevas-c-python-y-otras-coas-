#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define documentos 13
#define temas 50
#define palabras_dic 1195

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

float** word_in_topic(char *filename1, char *filename2) {
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");
    
    // Asignar memoria dinámica para la matriz
    float **mtx_1 = (float**)malloc(documentos * sizeof(float*));
    for(int i = 0; i < documentos; i++) {
        mtx_1[i] = (float*)calloc(temas, sizeof(float)); // calloc inicializa en 0.0
    }

    if(file1 == NULL || file2 == NULL) {
        printf("Error al abrir los archivos.\n");
        if(file1) fclose(file1);
        if(file2) fclose(file2);
        // Liberar memoria antes de retornar
        for(int i = 0; i < documentos; i++) {
            free(mtx_1[i]);
        }
        free(mtx_1);
        return NULL;
    }

    double rangos[301] = {0};
    for(int i = 0; i < temas; i++){
        rangos[i+1] = rangos[i] + 0.02;
    }

    char palabra[100];
    int l = 0;
    
    while (fscanf(file1, "%s", palabra) != EOF) {
        if (es_delimitador(palabra)){
            l++;
        } else {
            if(l > 0 && l <= documentos) {
                float probabilidad = numeros_aleatorios();
                for(int j = 0; j < temas; j++){
                    if (probabilidad >= rangos[j] && probabilidad < rangos[j+1]){
                        mtx_1[l-1][j]++;
                        break;
                    }
                }
            }
        }
    }

    fclose(file1);
    fclose(file2);
    return mtx_1;
}

float** dic_in_topic(char *filename2, char *filename3) {
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

    double rangos[51] = {0};
    for(int i = 0; i < temas; i++){
        rangos[i+1] = rangos[i] + (1.0 / 50.0);
    }

    // Asignar memoria dinámica para la matriz
    float **mtx_2 = (float**)malloc(num_palabras_dic * sizeof(float*));
    for(int i = 0; i < num_palabras_dic; i++) {
        mtx_2[i] = (float*)calloc(temas, sizeof(float));  // SOLO 50, NO 300
    }
    
    // CORREGIR ESTE BUCLE - asignar tópico aleatorio a cada palabra
    for(int k = 0; k < num_palabras_dic; k++) {
        float probabilidad = numeros_aleatorios();
        for(int j = 0; j < temas; j++){
            if (probabilidad >= rangos[j] && probabilidad < rangos[j+1]){
                mtx_2[k][j]++;
                break;  // Importante: salir después de asignar
            }
        }
    }

    return mtx_2;
}

float** parametro_sigma(char *filename2, char *filename3){
    
    float betha = 1.0;  // 50/50 = 1.0
    
    printf("Cargando dic_in_topic en parametro_sigma...\n");
    float **mtx_2 = dic_in_topic(filename2, filename3);
    
    if (mtx_2 == NULL) {
        printf("Error: mtx_2 es NULL\n");
        return NULL;
    }
    
    printf("Calculando n_k...\n");
    float* n_k = (float*)calloc(palabras_dic, sizeof(float));
    
    // Calcular n_k para TODAS las palabras (1296)
    for(int i = 0; i < palabras_dic; i++){
        for(int j = 0; j < temas; j++){
            n_k[i] += mtx_2[i][j];
        }
    }
    
    printf("Asignando memoria para sigma...\n");
    // Asignar memoria para sigma - TODAS las 1296 filas
    float **sigma = (float**)malloc(palabras_dic * sizeof(float*));
    for(int i = 0; i < palabras_dic; i++){  // CAMBIAR 1195 a 1296
        sigma[i] = (float*)calloc(temas, sizeof(float));
    }
    
    printf("Calculando sigma...\n");
    // Calcular sigma
    for(int k = 0; k < palabras_dic; k++){
        for(int t = 0; t < temas; t++){
            float a = mtx_2[k][t] + betha;
            float b = n_k[k] + (betha * 50);  // Sumar betha * número de tópicos
            
            if (b > 0) {
                sigma[k][t] = a / b;
            } else {
                sigma[k][t] = 0.0;
            }
        }
    }
    
    printf("Sigma calculado correctamente\n");
    free(n_k);
    
    return sigma;
}

/*float ** new_positions(filename1, filename2, filename3) {
    float betha = 50/50;
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
        for(int j = 0; j < 50; j++){
            n_m[i] += mtx_1[i][j] ;
        }
        
    } 
    for(int i = 0; i < 1195; i++){
        for(int j = 0; j < 50; j++){
            n_k[i] += mtx_2[i][j] ;
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
*/
int main() {
    srand(time(NULL));
    
    char filename1[] = "principito_lemas.txt";
    char filename2[] = "topicos.txt";
    char filename3[] = "dic.txt";

    printf("Generando word_in_topic...\n");
    float **mtx1 = word_in_topic(filename1, filename2);
    
    printf("Generando dic_in_topic...\n");
    float **mtx2 = dic_in_topic(filename2, filename3);
    
    printf("Calculando parametro_sigma...\n");
    float **estimadores = parametro_sigma(filename2, filename3);  // UNA SOLA VEZ
    
    if (estimadores == NULL) {
        printf("Error: estimadores es NULL\n");
        return 1;
    }

    // Guardar matriz2
    FILE *out2 = fopen("matriz2.txt","w");
    if (out2 == NULL) {
        printf("Error al abrir matriz2.txt para escritura\n");
        return 1;
    }
    
    for (int i = 0; i < palabras_dic; i++) {
        for (int j = 0; j < temas; j++) {
            fprintf(out2,"%.0f ", mtx2[i][j]);
        }
        fprintf(out2,"\n");
    }
    fclose(out2);
    printf("matriz2.txt guardado correctamente\n");

    // Guardar matrices (mtx1)
    FILE *output = fopen("matrices.txt", "w");
    if (output == NULL) {
        printf("Error al abrir el archivo de salida1.\n");
        return 1;
    }
    
    for(int i = 0; i < 13; i++){
        for(int j = 0; j < temas; j++){
            fprintf(output, "%.0f ", mtx1[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);
    printf("matrices.txt guardado correctamente\n");

    // Guardar sigma
    FILE *estimador1 = fopen("sigma.txt","w");
    if (estimador1 == NULL) {
        printf("Error al abrir sigma.txt para escritura\n");
        return 1;
    }
    
    printf("Escribiendo sigma.txt...\n");
    for (int i = 0; i < palabras_dic; i++) {
        for (int j = 0; j < temas; j++) {
            fprintf(estimador1,"%f ", estimadores[i][j]);
        }
        fprintf(estimador1,"\n");
    }
    fclose(estimador1);
    printf("sigma.txt guardado correctamente\n");
    
    // Liberar memoria
    for(int i = 0; i < 13; i++) {
        free(mtx1[i]);
    }
    free(mtx1);
    
    for(int i = 0; i < palabras_dic; i++) {
        free(mtx2[i]);
        free(estimadores[i]);
    }
    free(mtx2);
    free(estimadores);
    
    printf("Programa finalizado exitosamente\n");
    return 0;
}
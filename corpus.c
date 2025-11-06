#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define documentos 13
#define temas 50
#define palabras_dic 1195
#define betha 1.0 
#define alfa 0.01


typedef struct {
    char **palabras;
    int num_palabras;
} Diccionario;

Diccionario* cargar_diccionario(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;
    
    Diccionario *dic = (Diccionario*)malloc(sizeof(Diccionario));
    dic->palabras = (char**)malloc(1300 * sizeof(char*));
    dic->num_palabras = 0;
    
    char palabra[100];
    while (fscanf(file, "%s", palabra) != EOF && dic->num_palabras < 1300) {
        dic->palabras[dic->num_palabras] = (char*)malloc(100 * sizeof(char));
        strcpy(dic->palabras[dic->num_palabras], palabra);
        dic->num_palabras++;
    }
    fclose(file);
    return dic;
}

int buscar_palabra_optimizada(Diccionario *dic, char *palabra) {
    // Búsqueda lineal simple pero en memoria (MUCHO más rápido que fscanf)
    for(int i = 0; i < dic->num_palabras; i++) {
        if(strcmp(dic->palabras[i], palabra) == 0) {
            return i;
        }
    }
    return -1;
}

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

float *n_ms(float **mtx_1){
    float *n_m = (float*)calloc(documentos, sizeof(float));
    // Calcular n_m para todos los documentos
    for(int i = 0; i < documentos; i++){
        for(int j = 0; j < temas; j++){
            n_m[i] += mtx_1[i][j];
        }
    }

    return n_m;
}

float*n_ks(float**mtx_2){
    float* n_k = (float*)calloc(palabras_dic, sizeof(float));
    for(int i = 0; i < palabras_dic; i++){
        for(int j = 0; j < temas; j++){
            n_k[i] += mtx_2[i][j];
        }
    }
    return n_k;
}

float** parametro_sigma(float **mtx_2){
    if (mtx_2 == NULL) {
        printf("Error: mtx_2 es NULL\n");
        return NULL;
    }
    // Calcular n_k para TODAS las palabras (1296)
    float * n_k = n_ks(mtx_2);
    
    // Asignar memoria para sigma - TODAS las 1296 filas
    float **sigma = (float**)malloc(palabras_dic * sizeof(float*));
    for(int i = 0; i < palabras_dic; i++){  // CAMBIAR 1195 a 1296
        sigma[i] = (float*)calloc(temas, sizeof(float));
    }
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

float** parametro_gama(float **mtx_1){
    
    
    if (mtx_1 == NULL) {
        printf("Error: mtx_1 es NULL\n");
        return NULL;
    }
    float* n_m = n_ms(mtx_1);
    // Asignar memoria para gama
    float **gama = (float**)malloc(documentos * sizeof(float*));
    for(int i = 0; i < documentos; i++){
        gama[i] = (float*)calloc(temas, sizeof(float));
    }
    // Calcular gama
    for(int k = 0; k < documentos; k++){
        for(int t = 0; t < temas; t++){
            float a = mtx_1[k][t] + alfa;
            float b = n_m[k] + (alfa * temas);  // Sumar alfa * número de tópicos
            
            if (b > 0) {
                gama[k][t] = a / b;
            } else {
                gama[k][t] = 0.0;
            } 
        }
    }
    
    printf("Gama calculado correctamente\n");
    
    // Liberar n_m
    free(n_m);
    
    return gama;
}

float *vector_intervalos(int posDic, int posDocumento, float **mtx_1, float **mtx_2,float * n_m,float* n_k  ){
    float* v = (float*)malloc(temas * sizeof(float));
    
    for(int i = 0; i < temas; i++){
        // Calculamos el primer cociente 
        float a = mtx_2[posDic][i] + betha - 1;
        if (a < 0){
            a = 0;
        }
        float b = (n_k[posDic] + (betha * temas)) - 1;
        
        float primerCociente = (b > 0) ? (a / b) : 0.0;

        // Ahora calculamos el segundo cociente 
        float a_1 = mtx_1[posDocumento][i] + alfa - 1;
        if (a_1 < 0){
            a_1 = 0;
        }
        float b_1 = (n_m[posDocumento] + (alfa * temas)) - 1;
        
        float segundoCociente = (b_1 > 0) ? (a_1 / b_1) : 0.0;

        v[i] = primerCociente * segundoCociente;
    }
    
    
    return v;
}

void free_matriz(float **mtx, int filas) {
    if (mtx == NULL) return;
    for(int i = 0; i < filas; i++) {
        if (mtx[i] != NULL) {
            free(mtx[i]);
        }
    }
    free(mtx);
}

float** matriz_final(float **mtx_1, float **mtx_2, int n_iteraciones, char *docs){
    
    // 1. Abrir archivos UNA SOLA VEZ antes del ciclo
    FILE *d = fopen(docs, "r");
    Diccionario *dic = cargar_diccionario("dic.txt");

    float * n_m = n_ms(mtx_1);
    float * n_k = n_ks(mtx_2);

    if(d == NULL){
        printf("Error al abrir archivos en matriz_final_iterativa\n");
        // Cerrar solo los archivos que se hayan abierto con éxito
        if(d) fclose(d);
        return NULL;
    }
    
    float **mtx_f_resultado = NULL;
    
    // INICIO DEL CICLO: Itera 'n_iteraciones' veces
    for (int iter = 0; iter < n_iteraciones; iter++) {
        
        if (mtx_f_resultado != NULL) {
            free_matriz(mtx_f_resultado, documentos);
        }

        mtx_f_resultado = (float**)malloc(documentos * sizeof(float*));
        if (mtx_f_resultado == NULL) {
             // Manejo de error de malloc
             printf("Error de memoria en la iteración %d\n", iter);
             // Cerrar archivos y salir
             fclose(d);
             return NULL;
        }
        
        for(int i = 0; i < documentos; i++) {
            mtx_f_resultado[i] = (float*)calloc(temas, sizeof(float)); 
            if (mtx_f_resultado[i] == NULL) {
                 
                 printf("Error de memoria en la iteración %d\n", iter);
                 free_matriz(mtx_f_resultado, i); 
                 fclose(d);
                 return NULL;
            }
        }
        rewind(d); 

        char palabra[100];
        int l = 0; 

        while (fscanf(d, "%s", palabra) != EOF) {
            if (es_delimitador(palabra)){
                l++;
            } else {
                if(l > 0 && l <= documentos) {
                    int posicion = buscar_palabra_optimizada(dic,palabra);
                    float *v = vector_intervalos(posicion, l-1, mtx_1, mtx_2,n_m,n_k);
                    
                    // Normalizar el vector v para crear probabilidades
                    float suma = 0.0;
                    for(int j = 0; j < temas; j++){
                        suma += v[j];
                    }
                    
                    if (suma > 0) {
                        // Crear rangos de probabilidad acumulada
                        float *rangos = (float*)malloc((temas + 1) * sizeof(float));
                        rangos[0] = 0.0;
                        for(int j = 0; j < temas; j++){
                            rangos[j+1] = rangos[j] + (v[j] / suma);
                        }
                        
                        // Asignar tópico basado en probabilidad
                        float probabilidad = numeros_aleatorios();
                        for(int j = 0; j < temas; j++){
                            if (probabilidad >= rangos[j] && probabilidad < rangos[j+1]){
                                mtx_f_resultado[l-1][j]++;
                                break;
                            }
                        }
                        free(rangos);
                    }
                    free(v);
                }
            }
        } // Fin del while
        
        printf("Iteración completada: %d/%d\n", iter + 1, n_iteraciones);
    } // FIN DEL CICLO
    
    // 5. Cerrar archivos UNA SOLA VEZ al final
    for(int i = 0; i < dic->num_palabras; i++) {
    free(dic->palabras[i]);
    }
    free(dic->palabras);
    free(dic);
    fclose(d);

    return mtx_f_resultado;
}


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
    float **sigma = parametro_sigma(mtx2);
    
    printf("Calculando parametro_gama...\n");
    float **gama = parametro_gama(mtx1);

    printf("Calculando la ultima matriz...\n");
    int n = 1000;
    float **mtx_final = matriz_final(mtx1, mtx2, n, filename1);
    
    if (sigma == NULL || gama == NULL) {
        printf("Error: sigma o gama es NULL\n");
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
    
    for(int i = 0; i < documentos; i++){
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
            fprintf(estimador1,"%f ", sigma[i][j]);
        }
        fprintf(estimador1,"\n");
    }
    fclose(estimador1);
    printf("sigma.txt guardado correctamente\n");
    
    // Guardar gama
    FILE *estimador2 = fopen("gamma.txt","w");
    if (estimador2 == NULL) {
        printf("Error al abrir gama.txt para escritura\n");
        return 1;
    }
    
    printf("Escribiendo gama.txt...\n");
    for (int i = 0; i < documentos; i++) {
        for (int j = 0; j < temas; j++) {
            fprintf(estimador2,"%f ", gama[i][j]);
        }
        fprintf(estimador2,"\n");
    }
    fclose(estimador2);
    printf("gama.txt guardado correctamente\n");
    
    // Guardar matriz final
    if (mtx_final != NULL) {
        FILE *matrizf_file = fopen("matrizf.txt", "w");
        if (matrizf_file == NULL) {
            printf("Error al abrir matrizf.txt para escritura\n");
        } else {
            printf("Escribiendo matrizf.txt...\n");
            for (int i = 0; i < documentos; i++) {
                for (int j = 0; j < temas; j++) {
                    fprintf(matrizf_file, "%.0f ", mtx_final[i][j]);
                }
                fprintf(matrizf_file, "\n");
            }
            fclose(matrizf_file);
            printf("matrizf.txt guardado correctamente\n");
            
            // Liberar memoria de matriz final
            for(int i = 0; i < documentos; i++) {
                free(mtx_final[i]);
            }
            free(mtx_final);
        }
    } else {
        printf("Advertencia: matriz_final devolvió NULL\n");
    }
    
    // Liberar memoria
    for(int i = 0; i < documentos; i++) {
        free(mtx1[i]);
        free(gama[i]);
    }
    free(mtx1);
    free(gama);
    
    for(int i = 0; i < palabras_dic; i++) {
        free(mtx2[i]);
        free(sigma[i]);
    }
    free(mtx2);
    free(sigma);
    
    printf("Programa finalizado exitosamente\n");
    return 0;
}
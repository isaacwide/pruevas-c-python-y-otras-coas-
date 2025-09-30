#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


const char delimitadores[] = "DOCUMENT1 DOCUMENT2 DOCUMENT3 DOCUMENT4 DOCUMENT5 DOCUMENT6 DOCUMENT7 DOCUMENT8 DOCUMENT9 DOCUMENT10 DOCUMENT11 DOCUMENT12 DOCUMENT13 DOCUMENT14 DOCUMENT15 DOCUMENT16 DOCUMENT17 DOCUMENT18 DOCUMENT19 DOCUMENT20 DOCUMENT21 DOCUMENT22 DOCUMENT23 DOCUMENT24 DOCUMENT25 DOCUMENT26 DOCUMENT27 document1 document2 document3 document4 document5 document6 document7 document8 document9 document10 document11 document12 document13 document14 document15 document16 document17 document18 document19 document20 document21 document22 document23 document24 document25 document26 document27 ";

void word_in_topic(char *filename1, char *filename2) {
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");
    int mtx_1[27][300] = {0} ;

    if(file1 == NULL || file2 == NULL) {
        printf("Error al abrir los archivos.\n");
        return 1;   
    }

    double rangos[301] ={0};
    for(int i = 0 ; i<301 ; i++){
        rangos[i+1 ]= rangos[i] + 0.00333;
    }

    char palabra[100];
    // llenado de matrices
    int l = 0;// cambiamos de documentos 
    //
    while (fscanf(filename1, "%s", palabra) != EOF) {
        if (strtok(palabra, delimitadores)){
            l++;
        }else {
            float probabilidad = numeros_aleatrios();
            for(int j = 0 ; j<301 ; j++){
                if (probabilidad >= rangos[j] && probabilidad < rangos[j+1]){
                    mtx_1[l-1][j]++;
                }
            }
        }
        
    }
    
}


void dic_in_topic(char *filename2, char * filename3) {

}

int numeros_aleatrios() {
    srand(time(NULL));
    return (float)rand() / RAND_MAX;
}

int main() {
    
    char filename1[] = "principito_lemas.txt";
    char filename2[] = "topicos.txt";
    char filename3[] = "dic.txt";

    word_in_topic(filename1, filename2);
    dic_in_topic(filename2,filename3);

}
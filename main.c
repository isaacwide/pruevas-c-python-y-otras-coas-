#include<stdio.h>
#include<stdlib.h>
#include<string.h>


__declspec(dllexport) double area_circle(double radius) {
    return 3.14159 * radius * radius;  
}
__declspec(dllexport) double area_rectangle(double length, double width) {
    return length * width;  
}

__declspec(dllexport) char * conca(char *str1, char *str2) {
    
    char inicio[] = "hola mi amigo";
    char fin[] = "desde c conca";
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    
    char *result = (char *)malloc(strlen(inicio) + len1 + len2 + strlen(fin) + 1);
    if (result == NULL) return NULL;
    strcpy(result, inicio);
    strcat(result, str1);
    strcat(result, str2);
    strcat(result, fin);
    return result;
}

// no regresa boleano porque no me acuerdo como se usan xd 
__declspec(dllexport) int archivo_existe(char *filename){
   FILE *file;
   if((file = fopen(filename, "r"))){
         fclose(file);
         return 1;
    } else {
         return 0;
   }
}
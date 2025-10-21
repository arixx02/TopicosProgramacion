#include "../GuiaHeader/GuiaHeader.h"
#include <stdio.h>


void printInt(void* e, void* datos);
void printFloat(void* e, void* datos);


int main(int argc, char *argv[]){
    if (guia_init()) return 1;
    if(argc<3){
        printf("No hay suficientes argumentos");
        return 1;
    }
    u64 numero;
    if(!parsePosInt(argv[1],&numero)){
        printf("Error de parseo");
        return 1;
    }
    if(esPerfecto(numero)){
        printf("El numero %llu es perfecto",numero);
    }
    size_t i=1;
    if(!parsePosInt(argv[2],&numero)){
        printf("Error de parseo");
        return 1;
    }
    u64 contador=0;
    while(i<numero){
        if(esPrimo(i)){
            printf("\nel numero %zu es primo\n",i);
            contador++;
        }
        i++;
    }
    printf("Primos menores a %zu encontrados: %llu\n",i,contador);
    Vector vector;
    if(!vectorCrear(&vector,sizeof(int))){
        printf("no se pudo crear el vector");
        return 1;
    }
    if(!parsePosInt(argv[3],&numero)){
        printf("Error de parseo");
        return 1;
    }
    for (i = 0; i < numero; i++) {
        int valor = i + 1;
        if (vectorInsertar(&vector, &valor) != TODO_OK) {
            return 1;
        }
    }
    printf("Primer valor del vector u64: %d\n",*((int*)(vector.vec)));

    vectorRecorrer(vector, printInt, NULL);

    vectorDestruir(&vector);

    Vector vec;
    if(!vectorCrear(&vec,sizeof(float))){
        printf("no se pudo crear el vector");
        return 1;
    }
    ingresoU64(&numero);
    for (i = 0; i < numero; i++) {
        float valor = i + 1+.141592;
        if (vectorInsertar(&vec, &valor) != TODO_OK) {
            return 1;
        }
    }

    printf("Primer valor del vector float: %f\n",*((float*)(vec.vec)));

    vectorRecorrer(vec, printFloat, NULL);

    vectorDestruir(&vec);
    ld decimal;
    if(!parsePosDouble("3.14159265",&decimal)){
        return 1;
    }
    printf("Decimal parseado: %.8lf\n",(double)decimal);
    return 0;
}

int ingresoU64(void* n){
    u64* num=(u64*)n;
reinicio:
    printf("ingresar n: ");
    if(scanf("%llu", num) != 1)
    {
        printf("Entrada invalida.\n"); // Limpiar buffer
        int c;
        while((c = getchar()) != '\n' && c != EOF);
        goto reinicio; // o algún valor por defecto
    }
    else
    {
        printf("Ingresaste: %llu\n", *num);
    }
    return TODO_OK;
}

void printInt(void* e, void* datos){
    int* elem = (int*)e;
    printf("[%02d]", *elem);
}

void printFloat(void* e, void* datos){
    float* elem = (float*)e;
    printf("[%.06f]", *elem);
}


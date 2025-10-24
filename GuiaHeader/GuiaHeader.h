#ifndef GUIAHEADER_H_INCLUDED
#define GUIAHEADER_H_INCLUDED

#include <stdlib.h>

typedef unsigned long long u64;

typedef long double ld;

#define TODO_OK 0
#define LLENO 1
#define DUPLICADO 2
#define SIN_MEM 3

typedef int bool;
#define true 1
#define false 0

typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
Vector;

typedef bool (*ingreso)(void* var);

typedef void (*Accion)(void* e, void* datos);

void vectorRecorrer(Vector vector, Accion accion, void* datos);

bool vectorCrear(Vector* vector, size_t tamElem); // malloc

int vectorInsertar(Vector* vector, const void* elem); // realloc

void vectorDestruir(Vector* vector); // free

u64 fibo(int n);

ld absolute(ld n);

char* parsePosInt(char const *cadena,u64* destino);

bool parsePosDouble(char const *cadena,ld* destino);

int guia_init(void); // llamar una vez al iniciar el programa

//EJERCICIOS DE LA GUIA

u64 factorial(int n);

u64 combinatorio(int m, int n);

ld ex(int x, double tol);

ld raizSec(u64 a, double tol);

bool esFibo(int n);

bool esFibonacci(u64 n);

ld seno(ld x);

int esPerfecto(u64 n);

u64 producto(int a, int b);

int* division(unsigned int a,unsigned int b);

u64 sumaHastaN(unsigned int n);

u64 sumaN_pares(unsigned int n);

u64 sumaParesMenoresN(unsigned int n);

bool esPrimo(u64 n);

bool insertarEnPosicionN(Vector* vector,unsigned int n,void* elemento);

#endif // GUIAHEADER_H_INCLUDED

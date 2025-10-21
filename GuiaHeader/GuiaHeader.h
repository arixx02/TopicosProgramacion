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

bool esFibo(int n);

typedef bool (*ingreso)(void* var);

typedef void (*Accion)(void* e, void* datos);

void vectorRecorrer(Vector vector, Accion accion, void* datos);

bool vectorCrear(Vector* vector, size_t tamElem); // malloc

int vectorInsertar(Vector* vector, const void* elem); // realloc

void vectorDestruir(Vector* vector); // free

u64 fibo(u64 n);

ld absolute(ld n);

int esPerfecto(u64 n);

ld raizSec(u64 a, double tol);

char* parsePosInt(char const *cadena,u64* destino);

bool parsePosDouble(char const *cadena,ld* destino);

bool esPrimo(u64 n);

int guia_init(void); // call this once at start

ld ex(int x, double tol);

#endif // GUIAHEADER_H_INCLUDED

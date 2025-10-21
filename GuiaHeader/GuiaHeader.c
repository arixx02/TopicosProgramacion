#include "GuiaHeader.h"
#define TOL 0.001
#define CAP_INI 60
#include <stdio.h>
#define FACTOR_INCR 1.3
#include <locale.h>
#include <string.h>

#define PHI  1.61803399
#define CONJUGATEPHI -0.61803399
#define RAIZ 2.236067977
#define binet(n) ((pow(PHI,n)+pow(CONJUGATEPHI,n))/RAIZ)

bool esFibo(int n){ //prueba heuristica usando la formula de binet
    int num=1;
    while(absolute(binet(num)-n)>0.5 && binet(num)-n<1){
        num++;
    }
    if(binet(num)-n>=1)return false;
    return true;
}

#ifdef _WIN32
#include <windows.h>
#endif


int guia_init(void) {
    // Set C locale to user default
    if (!setlocale(LC_ALL, "")) {
        fprintf(stderr, "Warning: could not set locale\n");
        return 1;
    }
    /*
    // Windows console UTF-8 output
    #ifdef _WIN32
        if (!SetConsoleOutputCP(CP_UTF8)) {
            fprintf(stderr, "Warning: could not set Windows console code page\n");
            return 1;
        }
    #endif
    */   //la consola de windows ya maneja UTF-8 entonces esto lo rompe, pero porque?

    return TODO_OK;
}

bool redimensionarVector(Vector* vector, float factor);

#define OFFSET(ptr, bytes) ((void*)((char*)(ptr) + (bytes)))



void vectorRecorrer(Vector vector, Accion accion, void* datos){
    char* ini = (char*)vector.vec;
    char* ult =  OFFSET(vector.vec, (vector.ce - 1) * vector.tamElem);
    for(char* i = ini; i <= ult; i += vector.tamElem)
    {
        accion(i, datos);
    }
    putchar('\n');
}

bool vectorCrear(Vector* vector, size_t tamElem){
    vector->ce = 0;

    vector->vec = malloc(CAP_INI * tamElem);

    if(vector->vec == NULL)
    {
        vector->cap = 0;
        vector->tamElem = 0;
        return false;
    }

    vector->cap = CAP_INI;
    vector->tamElem = tamElem;

    return true;
}


int vectorInsertar(Vector* vector, const void* elem){
    if(vector->ce == vector->cap)
    {
        if(!redimensionarVector(vector, FACTOR_INCR))
        {
            return SIN_MEM;
        }
    }
    void* insercion = OFFSET(vector->vec, vector->ce * vector->tamElem);//si aca escribo vector->ce * vector->tamElem cada elemento se miltiplica por 256=2^(8)

    memcpy(insercion, elem, vector->tamElem);
    vector->ce++;

    return TODO_OK;
}


bool redimensionarVector(Vector* vector, float factor){
    size_t nuevaCap = vector->cap * factor;

    if(nuevaCap < CAP_INI)
    {
        return true;
    }

    void* nuevoVec = realloc(vector->vec, nuevaCap * vector->tamElem);

    if(nuevoVec == NULL)
    {
        return false;
    }

    printf("Redimensión de %zu a %zu\n", vector->cap, nuevaCap);

    vector->vec = nuevoVec;
    vector->cap = nuevaCap;

    return true;
}

void vectorDestruir(Vector* vector){
    free(vector->vec);
    // vector->vec = NULL;
    vector->ce = 0;
    vector->cap = 0;
}

ld absolute(ld n){
    if(n<0){
        return -n;
    }
    return n;
}

char* parsePosInt(char const *cadena,u64* destino){
    if(!cadena){
        printf("Ingresar string no nulo\n");
        return NULL;
    }
    u64 resultado=0;
    u64 siguienteDig;
    const char* ptr=cadena;
    while((*ptr)>='0' && (*ptr)<='9'){
            siguienteDig=resultado*10+(*ptr)-'0';
            if(siguienteDig<resultado){
                printf("OVERFLOW");
                return NULL;
            }
            resultado=siguienteDig;
            ptr++;
    }
    *destino=resultado;
    return ptr;
}

ld raizSec(u64 a, double tol){ //metodo Newton-Raphson (babilonico)
    if(a==1)return 1;
    ld anterior=1;
    ld res=1;
    ld aux;
    while(absolute(res-anterior)>tol || res/anterior==1){
        aux=res;
        res=0.5*(anterior+(ld)a/anterior);
        anterior=aux;
    }
    return res;
}


int esPerfecto(u64 n){
    u64 suma=1;
    u64 rango=(u64)raizSec(n,TOL);
    for(u64 i=2;i<=rango;i++){
        if(n%i==0){
            suma+=i;
            suma+=n/i;
        }
        if(i*i==n){
            suma-=n/i;
        }
    }
    if(suma==n){
        return 1;//es perfecto
    }
    if(suma>n){
        return 0;//es abudante
    }
    if(suma<n){
        return -1;//es deficiente
    }
    return -2;
}

bool esPrimo(u64 n){
    bool primo=true;
    if(n%2==0 && n>2){
        primo=false;
    }
    u64 i=3;
    u64 rango=(u64)raizSec(n,TOL);
    while((primo==true) && !(i>rango)){
        if(n%i==0){
            primo=false;
        }
        i+=2;
    }
    return primo;
}

u64 fibo(u64 n){
    if(n<1)return -1;
    u64 anterior=0,fibo=1;
    if(n==1)return anterior;
    if(n==2)return fibo;
    for(size_t i=1; i<n-1; i++)
    {
        anterior^=fibo;
        fibo^=anterior;
        anterior^=fibo;
        fibo+=anterior;
    }
    return fibo;
}

bool parsePosDouble(char const *cadena,ld* destino){
    if(!cadena){
        printf("Ingresar string no nulo\n");
        return NULL;
    }
    ld res;
    u64 temp,tempDecimal;
    const char *ptr=parsePosInt(cadena,&temp);
    if(!ptr){
        return false;
    }
    if(temp>INT_MAX)return false;
    if(*ptr!='.')return false;
    const char* ceros=ptr;
    ceros++;
    int contadorCeros=0;
    while(*ceros=='0'){
            contadorCeros++;
            if(contadorCeros>15){
                printf("Parte decimal es practicamente 0\n");
                *destino=(ld)temp;
                return true;
            }
            ceros++;
    }
    if(!parsePosInt(ceros,&tempDecimal)){
        return false;
    }
    res=(ld)tempDecimal;
    while(res>=1){
        res/=10;
    }
    for(int i=0;i<contadorCeros;i++){
        res/=10;
    }
    *destino=(ld)temp+res;
    return true;
}

ld ex(int x, double tol){
    ld res=0;
    unsigned int contador=1;
    ld siguiente=1;
    while(tol-siguiente<0){
        res+=siguiente;
        siguiente*=x/(ld)contador;
        contador++;
    }
    return res;
}

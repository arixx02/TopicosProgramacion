#include "GuiaHeader.h"
#define TOL 0.00001
#define CAP_INI 60
#include <stdio.h>
#define FACTOR_INCR 1.3
#include <locale.h>
#include <string.h>
#include <math.h>

#define PHI  1.61803399
#define CONJUGATEPHI -0.61803399
#define RAIZ 2.236067977
#define binet(n) ((pow(PHI,n)+pow(CONJUGATEPHI,n))/RAIZ)

#ifdef _WIN32
#include <windows.h>
#endif

#define OFFSET(ptr, bytes) ((void*)((char*)(ptr) + (bytes)))

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



/*
usando xor para intercambio
u64 fibo(int n){
    if(n<1)return -1;
    u64 anterior=0,fibo=1;
    if(n==1)return anterior;
    if(n==2)return fibo;
    for(int i=1; i<n; i++)
    {
        anterior^=fibo;
        fibo^=anterior;
        anterior^=fibo;
        fibo+=anterior;
    }
    return fibo;
}
*/
u64 fibo(int n){
    if(n<1)return -1;
    u64 anterior=0,fibo=1;
    if(n==1)return anterior;
    if(n==2)return fibo;
    for(int i=1; i<n; i++)
    {
        fibo+=anterior; //actual=actual
        anterior=fibo-anterior; //anterior=actual+anterior-actual=anterior
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

//EJERCICIOS DE LA GUIA-------------------------------------------------------------------------------------------

u64 factorial(int n){
    u64 res=1;
    for(int i=1;i<=n;i++){
        res*=i;
    }
    return res;
}

u64 combinatorio(int m, int n){
    if(m<n){
        u64 aux=m;
        m=n;
        n=aux;
    }
    return factorial(m)/(factorial(n)*factorial(m-n));
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

bool esFibo(int n){ //prueba heuristica usando la formula de binet
    int num=1;
    while(absolute(binet(num)-n)>0.5 && binet(num)-n<1){
        num++;
    }
    if(binet(num)-n>=1)return false;
    return true;
}

bool esFibonacci(u64 n){
    u64 anterior=0, actual=1;
    while(actual<n && anterior<=actual){
        actual+=anterior;
        anterior=actual-anterior;
    }
    if(actual==n){
        printf("%llu ES FIBO",n);
        return true;
    }
    return false;
}

ld seno(ld x){
    ld res=0;
    unsigned int contador=1;
    ld siguiente=x;
    while(absolute(siguiente)>TOL){
        res+=siguiente;
        contador+=2;
        siguiente*=(-x*x)/(ld)(contador*(contador-1));
    }
    return res;
}

u64 producto(int a, int b){
    u64 res=0;
    for(u64 i=0;i<b;i++){
        res+=a;
    }
    return res;
}

int* division(unsigned int a,unsigned int b){
    static int division[2]={0,0};
    if(b==0){
        return NULL;
    }
    int cociente=0;
    int resto=a;
    while(resto>=b){
        resto-=b;
        cociente++;
    }
    division[0]=cociente;
    division[1]=resto;
    return division;
}

u64 sumaHastaN(unsigned int n){
    return (u64)n * (n+1) /2;
}

u64 sumaN_pares(unsigned int n){
    return (u64)n * (n+1);
}

u64 sumaParesMenoresN(unsigned int n){
    unsigned int k = (n-1)/2;
    return (u64)k*(k+1);
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

bool insertarEnPosicionN(Vector* vector,unsigned int n,void* elemento){
    if(n>vector->ce){
        return false;
    }
    void* cursor=vector->vec;
    cursor=OFFSET(vector->vec,n*vector->tamElem);
    memcpy(cursor,elemento,vector->tamElem);
    if(n==vector->ce){
        vector->ce++;
    }
    return true;
}

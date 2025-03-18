#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef	struct Casilla{
    int jugador;
    int cantidad;
}*Elem;

Elem casillaVacia(){
    Elem nuevo = (Elem)malloc(sizeof(struct Casilla));
    nuevo->jugador = 0;
    nuevo->cantidad = 0;
    return nuevo;
}


Elem crearDato(int j, int c){
    Elem nuevo = (Elem)malloc(sizeof(struct Casilla));
    nuevo->jugador = j;
    nuevo->cantidad = c;
    return nuevo;
}


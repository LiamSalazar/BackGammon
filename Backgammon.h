# include <stdio.h>
# include <stdlib.h>
#include <time.h>
#include".\Elem\Casilla.h"
#include".\BiCola\BiCola.h"

BiCola tablero;
Elem barJugador1, barJugador2;

void inicializarJuego() {
    tablero = nuevaBi();
    barJugador1 = crearDato(1, 0);
    barJugador2 = crearDato(2, 0);
    for (int i = 1; i < 25; i++) {
        Elem casilla;
        // Posición inicial de las fichas
        if (i == 1) casilla = crearDato(1, 2);
        else if (i == 12) casilla = crearDato(1, 5);
        else if (i == 17) casilla = crearDato(1, 3);
        else if (i == 19) casilla = crearDato(1, 5);
        else if (i == 24) casilla = crearDato(2, 2);
        else if (i == 13) casilla = crearDato(2, 5);
        else if (i == 6) casilla = crearDato(2, 5);
        else if (i == 8) casilla = crearDato(2, 3);
        else casilla = crearDato(0, 0);
        formarDer(tablero, casilla);
    }
}

// Test fase final
// void inicializarJuego() {
//     tablero = nuevaBi();
//     barJugador1 = crearDato(1, 0);
//     barJugador2 = crearDato(2, 0);
// 
//     for (int i = 1; i < 25; i++) {
//         Elem casilla;
//         if (i >= 19 && i <= 24) casilla = crearDato(1, 1); 
//         else if (i >= 1 && i <= 6) casilla = crearDato(2, 1);
//         else casilla = crearDato(0, 0);  // Casillas vacías
//         formarDer(tablero, casilla);
//     }
// }

void tirarDados(int *dado1, int *dado2) {
    *dado1 = rand() % 6 + 1;
    *dado2 = rand() % 6 + 1;
}

Elem obtenerCasilla(BiCola *tablero, int posicion) {
    if (posicion < 1 || posicion > 24) return NULL;
    BiCola copia = nuevaBi(); 
    BiCola temp = *tablero;  
    Elem casilla = NULL;
    int contador = 1;  
    while (!esnueva(temp)) {
        if (contador == posicion) {
            casilla = izquierdo(temp);  
        }
        copia = formarDer(copia, izquierdo(temp));  
        temp = desformarIzq(temp); 
        contador++;
    }
    *tablero = copia;  
    return casilla;  
}

int bloqueoFicha(Elem destino, int jugador) {
    if (destino == NULL) return 1;
    if (destino->cantidad >= 5) return 1;
    return (destino->jugador != jugador && destino->cantidad >= 2);
}

int obtenerPosicionOrigen(int jugador) {
    int posicion;
    Elem origen = NULL;
    while (1) {
        printf("Jugador %d, seleccione la casilla de donde quiere mover una ficha: ", jugador);
        scanf("%d", &posicion);
        if (posicion < 1 || posicion > 24) {
            printf("No existe esa casilla.\n");
            continue;
        }
        origen = obtenerCasilla(&tablero, posicion);
        if (origen->cantidad == 0 || origen->jugador != jugador) {
            printf("No hay fichas disponibles en esta casilla. Seleccione otra.\n");
            continue;
        }
        return posicion;
    }
}

void comerFicha(Elem origen, Elem destino, int jugador) {
    if (destino == NULL) return;
    printf("Jugador %d se comió una ficha\n", jugador);
    if (destino->jugador == 1) barJugador1->cantidad++;
    else if (destino->jugador == 2) barJugador2->cantidad++;
    
    destino->jugador = jugador;
    destino->cantidad = 1;
    origen->cantidad--;
    
    if (origen->cantidad == 0) {
        origen->jugador = 0;
    }
}

void moverFicha(Elem origen, Elem destino, int jugador, int posicion, int nuevaPos) {
    if (destino->jugador != jugador && destino->cantidad == 1) {
        comerFicha(origen, destino, jugador);
        return;
    }
    destino->cantidad++;
    destino->jugador = jugador;
    origen->cantidad--;
    if (origen->cantidad == 0) {
        origen->jugador = 0;
    }
    printf("Jugador %d movió una ficha de la casilla %d a la %d.\n", jugador, posicion, nuevaPos);
}

int puedeSacarFichas(int jugador) {
    int inicio = (jugador == 1) ? 1 : 7; 
    int fin = (jugador == 1) ? 18 : 24;
    for (int i = inicio; i <= fin; i++) {
        Elem casilla = obtenerCasilla(&tablero, i);
        if (casilla != NULL && casilla->jugador == jugador && casilla->cantidad > 0) {
            return 0;  
        }
    }
    return 1;  
}

void sacarFichaDeBarra(int jugador, int dado) {
    Elem bar = (jugador == 1) ? barJugador1 : barJugador2;
    if (bar->cantidad > 0) {
        puts("Fichas en la barra, primero se deben sacar esas");
        int nuevaPos = (jugador == 1) ? dado : 25 - dado;
        Elem destino = obtenerCasilla(&tablero, nuevaPos);
        if (destino == NULL) {
            printf("Casilla destino inválida. No se puede meter la ficha al tablero.\n");
            return;
        }
        if (destino->jugador != jugador && destino->cantidad == 1) {
            comerFicha(bar, destino, jugador);
            return;  
        }
        if (bloqueoFicha(destino, jugador)) {
            printf("Casilla bloqueada. No se puede meter la ficha al tablero.\n");
            return;
        }
        destino->cantidad++;
        destino->jugador = jugador;
        bar->cantidad--;
        printf("Jugador %d metió una ficha en la casilla %d.\n", jugador, nuevaPos);
    }
}


int jugadorTieneMovimientos(int jugador, int dado) {
    Elem bar = (jugador == 1) ? barJugador1 : barJugador2;
    if (bar->cantidad > 0) {
        int nuevaPos = (jugador == 1) ? dado : 25 - dado;
        Elem destino = obtenerCasilla(&tablero, nuevaPos);
        if (destino != NULL && !bloqueoFicha(destino, jugador)) {
            return 1; 
        }
        return 0; 
    }
    for (int i = 1; i <= 24; i++) {
        Elem origen = obtenerCasilla(&tablero, i);
        if (origen == NULL) continue;
        if (origen->jugador == jugador && origen->cantidad > 0) {
            int nuevaPos = (jugador == 1) ? i + dado : i - dado;
            if (nuevaPos > 24 || nuevaPos < 1) {
                if (puedeSacarFichas(jugador)) return 1;
                continue; 
            }
            Elem destino = obtenerCasilla(&tablero, nuevaPos);
            if (destino != NULL && !bloqueoFicha(destino, jugador)) {
                return 1; 
            }
        }
    }
    return 0; 
}



void avanzarFicha(int jugador, int dado) {
    if (!jugadorTieneMovimientos(jugador, dado)) {
        printf("Jugador %d no tiene movimientos disponibles. Turno saltado.\n", jugador);
        return;
    }
    Elem bar = (jugador == 1) ? barJugador1 : barJugador2;
    if (bar->cantidad > 0) {
        sacarFichaDeBarra(jugador, dado);
        return;
    }
    int posicion, nuevaPos;
    while (1) {
        posicion = obtenerPosicionOrigen(jugador);
        nuevaPos = (jugador == 1) ? posicion + dado : posicion - dado;
        if ((jugador == 1 && nuevaPos > 24) || (jugador == 2 && nuevaPos < 1)) {
            if (puedeSacarFichas(jugador)) {
                Elem origen = obtenerCasilla(&tablero, posicion);
                origen->cantidad--;  
                if (origen->cantidad == 0) {
                    origen->jugador = 0;  
                }
                printf("Jugador %d sacó una ficha del tablero.\n", jugador);
                return;
            } else {
                printf("Aún no puedes sacar fichas. Selecciona otra ficha\n");
                continue;
            }
        }
        Elem origen = obtenerCasilla(&tablero, posicion);
        Elem destino = obtenerCasilla(&tablero, nuevaPos);
        if (origen == NULL || destino == NULL) {
            printf("Error al acceder a casillas. Inténtalo de nuevo.\n");
            continue;
        }
        if (!bloqueoFicha(destino, jugador)) {
            moverFicha(origen, destino, jugador, posicion, nuevaPos);
            return;
        } else {
            printf("Casilla bloqueada. Selecciona otra ficha\n");
            continue;
        }
    }
}


int verificarGanador(int jugador) {
    Elem bar = (jugador == 1) ? barJugador1 : barJugador2;
    if(bar->cantidad > 0)
        return 0;
    for (int i = 1; i <= 24; i++) {
        Elem casilla = obtenerCasilla(&tablero, i);
        if (casilla->jugador == jugador && casilla->cantidad > 0) {
            return 0; 
        }
    }
    return 1; 
}

void imprimirTablero() {
    printf("\nTABLERO BACKGAMMON\n");
    // Imprimir casillas de arriba
    printf("\n13\t14\t15\t16\t17\t18\t19\t20\t21\t22\t23\t24\n");
    for (int i = 13; i <= 24; i++) {
        Elem casilla = obtenerCasilla(&tablero, i);
        if (casilla->cantidad > 0) {
            printf("(%d%d)\t", casilla->jugador, casilla->cantidad);
        } else {
            printf("--\t");
        }
    }
    printf("\n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    // Imprimir casillas de abajo
    printf("12\t11\t10\t9\t8\t7\t6\t5\t4\t3\t2\t1\n");
    for (int i = 12; i >= 1; i--) {
        Elem casilla = obtenerCasilla(&tablero, i);
        if (casilla->cantidad > 0) {
            printf("(%d%d)\t", casilla->jugador, casilla->cantidad);
        } else {
            printf("--\t");
        }
    }
    printf("\n");
    // Imprimir fichas de la barra
    printf("\nBAR\n");
    printf("Jugador 1: %d fichas\tJugador 2: %d fichas\n", barJugador1->cantidad, barJugador2->cantidad);
    printf("\n-------------------------------------------------------------\n");
}


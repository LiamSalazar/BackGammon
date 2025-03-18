#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include".\Elem\Casilla.h"
#include".\BiCola\BiCola.h"
#include".\Backgammon.h"

int main() {
    srand(time(NULL));
    inicializarJuego();
    int jugador = 1;
    int dado1, dado2;
    while (1) {
        printf("\nTurno del Jugador %d\n", jugador);
        tirarDados(&dado1, &dado2);
        imprimirTablero();
        printf("Dados: %d y %d\n", dado1, dado2);
        if (dado1 == dado2) {
            printf("Doble %d. Puedes mover cuatro veces.\n", dado1);
            for (int i = 0; i < 4; i++) {
                avanzarFicha(jugador, dado1);
                if (verificarGanador(jugador)) {
                    printf("\nJugador %d ganó\n", jugador);
                    return 0;
                }
                imprimirTablero();
            }
        } else {
            avanzarFicha(jugador, dado1);
            if (verificarGanador(jugador)) {
                printf("\nJugador %d ganó\n", jugador);
                return 0;
            }
            imprimirTablero();
            avanzarFicha(jugador, dado2);
            if (verificarGanador(jugador)) {
                printf("\nJugador %d ganó\n", jugador);
                return 0;
            }
            imprimirTablero();
        }
        jugador = (jugador == 1) ? 2 : 1;
    }
    return 0;
}
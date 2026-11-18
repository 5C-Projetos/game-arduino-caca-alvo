#include <Keypad.h>

#define TLIN  4     // número de linhas (teclado)
#define TCOL  3     // número de colunas (teclado)

byte pinos_lin[TLIN] = {5, 6, 7, 8};    // Pinos - Linhas
byte pinos_col[TCOL] = {2, 3, 4};      // Pinos - Colunas

// Matriz Caracteres
char mat_teclas[TLIN][TCOL] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

Keypad teclado = Keypad( makeKeymap(mat_teclas), pinos_lin, pinos_col, TLIN, TCOL); 
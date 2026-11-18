#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

const byte LINHAS = 4;
const byte COLUNAS = 3;
char teclas[LINHAS][COLUNAS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pinosLinhas[LINHAS] = {9, 8, 7, 6};
byte pinosColunas[COLUNAS] = {5, 4, 3};

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

// Mapeamento espacial das 6 teclas superiores (grade 2x3)
const char mapaTeclas[6] = {'1', '2', '3', '4', '5', '6'};

int alvoAtual = -1;
unsigned long tempoInicio = 0;
unsigned long tempoLimite = 2000; // Começa com 2 segundos por alvo
int pontos = 0;
int vidas = 3;
bool jogando = true;

void desenharTabuleiro() {
  for (int i = 0; i < 6; i++) {
    int linha = (i < 3) ? 0 : 1;
    int coluna = (i % 3) * 2;
    lcd.setCursor(coluna, linha);
    lcd.print(i == alvoAtual ? '#' : '_');
  }
  
  lcd.setCursor(9, 0);
  lcd.print("Pts:");
  if (pontos < 100) lcd.print('0');
  if (pontos < 10) lcd.print('0');
  lcd.print(pontos);

  lcd.setCursor(9, 1);
  lcd.print("Vidas:");
  lcd.print(vidas);
}

void novoAlvo() {
  int proximo;
  do {
    proximo = random(0, 6);
  } while (proximo == alvoAtual); // Evita repetir a mesma posição seguida
  
  alvoAtual = proximo;
  desenharTabuleiro();
  tempoInicio = millis();
}

void iniciarJogo() {
  pontos = 0;
  vidas = 3;
  tempoLimite = 2000;
  jogando = true;
  lcd.clear();
  novoAlvo();
}

void perderVida() {
  vidas--;
  if (vidas <= 0) {
    jogando = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   GAME OVER!   ");
    lcd.setCursor(0, 1);
    lcd.print("Pts:");
    lcd.print(pontos);
    lcd.setCursor(11, 1);
    lcd.print("(#)  ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("ERROU! ");
    delay(450);
    lcd.clear();
    novoAlvo();
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(A0));
  iniciarJogo();
}

void loop() {
  char tecla = teclado.getKey();

  if (!jogando) {
    if (tecla == '#') iniciarJogo();
    return;
  }

  // Verifica se o tempo para acertar o alvo esgotou
  if (millis() - tempoInicio >= tempoLimite) {
    perderVida();
    return;
  }

  if (tecla != NO_KEY) {
    if (tecla == mapaTeclas[alvoAtual]) {
      pontos++;
      if (tempoLimite > 550) {
        tempoLimite -= 65; // Acelera o jogo progressivamente
      }
      novoAlvo();
    } else {
      perderVida();
    }
  }
}

/*

As duas linhas superiores do teclado matricial (1 2 3 e 4 5 6 ) 
são mapeadas diretamente para 6 posições no lado esquerdo do display. 
Um alvo (#) surge aleatoriamente em uma das posições e você precisa 
pressionar a tecla correspondente antes que o tempo acabe. A cada acerto, 
a velocidade aumenta.

Controles: 
Teclas 1, 2, 3 (linha superior) e 4, 5, 6 (linha inferior) acertam os alvos; 
# reinicia após o Game Over.

*/
